﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "MainPage.xaml.h"

void ::Player::MainPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::Player::MainPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 1:
        {
            this->Splitter = safe_cast<::Windows::UI::Xaml::Controls::SplitView^>(__target);
        }
        break;
    case 2:
        {
            this->Hamburger = safe_cast<::Windows::UI::Xaml::Controls::FontIcon^>(__target);
        }
        break;
    case 3:
        {
            this->Header = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 4:
        {
            ::Windows::UI::Xaml::Controls::AppBarButton^ element4 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element4))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::Player::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::OpenLocalFile);
        }
        break;
    case 5:
        {
            this->mediaElement = safe_cast<::Windows::UI::Xaml::Controls::MediaElement^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::MediaElement^>(this->mediaElement))->MediaFailed += ref new ::Windows::UI::Xaml::ExceptionRoutedEventHandler(this, (void (::Player::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::ExceptionRoutedEventArgs^))&MainPage::MediaFailed);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::Player::MainPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)

