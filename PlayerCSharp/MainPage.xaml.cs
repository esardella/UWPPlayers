using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
using Windows.Storage;
using Windows.Media.Core;
using MSDKDecodeInterop;
 
namespace PlayerCSharp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void OpenLocalFileAsync(object sender, RoutedEventArgs e)
        {
            var picker = new Windows.Storage.Pickers.FileOpenPicker();
            picker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.ComputerFolder;
            picker.FileTypeFilter.Add(".h264");
            picker.FileTypeFilter.Add(".264");
            picker.FileTypeFilter.Add(".264");
            picker.FileTypeFilter.Add(".m2v");
            picker.FileTypeFilter.Add(".mpg");
            picker.FileTypeFilter.Add(".bs");
            picker.FileTypeFilter.Add(".es");
            // No support for HEVC in UWP version of library so far because plugins are unsupported
            picker.FileTypeFilter.Add(".h265");
            picker.FileTypeFilter.Add(".265");
            picker.FileTypeFilter.Add(".hevc");

            StorageFile file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                mediaElement.Stop();
                try
                {

                    var msdkMSS = MSDKDecodeInterop.MSDKInterop.CreatefromFile(file);
                    if (msdkMSS != null)
                    {

                        MediaStreamSource mss = msdkMSS.GetMediaStreamSource();
                        if (mss != null)
                        {
                            mediaElement.SetMediaStreamSource(mss);
                            Splitter.IsPaneOpen = false;

                        }
                        else
                        {
                            // DisplayErrorMessage("Cannot Open Media");
                        }
                    }
                    else
                    {
                        // DisplayErrorMessage("Cannot open media");

                    }
                }
                catch (Exception ex)
                {
                    // DisplayErrorMessage(ex->Message);
                }
            }
            else
            {

            }

        }

        private void MediaFailed(object sender, ExceptionRoutedEventArgs e)
        {

        }

    }    
}
