﻿#include "pch.h"
#include "MSDKDecodeInterop.h"


using namespace MSDKDecodeInterop; 

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
	if (FAILED(msdkDecoder->pipeline.OnStart()))
	{
		msdkDecoder = nullptr; 
	}

	return msdkDecoder; 
}

MediaStreamSource ^ MSDKDecodeInterop::MSDKInterop::GetMediaStreamSource()
{
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

}

void MSDKDecodeInterop::MSDKInterop::OnSampleRequested(MediaStreamSource ^ sender, MediaStreamSourceSampleRequestedEventArgs ^ args)
{

}