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

void Player::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	TimeSpan ts;
	ts.Duration = 500;
	timer->Interval = ts;
	timer->Tick += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &Player::MainPage::OnTick);
	timer->Start();

}
void Player::MainPage::OpenLocalFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
 


 

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

void Player::MainPage::OnTick(Platform::Object ^sender, Platform::Object ^args)
{


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