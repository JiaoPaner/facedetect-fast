using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace facedetect_test
{
    class FaceDetectionResult
    {
        public int Status { get; set; }
        public string Msg { get; set; }
        public List<FaceInfo> Faces { get; set; }
    }
}
