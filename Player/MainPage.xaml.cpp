//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Player;
using namespace MSDKDecodeInterop;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Core;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}



void Player::MainPage::OpenLocalFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//LoadSource(); 





	Windows::Storage::Pickers::FileOpenPicker^ picker = ref new Windows::Storage::Pickers::FileOpenPicker();
	picker->ViewMode = Windows::Storage::Pickers::PickerViewMode::Thumbnail;
	picker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::ComputerFolder;
	picker->FileTypeFilter->Append(".h264");
	picker->FileTypeFilter->Append(".264");
	picker->FileTypeFilter->Append(".264");
	picker->FileTypeFilter->Append(".m2v");
	picker->FileTypeFilter->Append(".mpg");
	picker->FileTypeFilter->Append(".bs");
	picker->FileTypeFilter->Append(".es");
	// No support for HEVC in UWP version of library so far because plugins are unsupported
	picker->FileTypeFilter->Append(".h265");
	picker->FileTypeFilter->Append(".265");
	picker->FileTypeFilter->Append(".hevc");

	create_task(picker->PickSingleFileAsync()).then([this](Windows::Storage::StorageFile^ file)
	{
		if (file)
		{
			
		/*
			
			create_task(file->OpenAsync(FileAccessMode::Read)).then([this, file](task<IRandomAccessStream^> stream)
			{

				IRandomAccessStream^ readStream = stream.get();
				auto dataReader = ref new Windows::Storage::Streams::DataReader(readStream);
				auto fileSize = readStream->Size;
				auto task = create_task(dataReader->LoadAsync(10000));
				task.wait();
				auto nBytesRead = task.get();
			});
			*/
			
			/*
			auto task = create_task(file->OpenReadAsync());
			try {
				task.wait();

				Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream = task.get();
				auto fileSize = stream->Size;
				}
			catch (Platform::COMException^ e)
			{
				//Example output: The system cannot find the specified file.
				OutputDebugString(e->Message->Data());
			}
			*/
			
			mediaElement->Stop();
			
				try {

			 
					msdkMSS = MSDKDecodeInterop::MSDKInterop::CreatefromFile(file);
					if (msdkMSS != nullptr)
					{
						MediaStreamSource^ mss = msdkMSS->GetMediaStreamSource();
						if (mss)
						{
							mediaElement->SetMediaStreamSource(mss);
							Splitter->IsPaneOpen = false;
						}
						else
						{
							DisplayErrorMessage("Cannot Open Media");
						}
					}
					else
					{
						DisplayErrorMessage("Cannot open media");

					}
				}
				catch (COMException^ ex)
				{
					DisplayErrorMessage(ex->Message);
				}
		
		
		
	
		}
	});


		

}

task<void> Player::MainPage::LoadSource()
{
 
	auto picker = ref new FileOpenPicker(); 
	picker->FileTypeFilter->Append(L".h264");
	picker->SuggestedStartLocation = PickerLocationId::ComputerFolder;

	auto file = co_await picker->PickSingleFileAsync();
	if (nullptr == file)
		return; 

	auto stream = co_await file->OpenReadAsync();
	auto dataReader = ref new Windows::Storage::Streams::DataReader(stream);
	auto filesize = stream->Size;
	auto nBytesRead = co_await dataReader->LoadAsync(stream->Size);


}

void MainPage::MediaFailed(Platform::Object^ sender, Windows::UI::Xaml::ExceptionRoutedEventArgs^ e)
{
	DisplayErrorMessage(e->ErrorMessage);
}
void MainPage::DisplayErrorMessage(Platform::String^ message)
{
	// Display error message
	auto errorDialog = ref new MessageDialog(message);
	errorDialog->ShowAsync();
}