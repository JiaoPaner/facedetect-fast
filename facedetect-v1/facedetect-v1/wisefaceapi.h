#pragma once
#define LIB_API __declspec(dllexport) //��������
extern "C" {
	LIB_API char*   facedetect(char* imagesrc);
}