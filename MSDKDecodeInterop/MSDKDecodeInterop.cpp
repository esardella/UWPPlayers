#include "pch.h"
#include "MSDKDecodeInterop.h"



using namespace MSDKDecodeInterop; 
using namespace Windows::Media::Core;
using namespace Windows::Media::MediaProperties;


MSDKInterop ^ MSDKDecodeInterop::MSDKInterop::CreatefromFile(Windows::Storage::StorageFile^ file)
{
	auto msdkDecoder = ref new MSDKInterop();
	if (FAILED(msdkDecoder->pipeline.Init()))
	{
		msdkDecoder = nullptr; 
	}

	 
	if (file->FileType == ".264" || file->FileType == ".h264")
	{
		msdkDecoder->pipeline.SetCodecID(MFX_CODEC_AVC);
	}
	else if (file->FileType == ".m2v" || file->FileType == ".mpg" || file->FileType == ".bs" || file->FileType == ".es")
	{
		msdkDecoder->pipeline.SetCodecID(MFX_CODEC_MPEG2);
	}
	else if (file->FileType == ".265" || file->FileType == ".h265" || file->FileType == ".hevc")
	{
		msdkDecoder->pipeline.SetCodecID(MFX_CODEC_HEVC);
	}
	else
	{
		DebugMessage(L"Invalid type of file selected");
		msdkDecoder = nullptr; 
	}
	
	msdkDecoder->pipeline.SetFileSource(file);
  
	return msdkDecoder; 
}

MediaStreamSource ^ MSDKDecodeInterop::MSDKInterop::GetMediaStreamSource()
{
	
	if (FAILED(pipeline.OnStart()))
	{
		mss = nullptr;
	}


	videoStreamDescriptor = pipeline.GetVideoStreamDiscriptor();
	if (videoStreamDescriptor == nullptr)
	{
		mss = nullptr;

	}
	mss = ref new MediaStreamSource(videoStreamDescriptor);
	if (mss)
	{
		//mss->Duration = mediaDuration;
		mss->CanSeek = false;
		mss->BufferTime = { 0 };


		startingRequestedToken = mss->Starting += ref new TypedEventHandler<MediaStreamSource ^, MediaStreamSourceStartingEventArgs ^>(this, &MSDKInterop::OnStarting);
		sampleRequestedToken = mss->SampleRequested += ref new TypedEventHandler<MediaStreamSource ^, MediaStreamSourceSampleRequestedEventArgs ^>(this, &MSDKInterop::OnSampleRequested);
	}
	else
	{
		mss = nullptr;
	}
	
	return mss; 
}

MSDKDecodeInterop::MSDKInterop::~MSDKInterop()
{

}

MSDKDecodeInterop::MSDKInterop::MSDKInterop()
{
	
}

void MSDKDecodeInterop::MSDKInterop::OnStarting(MediaStreamSource ^ sender, MediaStreamSourceStartingEventArgs ^ args)
{
	TimeSpan startPos = { 0 };
	args->Request->SetActualStartPosition(startPos);

}

void MSDKDecodeInterop::MSDKInterop::OnSampleRequested(MediaStreamSource ^ sender, MediaStreamSourceSampleRequestedEventArgs ^ args)
{
	mutexGuard.lock();

 
	if (mss != nullptr)
	{
		if (args->Request->StreamDescriptor == videoStreamDescriptor)
		{
			args->Request->Sample = GetNextSample();

		}
		else
		{
			args->Request->Sample = nullptr;
		}
	}
	mutexGuard.unlock();
}

MediaStreamSample ^ MSDKDecodeInterop::MSDKInterop::GetNextSample()
{
	MediaStreamSample^ sample;
	CMfxFrameSurfaceExt* pSurf; 
	LONGLONG ulTimeSpan = 0;
	LONGLONG pts = 0; //crude pts
	DataWriter^ dataWriter = ref new DataWriter();
	
	pSurf = pipeline.GetNextFrame();
		if (pSurf)
		{
			mfxStatus sts = pSurf->pAlloc->Lock(pSurf->pAlloc->pthis, pSurf->Data.MemId, &(pSurf->Data));
			if (sts == MFX_ERR_NONE)
			{
				auto YBuffer = ref new Platform::Array<uint8_t>(pSurf->Data.Y, pSurf->Data.Pitch * pSurf->Info.Height);
				auto UVBuffer = ref new Platform::Array<uint8_t>(pSurf->Data.UV, pSurf->Data.Pitch * pSurf->Info.Height / 2);
				dataWriter->WriteBytes(YBuffer);
				dataWriter->WriteBytes(UVBuffer);
				IBuffer^ buf = dataWriter->DetachBuffer();
				UINT32 ui32Numerator = pSurf->Info.FrameRateExtN;
				UINT32 ui32Denominator = pSurf->Info.FrameRateExtD;
				if (ui32Numerator != 0)
				{
					ulTimeSpan = ((ULONGLONG)ui32Denominator) * 10000000 / ui32Numerator;
				}

				pts = pts + pSurf->Data.FrameOrder;
				sample = MediaStreamSample::CreateFromBuffer(buf, { pts });
				sample->Duration = { ulTimeSpan };
				sample->Discontinuous = false;

				sts = pSurf->pAlloc->Unlock(pSurf->pAlloc->pthis, pSurf->Data.MemId, &(pSurf->Data));
				pSurf->UserLock = false;
			}
		}
		else
		{
			 
			sample = nullptr;  
	
		}
		return sample;
	 
	
}
