using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace facedetect_test
{
    public partial class Form : System.Windows.Forms.Form
    {
        [DllImport(@"facedetect-v1-x64.dll")]
        extern static string facedetect_file(string src);

        [DllImport(@"facedetect-v1-x64.dll")]
        extern static string facedetect(byte[] bytes, int width, int height, int channels);
        public Form()
        {
            InitializeComponent();
            //图片文件检测
            string result = facedetect_file("test.jpeg");
            label1.Text = "图片检测结果："+result;

            //视频帧检测 （仅仅模拟将图片帧转为byte数组）
            Bitmap image = new Bitmap("test.jpeg");
            int stride;
            byte[] source = GetBGRValues(image, out stride);
            int channels = Image.GetPixelFormatSize(image.PixelFormat) / 8;//通道

            string result_cv = facedetect(source, image.Width,image.Height,channels);
            label2.Text = "视频帧检测结果：" + result_cv;
        }
        public static byte[] GetBGRValues(Bitmap bmp, out int stride)
        {
            var rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            var bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);
            stride = bmpData.Stride;
            //int channel = bmpData.Stride / bmp.Width; 
            var rowBytes = bmpData.Width * Image.GetPixelFormatSize(bmp.PixelFormat) / 8;
            var imgBytes = bmp.Height * rowBytes;
            byte[] rgbValues = new byte[imgBytes];
            IntPtr ptr = bmpData.Scan0;
            for (var i = 0; i < bmp.Height; i++)
            {
                Marshal.Copy(ptr, rgbValues, i * rowBytes, rowBytes);   //对齐
                ptr += bmpData.Stride; // next row
            }
            bmp.UnlockBits(bmpData);

            return rgbValues;
        }
    }
}
