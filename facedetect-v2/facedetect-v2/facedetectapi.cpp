#include "facedetectapi.h"
#include "cJSON.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"

#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;

LIB_API char * facedetect(char * imagesrc){
	cJSON  *result = cJSON_CreateObject(), *faces = cJSON_CreateArray();
	char *resultJson;
	try {
		Mat image = imread(imagesrc);
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
		pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);

		cJSON_AddNumberToObject(result, "status", 1);
		cJSON_AddStringToObject(result, "msg", "detect ok");
		cJSON_AddItemToObject(result, "faces", faces);

		for (int i = 0; i < (pResults ? *pResults : 0); i++) {
			short * p = ((short*)(pResults + 1)) + 142 * i;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int confidence = p[4];
			int angle = p[5];

			cJSON  *bbox;
			cJSON_AddItemToArray(faces, bbox = cJSON_CreateObject());
			cJSON_AddNumberToObject(bbox, "score", confidence);
			cJSON_AddNumberToObject(bbox, "x", x);
			cJSON_AddNumberToObject(bbox, "y", y);
			cJSON_AddNumberToObject(bbox, "width", w);
			cJSON_AddNumberToObject(bbox, "height", h);
		}
		resultJson = cJSON_PrintUnformatted(result);
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
