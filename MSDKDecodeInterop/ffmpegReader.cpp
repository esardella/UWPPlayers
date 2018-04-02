#include "pch.h"
#include "ffmpegReader.h"
#include "shcore.h"


#define INITIAL_BITSTREAM_SIZE 100000

const int FILESTREAMBUFFERSZ = 16384;
// Static functions passed to FFmpeg
static int FileStreamRead(void* ptr, uint8_t* buf, int bufSize);
static int64_t FileStreamSeek(void* ptr, int64_t pos, int whence);

ffmpegReader::ffmpegReader()
	: avDict(nullptr)
	, avIOCtx(nullptr)
	, avFormatCtx(nullptr)
	, avAudioCodecCtx(nullptr)
	, avVideoCodecCtx(nullptr)
	, fileStreamData(nullptr)
	, fileStreamBuffer(nullptr)
{

	if (!isRegistered)
	{
		av_register_all();
		//av_lockmgr_register(lock_manager);
		isRegistered = true;
	}
	framePts = frameDts = frameDuration = 0;
}

ffmpegReader::~ffmpegReader()
{
	stream = nullptr;
	dataReader = nullptr;

	m_bInited = false;
	fileSize = 0;
	bytesCompleted = 0;
	delete[] BitStream.Data;
	MSDK_ZERO_MEMORY(BitStream);
}

void ffmpegReader::Reset()
{
	if (!m_bInited)
		return;

	stream->Seek(0);
	bytesCompleted = 0;

}

void ffmpegReader::Close()
{
	stream = nullptr;
	dataReader = nullptr;

	m_bInited = false;
	fileSize = 0;
	bytesCompleted = 0;
	delete[] BitStream.Data;
	MSDK_ZERO_MEMORY(BitStream);

}

