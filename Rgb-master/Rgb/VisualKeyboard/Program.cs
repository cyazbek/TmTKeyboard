using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HidLibrary;
using RgbHidLibrary;

namespace VisualKeyboard
{
    class Program
    {
        static void Main(string[] args)
        {
            RgbHidBase rgbDevice;
            RgbHid.TryCreate(RgbDeviceType.CorsairK70Rgb, out rgbDevice);
            K70Rgb myKeyboard = rgbDevice as K70Rgb;


            IDictionary<uint, Color> x = new Dictionary<uint, Color>();
            x.Add(7, Color.Red);
            myKeyboard.DrawKeys(x);
        }
    }
}
