#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Pixels {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
public: 
	Pixels();
	Pixels(unsigned char blue, unsigned char green, unsigned char red);
	unsigned char GetBlue() {
		return blue;
	}
	unsigned char GetGreen() {
		return green;
	}
	unsigned char GetRed() {
		return red;
	}
};
class Image
{
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
	vector<Pixels> pixels;
public: 
	Image();
	Image(char id, char mapType, char typeCode, short mapOrigin, short mapLength, char mapDepth, short x, short y, short imageWidth, short imageHeight, char bitsPixs, char imageDesc);
	char getIdLength() {
		return idLength;
	}
	void setIdLength(char idlength) {
		idLength = idlength;
	}
	char GetColorMapType() {
		return colorMapType;
	}
	void setColorMapType(char mapType) {
		colorMapType = mapType;
	}
	char GetDataType() {
		return dataTypeCode;
	}
	void setDataType(char typeCode) {
		dataTypeCode = typeCode;
	}
	short GetColorMapOrigin() {
		return colorMapOrigin;
	}
	void setColorMapOrigin(short mapOrigin) {
		colorMapOrigin = mapOrigin;
	}
	short GetColorMapLength() {
		return colorMapLength;
	}
	void setColorMapLength(short mapLength) {
		colorMapLength = mapLength;
	}
	char GetColorMapDepth() {
		return colorMapDepth;
	}
	void setColorMapDepth(char mapDepth) {
		colorMapDepth = mapDepth;
	}
	short GetxOrigin() {
		return xOrigin;
	}
	void setxOrigin(short x) {
		xOrigin = x;
	}
	short GetyOrigin() {
		return yOrigin;
	}
	void setyOrigin(short y) {
		yOrigin = y;
	}
	short GetWidth() {
		return width;
	}
	void setWidth(short imageWidth) {
		width = imageWidth;
	}
	short GetHeight() {
		return height;
	}
	void setHeight(short imageHeight) {
		height = imageHeight;
	}
	char GetBitsPerPixels() {
		return bitsPerPixel;
	}
	void setBitsPerPixel(char bitsPixs) {
		bitsPerPixel = bitsPixs;
	}
	char GetImageDescriptor() {
		return imageDescriptor;
	}
	void setImageDescriptor(char imageDesc) {
		imageDescriptor = imageDesc;
	}
	unsigned int GetNumPixels() {
		return (width * height);
	}
	void addPixel(Pixels& pixel);
	vector<Pixels> GetPixels() {
		return pixels;
	}
};
