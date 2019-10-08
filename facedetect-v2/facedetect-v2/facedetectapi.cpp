#include "facedetectapi.h"
#include "cJSON.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"

#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;

char * detect(Mat& image,int speed = 0) {
	cJSON  *result = cJSON_CreateObject(), *faces = cJSON_CreateArray();
	char *resultJson;
	try {
		if (image.empty()) {
			cJSON_AddNumberToObject(result, "status", -1);
			cJSON_AddStringToObject(result, "msg", "Can not load the image file");
			cJSON_AddItemToObject(result, "faces", faces);
			resultJson = cJSON_PrintUnformatted(result);
			return resultJson;
		}
		//Mat gray;
		//cvtColor(image, gray, CV_BGR2GRAY);
		int * pResults = NULL;
		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);

		Size size(160,120);
		int width = int(image.cols / 64) * 64, height = int(image.rows / 64) * 64;
		if (speed == 1) {
			Mat resized;
			resize(image, resized, size);
			pResults = facedetect_cnn(pBuffer, (unsigned char*)(resized.ptr(0)), resized.cols, resized.rows, (int)resized.step);
		}
		else {
			
			pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), width, height, (int)image.step);
		}
		
		cJSON_AddNumberToObject(result, "status", 1);
		cJSON_AddStringToObject(result, "msg", "detect ok");
		cJSON_AddItemToObject(result, "faces", faces);

		for (int i = 0; i < (pResults ? *pResults : 0); i++) {
			short * p = ((short*)(pResults + 1)) + 142 * i;
			int x, y, w, h;
			if (speed == 1) {
				x = p[0] * (image.cols / size.width);
				y = p[1] * (image.rows / size.height);
				w = p[2] * (image.cols / size.width);
				h = p[3] * (image.rows / size.height);
			}
			else {
				x = p[0];
				y = p[1];
				w = p[2];
				h = p[3];
			}
			
			int confidence = p[4];
			int angle = p[5];
			if (confidence < 60)
				continue;
			cJSON  *bbox;
			cJSON_AddItemToArray(faces, bbox = cJSON_CreateObject());
			cJSON_AddNumberToObject(bbox, "score", confidence);
			cJSON_AddNumberToObject(bbox, "x", x );
			cJSON_AddNumberToObject(bbox, "y", y );
			cJSON_AddNumberToObject(bbox, "width", w  );
			cJSON_AddNumberToObject(bbox, "height", h);
			//rectangle(image, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		}
		resultJson = cJSON_PrintUnformatted(result);
		free(pBuffer);
		//imshow("result_cnn", image);
		return resultJson;
	}
	catch (const std::exception&) {
		cJSON_AddNumberToObject(result, "status", -1);
		cJSON_AddStringToObject(result, "msg", "detect failed");
		cJSON_AddItemToObject(result, "faces", faces);
		resultJson = cJSON_PrintUnformatted(result);
		return resultJson;
	}
}

LIB_API char * facedetect_file(char * src) {
	Mat image = imread(src);
	return detect(image);
}

LIB_API char * facedetect(unsigned char * src, int width, int height, int channels) {
	int format;
	switch (channels) {
	case 1:
		format = CV_8UC1;
		break;
	case 2:
		format = CV_8UC2;
		break;
	case 3:
		format = CV_8UC3;
		break;
	default:
		format = CV_8UC4;
		break;
	}
	Mat image(height, width, format, src);
	return detect(image,1);
}

