#pragma once
#pragma once
#define LIB_API __declspec(dllexport) 

extern "C" {
	LIB_API char*   facedetect_file(char* src);
	LIB_API char*   facedetect(unsigned char* src, int width, int height, int channels);
}