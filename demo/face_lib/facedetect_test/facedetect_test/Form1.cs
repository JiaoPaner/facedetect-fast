using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace facedetect_test
{
    public partial class Form : System.Windows.Forms.Form
    {
        [DllImport(@"facedetect-v1-x64.dll")]
        static extern string facedetect_file(string src);

        [DllImport(@"facedetect-v1-x64.dll")]
        static extern string facedetect(byte[] bytes, int width, int height, int channels);
        public Form()
        {
            InitializeComponent();
        }
        public static byte[] GetBGRValues(Bitmap bmp, out int stride)
        {
            var rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            var bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);
            stride = bmpData.Stride;
            //int channel = bmpData.Stride / bmp.Width; 
            var rowBytes = bmpData.Width * Image.GetPixelFormatSize(bmp.PixelFormat) / 8;
            var imgBytes = bmp.Height * rowBytes;
            var rgbValues = new byte[imgBytes];
            var ptr = bmpData.Scan0;
            for (var i = 0; i < bmp.Height; i++)
            {
                Marshal.Copy(ptr, rgbValues, i * rowBytes, rowBytes);   //对齐
                ptr += bmpData.Stride; // next row
            }
            bmp.UnlockBits(bmpData);

            return rgbValues;
        }

        private FaceDetectionResult DetectFacesInImageFile(string file)
        {
            var res = facedetect_file(file);
            return JsonConvert.DeserializeObject<FaceDetectionResult>(res);
        }

        private FaceDetectionResult DetectFacesInBitmap(Bitmap image)
        {
            int stride;
            var source = GetBGRValues(image, out stride);
            var channels = Image.GetPixelFormatSize(image.PixelFormat) / 8;//通道

            var res = facedetect(source, image.Width, image.Height, channels);
            return JsonConvert.DeserializeObject<FaceDetectionResult>(res);
        }
        private void btnOpenImage_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog
            {
                Title = @"Open Image File",
                Filter = @"Image files (*.jpg, *.jpeg, *.bmp) | *.jpg; *.jpeg; *.bmp",
            };
            if (dlg.ShowDialog() != DialogResult.OK) return;
            try
            {
                var watch = new Stopwatch();
                watch.Start();
                var res = DetectFacesInImageFile(dlg.FileName);
                watch.Stop();
                lblResult.Text = $@"Found {res.Faces.Count} faces in {watch.Elapsed}";

                pctResult.Image = Image.FromFile(dlg.FileName);

                lvwFaces.Items.Clear();
                var counter = 1;
                foreach (var faceInfo in res.Faces)
                {
                    var row = new string[] { $@"ID-{counter}", $"{faceInfo.X} - {faceInfo.Width}", $"{faceInfo.Y} - {faceInfo.Height}", $"{faceInfo.Score}" };
                    var listViewItem = new ListViewItem(row);
                    lvwFaces.Items.Add(listViewItem);
                    counter++;
                }
                ReportResults(new Bitmap(dlg.FileName), res);

            }
            catch (Exception ex)
            {
                MessageBox.Show($@"Some error occurred: {ex.Message}");
            }
        }

        private void ReportResults(Bitmap bmp, FaceDetectionResult faceDetectionResult)
        {
            using (var G = Graphics.FromImage(bmp))
            {
                var counter = 1;
                foreach (var f in faceDetectionResult.Faces)
                {
                    G.DrawString($@"ID-{counter}", new Font("Tahoma", 12,FontStyle.Bold), Brushes.Red, (float)f.X, (float)f.Y);
                    G.DrawRectangle(new Pen(Color.Yellow, 2.5f), new Rectangle(f.X, f.Y, f.Width, f.Height));
                    counter++;
                }
            }

            pctResult.Image = bmp;
        }
    }
}
