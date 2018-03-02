//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <experimental\resumable>

#include <pplawait.h>
using namespace concurrency;

namespace Player
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		void MediaFailed(Platform::Object ^ sender, Windows::UI::Xaml::ExceptionRoutedEventArgs ^ e);
		void DisplayErrorMessage(Platform::String ^ message);

	private:
		void  OpenLocalFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		task<void> LoadSource();

		MSDKDecodeInterop::MSDKInterop^ msdkMSS;
	};
}
