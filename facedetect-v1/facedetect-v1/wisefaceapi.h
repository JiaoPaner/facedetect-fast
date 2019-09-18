#pragma once
#define LIB_API __declspec(dllexport) //µ¼³öº¯Êý
extern "C" {
	LIB_API char*   facedetect(char* imagesrc);
}