mfxStatus ffmpegReader::Init(Windows::Storage::StorageFile ^ fileSource)
{
	HRESULT hr = S_OK;
	mfxStatus sts = MFX_ERR_NONE;
	

	if (fileSource && !fileSource->IsAvailable)
	{
		return MFX_ERR_NULL_PTR;
	}

	Close();

	auto task = concurrency::create_task(fileSource->OpenReadAsync());
	task.wait();

	Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream = task.get();
	fileSize = stream->Size;
	hr = CreateStreamOverRandomAccessStream(reinterpret_cast<IUnknown*>(stream), IID_PPV_ARGS(&fileStreamData));
	if (SUCCEEDED(hr))
	{
		// Setup FFmpeg custom IO to access file as stream. This is necessary when accessing any file outside of app installation directory and appdata folder.
		// Credit to Philipp Sch http://www.codeproject.com/Tips/489450/Creating-Custom-FFmpeg-IO-Context
		fileStreamBuffer = (unsigned char*)av_malloc(FILESTREAMBUFFERSZ);
		if (fileStreamBuffer == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		avIOCtx = avio_alloc_context(fileStreamBuffer, FILESTREAMBUFFERSZ, 0, fileStreamData, FileStreamRead, 0, FileStreamSeek);
		if (avIOCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		avFormatCtx = avformat_alloc_context();
		if (avFormatCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		avFormatCtx->pb = avIOCtx;
		avFormatCtx->flags |= AVFMT_FLAG_CUSTOM_IO;

		// Open media file using custom IO setup above instead of using file name. Opening a file using file name will invoke fopen C API call that only have
		// access within the app installation directory and appdata folder. Custom IO allows access to file selected using FilePicker dialog.
		if (avformat_open_input(&avFormatCtx, "", NULL, &avDict) < 0)
		{
			hr = E_FAIL; // Error opening file
		}

		// avDict is not NULL only when there is an issue with the given ffmpegOptions such as invalid key, value type etc. Iterate through it to see which one is causing the issue.
		if (avDict != nullptr)
		{
			DebugMessage(L"Invalid FFmpeg option(s)");
			av_dict_free(&avDict);
			avDict = nullptr;
		}
	}

	//End File Setup 
	CompleteFFMPEGInit();


}

mfxStatus ffmpegReader::InitURI(Platform::String ^ uri)
{
	//RTMP Support. 
	HRESULT hr = S_OK;
	const char* charStr = nullptr;
	if (!uri)
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr))
	{
		avFormatCtx = avformat_alloc_context();
		if (avFormatCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		std::wstring uriW(uri->Begin());
		std::string uriA(uriW.begin(), uriW.end());
		charStr = uriA.c_str();

		// Open media in the given URI using the specified options
		if (avformat_open_input(&avFormatCtx, charStr, NULL, &avDict) < 0)
		{
			hr = E_FAIL; // Error opening file
		}

		// avDict is not NULL only when there is an issue with the given ffmpegOptions such as invalid key, value type etc. Iterate through it to see which one is causing the issue.
		if (avDict != nullptr)
		{
			DebugMessage(L"Invalid FFmpeg option(s)");
			av_dict_free(&avDict);
			avDict = nullptr;
		}
	}
	CompleteFFMPEGInit();
	return mfxStatus();
}

mfxStatus ffmpegReader::CompleteFFMPEGInit()
{
	HRESULT hr= S_OK; 
	mfxStatus sts; 
	AVCodec* avVideoCodec = nullptr;


	if (avformat_find_stream_info(avFormatCtx, NULL) < 0)
		{
			hr = E_FAIL; // Error finding info
		}
	

	videoStreamIndex = av_find_best_stream(avFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &avVideoCodec, 0);
	if (videoStreamIndex != AVERROR_STREAM_NOT_FOUND && avVideoCodec)
	{
		avVideoCodecCtx = avcodec_alloc_context3(avVideoCodec);
		if (!avVideoCodecCtx)
		{
			DebugMessage(L"Could not allocate a decoding context\n");
			avformat_close_input(&avFormatCtx);
			hr = E_OUTOFMEMORY;
		}

		if (SUCCEEDED(hr))
		{
			// initialize the stream parameters with demuxer information
			if (avcodec_parameters_to_context(avVideoCodecCtx, avFormatCtx->streams[videoStreamIndex]->codecpar) < 0)
			{
				avformat_close_input(&avFormatCtx);
				avcodec_free_context(&avVideoCodecCtx);
				hr = E_FAIL;
			}
		}
		//Load the Bistream filter needed 
		if (avVideoCodecCtx->codec_id == AV_CODEC_ID_H264)
		{
			int ret;
			bsf = av_bsf_get_by_name("h264_mp4toannexb");
			ret = av_bsf_alloc(bsf, &ctx);
			ret = avcodec_parameters_copy(ctx->par_in, avFormatCtx->streams[videoStreamIndex]->codecpar);
			ctx->time_base_in = avFormatCtx->streams[0]->time_base;
			hr = av_bsf_init(ctx);
		}

	}

	MSDK_ZERO_MEMORY(BitStream);
	BitStream.Data = new(std::nothrow) unsigned char[1024 * 1024];
	MSDK_CHECK_POINTER_SAFE(BitStream.Data, MFX_ERR_NULL_PTR, Close());
	BitStream.MaxLength = 1024 * 1024;

	hr == S_OK ? sts = MFX_ERR_NONE : sts = MFX_ERR_NOT_INITIALIZED;

	return sts;
}
 

mfxStatus ffmpegReader::ReadNextFrame()
{
	
	int ret;
	mfxStatus sts = MFX_ERR_NONE; 
	bool frameFound = false;
	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = NULL;
	avPacket.size = 0;
	HRESULT hr = S_OK;
	static AVRational q = { 1, 90000 };

	while (!frameFound)
	{
		if (!av_read_frame(avFormatCtx, &avPacket))
		{

			if (avPacket.stream_index == videoStreamIndex)
			{
				int ret;
				ret = av_bsf_send_packet(ctx, &avPacket);
				if (ret == AVERROR(EAGAIN))
				{
					_ASSERT(FALSE);
					DebugMessage(L"Failed TO Filter bitstream");
				}
				if (ret < 0)
				{
					hr = E_FAIL;
				}
				ret = av_bsf_receive_packet(ctx, &avPacket);
				if (ret == AVERROR(EAGAIN) || ret < 0) {
					DebugMessage(L"Failed to read Filtered bitstream");
					hr = E_FAIL;
				}

				memmove(BitStream.Data, BitStream.Data + BitStream.DataOffset, BitStream.DataLength);
				BitStream.DataOffset = 0;
				memcpy(BitStream.Data + BitStream.DataLength, avPacket.data, avPacket.size);
				BitStream.DataLength += avPacket.size;
				BitStream.DataFlag = MFX_BITSTREAM_COMPLETE_FRAME;
				if (avPacket.pts == AV_NOPTS_VALUE)
				{
					BitStream.TimeStamp = (mfxU64)MFX_TIMESTAMP_UNKNOWN;

				}
				else
				{
					BitStream.TimeStamp = avPacket.pts; // av_rescale_q(avPacket.pts, avFormatCtx->streams[videoStreamIndex]->time_base, q);
				}

				if (avPacket.dts == AV_NOPTS_VALUE)
				{
					BitStream.DecodeTimeStamp = MFX_TIMESTAMP_UNKNOWN;
				}
				else
				{
					BitStream.DecodeTimeStamp = avPacket.dts; // av_rescale_q(avPacket.dts, avFormatCtx->streams[videoStreamIndex]->time_base, q);
				}

				frameDuration = avPacket.duration;
				frameDts = avPacket.dts;

				frameFound = true;
				av_packet_unref(&avPacket);

			}
			else
			{
				continue;
			}

		}


	}
	hr == S_OK ? sts = MFX_ERR_NONE : sts = MFX_ERR_UNKNOWN;
	return sts;
 
}


// Static function to read file stream and pass data to FFmpeg. Credit to Philipp Sch http://www.codeproject.com/Tips/489450/Creating-Custom-FFmpeg-IO-Context
static int FileStreamRead(void* ptr, uint8_t* buf, int bufSize)
{
	IStream* pStream = reinterpret_cast<IStream*>(ptr);
	ULONG bytesRead = 0;
	HRESULT hr = pStream->Read(buf, bufSize, &bytesRead);

	if (FAILED(hr))
	{
		return -1;
	}

	// If we succeed but don't have any bytes, assume end of file
	if (bytesRead == 0)
	{
		return AVERROR_EOF;  // Let FFmpeg know that we have reached eof
	}

	return bytesRead;
}

// Static function to seek in file stream. Credit to Philipp Sch http://www.codeproject.com/Tips/489450/Creating-Custom-FFmpeg-IO-Context
static int64_t FileStreamSeek(void* ptr, int64_t pos, int whence)
{
	IStream* pStream = reinterpret_cast<IStream*>(ptr);
	LARGE_INTEGER in;
	in.QuadPart = pos;
	ULARGE_INTEGER out = { 0 };

	if (FAILED(pStream->Seek(in, whence, &out)))
	{
		return -1;
	}

	return out.QuadPart; // Return the new position:
}