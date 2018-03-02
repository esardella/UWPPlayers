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

#include "pch.h"
#include "SimpleBitstreamReader.h"
#include "sample_defs.h"
#include <pplawait.h>

//using namespace MSDKDecodeInterop;
using namespace concurrency; 
using namespace Platform;
using namespace Windows::Storage; 
using namespace Windows::Storage::Streams; 
 
 


#define INITIAL_BITSTREAM_SIZE 10000

//template <typename T>
//auto ExecSync(Windows::Foundation::IAsyncOperation<T^>^ asyncFunc)
//{
//    auto task = concurrency::create_task(asyncFunc);
//    task.wait();
//    return task.get();
//}

CSimpleBitstreamReader::CSimpleBitstreamReader()
{
    m_bInited = false;
    fileSize = 0;
    bytesCompleted = 0;
}

CSimpleBitstreamReader::~CSimpleBitstreamReader()
{
    Close();
}

void CSimpleBitstreamReader::Close()
{
    stream = nullptr;
    dataReader = nullptr;

    m_bInited = false;
    fileSize = 0;
    bytesCompleted = 0;
    delete[] BitStream.Data;
    MSDK_ZERO_MEMORY(BitStream);
}

void CSimpleBitstreamReader::Reset()
{
    if (!m_bInited)
        return;

    stream->Seek(0);

    bytesCompleted = 0;
    
}

task<void>CSimpleBitstreamReader::LoadSource(Windows::Storage::StorageFile^ fileSource)
{

 

 

	auto stream = co_await fileSource->OpenReadAsync();
	 
    dataReader = ref new Windows::Storage::Streams::DataReader(stream);
	auto filesize = stream->Size;
	//auto nBytesRead = co_await dataReader->LoadAsync(stream->Size);


}

mfxStatus CSimpleBitstreamReader::Init(Windows::Storage::StorageFile^ fileSource)
{
    if (fileSource && !fileSource->IsAvailable)
    {
        return MFX_ERR_NULL_PTR;
    }

    Close();
	//concurrency::task<void> task = LoadSource(fileSource);
	
 
 

    //Getting File Size
   // auto task = concurrency::create_task(temp->GetBasicPropertiesAsync());
   // task.wait();
   // fileSize = task.get()->Size;
    //fileSize = ExecSync(file->GetBasicPropertiesAsync())->Size;

    //open file to read input stream
	//auto task = create_task(fileSource->OpenReadAsync());

	auto task = create_task(fileSource->OpenReadAsync()); 
	task.then([this,task](IRandomAccessStreamWithContentType^ stream)
	{
		task.wait();
		 stream = task.get();
		 dataReader = ref new Windows::Storage::Streams::DataReader(stream);
		
	}, Concurrency::task_continuation_context::use_arbitrary());
	

	
//	task.wait();

//	Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream = task.get();
//	fileSize = stream->Size;
//	m_bInited = true;

//	dataReader = ref new Windows::Storage::Streams::DataReader(stream);
	m_bInited = true;




  //  dataReader = ref new Windows::Storage::Streams::DataReader(stream);
	
    // Initializing bitstream
    MSDK_ZERO_MEMORY(BitStream);
    BitStream.Data = new(std::nothrow) unsigned char[INITIAL_BITSTREAM_SIZE];
    MSDK_CHECK_POINTER_SAFE(BitStream.Data, MFX_ERR_NULL_PTR, Close());
    BitStream.MaxLength = INITIAL_BITSTREAM_SIZE;

    return MFX_ERR_NONE;
}

mfxStatus CSimpleBitstreamReader::ReadNextFrame()
{
    if (!m_bInited)
        return MFX_ERR_NOT_INITIALIZED;

    try
    {
        mfxU32 nBytesRead = 0;

        bytesCompleted += BitStream.DataOffset;
        memmove(BitStream.Data, BitStream.Data + BitStream.DataOffset, BitStream.DataLength);
        BitStream.DataOffset = 0;

        auto task = concurrency::create_task(dataReader->LoadAsync(BitStream.MaxLength - BitStream.DataLength));
        task.wait();
        nBytesRead = task.get();

        //nBytesRead = (mfxU32)fread(BitStream.Data + BitStream.DataLength, 1, BitStream.MaxLength - BitStream.DataLength, m_fSource);

        if (0 == nBytesRead)
        {
            return MFX_ERR_MORE_DATA;
        }


        Platform::ArrayReference<unsigned char> arrayWrapper((unsigned char*)BitStream.Data + BitStream.DataLength, nBytesRead);
        dataReader->ReadBytes(arrayWrapper);

        BitStream.DataLength += nBytesRead;
    }
    catch (Platform::Exception^ e)
    {
//        ShowErrorAndExit(e->Message);
    }

    return MFX_ERR_NONE;
}

mfxStatus CSimpleBitstreamReader::ExpandBitstream(unsigned int extraSize)
{
    unsigned char* oldPtr = BitStream.Data;
    BitStream.Data = new(std::nothrow) unsigned char[BitStream.MaxLength +extraSize];
    MSDK_CHECK_POINTER_SAFE(BitStream.Data, MFX_ERR_NULL_PTR, Close());
    MSDK_MEMCPY(BitStream.Data, oldPtr, BitStream.MaxLength);
    BitStream.MaxLength += extraSize;
    return MFX_ERR_NONE;
}
