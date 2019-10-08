
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"
#include "facedetectapi.h"

//define the buffer size. Do not change the size!
//#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;
//extern char* detect(Mat src);
int main(int argc, char* argv[])
{
	/*
	if (argc != 2)
	{
	printf("Usage: %s <image_file_name>\n", argv[0]);
	return -1;
	}


	//load an image and convert it to gray (single-channel)
	//Mat image = imread(argv[1]);
	Mat image = imread("test.jpg");
	if (image.empty())
	{
	fprintf(stderr, "Can not load the image file %s.\n", argv[1]);
	return -1;
	}

	int * pResults = NULL;
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if (!pBuffer)
	{
	fprintf(stderr, "Can not alloc buffer.\n");
	return -1;
	}


	///////////////////////////////////////////
	// CNN face detection
	// Best detection rate
	//////////////////////////////////////////
	//!!! The input image must be a BGR one (three-channel) instead of RGB
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);

	printf("%d faces detected.\n", (pResults ? *pResults : 0));
	Mat result_cnn = image.clone();
	//print the detection results
	for (int i = 0; i < (pResults ? *pResults : 0); i++)
	{
	short * p = ((short*)(pResults + 1)) + 142 * i;
	int x = p[0];
	int y = p[1];
	int w = p[2];
	int h = p[3];
	int confidence = p[4];
	int angle = p[5];

	printf("face_rect=[%d, %d, %d, %d], confidence=%d, angle=%d\n", x, y, w, h, confidence, angle);
	rectangle(result_cnn, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
	}
	imshow("result_cnn", result_cnn);

	waitKey();

	//release the buffer
	free(pBuffer);
	*/

	/*----------------------------------------------------------------
	VideoCapture capture;
	capture.open(0);
	//if (!capture.isOpened())
	//{
	//	cout << "can't open video" << endl;
	//	return -1;
	//}
	Mat frame;
	int * pResults = NULL;
	while (true)
	{

		//pBuffer is used in the detection functions.
		//If you call functions in multiple threads, please create one buffer for each thread!
		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if (!pBuffer)
		{
			fprintf(stderr, "Can not alloc buffer.\n");
			break;
		}
		capture >> frame;
		pResults = facedetect_cnn(pBuffer, (unsigned char*)(frame.ptr(0)), frame.cols, frame.rows, (int)frame.step);

		printf("%d faces detected.\n", (pResults ? *pResults : 0));
		Mat result_cnn = frame.clone();
		//print the detection results
		for (int i = 0; i < (pResults ? *pResults : 0); i++)
		{
			short * p = ((short*)(pResults + 1)) + 142 * i;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int confidence = p[4];
			int angle = p[5];

			printf("face_rect=[%d, %d, %d, %d], confidence=%d, angle=%d\n", x, y, w, h, confidence, angle);
			rectangle(result_cnn, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		}
		imshow("result_cnn", result_cnn);
		free(pBuffer);
		char c = waitKey(30);
		if (c == 27)//esc¼ü
		{
			break;
		}
	}
	*/

	/**/
	VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened()){
		cout << "can't open video" << endl;
		return -1;
	}
	Mat frame;
	while (true){
		capture >> frame;
		int size = frame.total() * frame.elemSize();
		unsigned char* bytes = new unsigned char[size];
		std::memcpy(bytes, frame.data, size * sizeof(unsigned char));
		char* result = facedetect(bytes, frame.cols, frame.rows, frame.channels());
		printf(result);
		free(result);
		free(bytes);
		//imwrite("detect.jpg", frame);
		//char* result = detect(frame);
		//printf(result);
		//imshow("result_cnn", frame);
		char c = waitKey(30);
		if (c == 27){
			break;
		}
	}
	
	//std::cout << facedetect_file(argv[1]);
	waitKey(0);
	return 0;
}

