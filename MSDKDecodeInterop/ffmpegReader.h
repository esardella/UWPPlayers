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

class ffmpegReader  : public CSimpleBitstreamReader
{

public:
	ffmpegReader();
	virtual ~ffmpegReader();

	//resets position to file begin
	virtual void      Reset();
	virtual void      Close();
	virtual mfxStatus Init(Windows::Storage::StorageFile^ fileSource);
    virtual mfxStatus InitURI(Platform::String^ uri);
	mfxStatus CompleteFFMPEGInit();
	virtual mfxStatus ReadNextFrame();
 
	
protected:
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