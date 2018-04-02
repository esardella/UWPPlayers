/******************************************************************************\
Copyright (c) 2005-2017, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This sample was distributed or derived from the Intel's Media Samples package.
The original version of this sample may be obtained from https://software.intel.com/en-us/intel-media-server-studio
or https://software.intel.com/en-us/media-client-solutions-support.
\**********************************************************************************/

#pragma once
#include "mfxvideo++.h"
#include "Thread11.h"
#include "SimpleBitstreamReader.h"
#include "ffmpegReader.h"
#include <atomic>
#include <list>
#include "d3d11_device.h"
#include "sysmem_allocator.h"
#include "SurfacesPool.h"
#include "Thread11.h"
#include "sample_defs.h"

#include "PluginsManager.h"

using namespace Windows::Media::Core;

namespace MSDKDecodeInterop
{
	
	class CDecodingPipeline  
	{
	public:

		CDecodingPipeline();

		bool Init();
	
		bool m_DecodeInited; 
		bool OnStart(); 
		void LaunchDecoderInit();
		bool InitDecoder();
		CMfxFrameSurfaceExt* GetNextFrame();

		VideoStreamDescriptor^ videoStreamDiscriptor;
		VideoStreamDescriptor^ GetVideoStreamDiscriptor(); 

		void SetCodecID(mfxU32 codec) { codecID = codec; }
		bool SetFileSource(Windows::Storage::StorageFile^ fs);
		bool SetURISource(Platform::String^ uri);
		

		bool IsHWLib;
		mfxU32 AsyncDepth;
		mfxHDL GetHWDevHdl()
		{
			mfxHDL hdl = NULL;
			return dev.GetHandle(MFX_HANDLE_D3D11_DEVICE, &hdl) >= MFX_ERR_NONE ? hdl : NULL;
		}
	
		int GetProgressPromilleage() { return  reader->GetFileSize() ? (int)(reader->GetBytesProcessed() * 1000 / reader->GetFileSize()) : 0; }
		std::list<CMfxFrameSurfaceExt*> outputSurfaces;
	protected:

		bool RunOnce();
		void OnClose();

	private:
		 
		mfxStatus InitSession();
		mfxStatus SyncOneSurface();
		void EnqueueSurface(CMfxFrameSurfaceExt* surface);
	

		bool LoadVideoStreamDiscriptor();
		MFXVideoSession session;
		mfxIMPL impl;

		std::auto_ptr<CSimpleBitstreamReader> reader; 
		 
		mfxVideoParam decoderParams;
		MFXVideoDECODE* pDecoder = NULL;
		mfxU32 codecID;

		CD3D11Device dev;

		SysMemFrameAllocator allocator; 
		CSurfacesPool surfacesPool;
		std::list<CMfxFrameSurfaceExt*> decodingSurfaces;

		Windows::Storage::StorageFile^ fileSource;
		Platform::String^ uri;
		bool isStreaming; 
		bool isDecodingEnding;
		CPluginsManager pluginsManager;
		mfxStatus CDecodingPipeline::LoadPluginsAndDecodeHeader(mfxBitstream* pBS, mfxU32 codecID);
	};

}