#pragma once
#include <queue>
#include <mutex>
#include "DecodingPipeline.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Core;
using namespace Windows::Storage::Streams;

namespace MSDKDecodeInterop
{
	public ref class MSDKInterop sealed {

	public:
		static MSDKInterop^ CreatefromFile(Windows::Storage::StorageFile^ file);
		MediaStreamSource^ GetMediaStreamSource(); 
		virtual ~MSDKInterop();
	
		property VideoStreamDescriptor^ VideoDescriptor
		{
			VideoStreamDescriptor^ get()
			{
				return  videoStreamDescriptor;
			};
		};
		property TimeSpan Duration
		{
			TimeSpan get()
			{
				return mediaDuration;
			};
		};
		property String^ VideoCodecName
		{
			String^ get()
			{
				return videoCodecName;
			};
		};
	private: 
		MSDKInterop(); 
		void OnStarting(MediaStreamSource ^sender, MediaStreamSourceStartingEventArgs ^args);
		void OnSampleRequested(MediaStreamSource ^sender, MediaStreamSourceSampleRequestedEventArgs ^args);

		CDecodingPipeline pipeline;
		MediaStreamSource ^ mss;
		EventRegistrationToken startingRequestedToken;
		EventRegistrationToken sampleRequestedToken;
		VideoStreamDescriptor ^ videoStreamDescriptor;
		TimeSpan mediaDuration;
		String^ videoCodecName;
 
	};


}