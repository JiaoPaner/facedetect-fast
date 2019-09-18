/*
created by jiaopan
*/
#include "wisefaceapi.h"
#include "cJSON.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetect-dll.h"

#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;

LIB_API char * facedetect(char * imagesrc){	
	cJSON  *result = cJSON_CreateObject(),*faces = cJSON_CreateArray();
	char *resultJson;
	try{
		Mat image = imread(imagesrc);
		if (image.empty()) {
			cJSON_AddNumberToObject(result, "status", -1);
			cJSON_AddStringToObject(result, "msg", "Can not load the image file");
			cJSON_AddItemToObject(result, "faces", faces);
			resultJson = cJSON_PrintUnformatted(result);
			return resultJson;
		}
		Mat gray;
		cvtColor(image, gray, CV_BGR2GRAY);
		int * pResults = NULL;
		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		int doLandmark = 1;
		pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 2, 48, 0, doLandmark);
		Mat result_frontal_surveillance = image.clone();;
		
		cJSON_AddNumberToObject(result, "status", 1);
		cJSON_AddStringToObject(result, "msg", "detect ok");
		cJSON_AddItemToObject(result, "faces", faces);

		for (int i = 0; i < (pResults ? *pResults : 0); i++){
			short * p = ((short*)(pResults + 1)) + 142 * i;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int neighbors = p[4];
			int angle = p[5];

			cJSON  *bbox;
			cJSON_AddItemToArray(faces, bbox = cJSON_CreateObject());
			cJSON_AddNumberToObject(bbox, "score", 99);
			cJSON_AddNumberToObject(bbox, "x", x);
			cJSON_AddNumberToObject(bbox, "y", y);
			cJSON_AddNumberToObject(bbox, "width", w);
			cJSON_AddNumberToObject(bbox, "height", h);

			rectangle(result_frontal_surveillance, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
			if (doLandmark)
			{
				for (int j = 0; j < 68; j++)
					circle(result_frontal_surveillance, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
			}
		}
		resultJson = cJSON_PrintUnformatted(result);
		//imshow("Results_frontal_surveillance", result_frontal_surveillance);
		//printf(resultJson);
		//waitKey();
		return resultJson;
	}
	catch (const std::exception&){
		cJSON_AddNumberToObject(result, "status", -1);
		cJSON_AddStringToObject(result, "msg", "detect failed");
		cJSON_AddItemToObject(result, "faces", faces);
		resultJson = cJSON_PrintUnformatted(result);
		return resultJson;
	}
}
