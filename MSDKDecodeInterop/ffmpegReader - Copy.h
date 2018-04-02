#pragma once

#include "pch.h"
#include "mfxvideo.h"
#include "SimpleBitstreamReader.h"
#include <atomic>
#include <pplawait.h>


extern "C"
{
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavcodec/avcodec.h>
}

class ffmpegReader   
{

public:
	ffmpegReader();
	virtual ~ffmpegReader();

	//resets position to file begin
	virtual void      Reset();
	virtual void      Close();
	virtual mfxStatus Init(Windows::Storage::StorageFile^ fileSource);
	virtual mfxStatus ReadNextFrame();
	mfxBitstream BitStream;
//	mfxStatus ExpandBitstream(unsigned int extraSize);


protected:
	Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream;
	Windows::Storage::Streams::DataReader^ dataReader;
	std::atomic<bool>      m_bInited;
	std::atomic<mfxU64> fileSize;
	std::atomic<mfxU64> bytesCompleted;
	Windows::Storage::StorageFile^ fs;
	IStream* fileStreamData;
	unsigned char* fileStreamBuffer;
	int audioStreamIndex;
	int videoStreamIndex;

	LONGLONG framePts;
	LONGLONG frameDuration;
	LONGLONG frameDts;
	bool isRegistered;
private:

	AVDictionary* avDict;
	AVIOContext* avIOCtx;
	AVFormatContext* avFormatCtx;
	AVCodecContext* avAudioCodecCtx;
	AVCodecContext* avVideoCodecCtx;
	const AVBitStreamFilter *bsf;
	AVBSFContext *ctx;
};