
#include "Images.h"

Image::Image()
{
	idLength = ' ';
	colorMapType = ' ';
	dataTypeCode = ' ';
	colorMapOrigin = 0;
	colorMapLength = 0;
	colorMapDepth = ' ';
	xOrigin = 0;
	yOrigin = 0;
	width = 0;
	height = 0;
	bitsPerPixel = ' ';
	imageDescriptor = ' ';
}

Image::Image(char id, char mapType, char typeCode, short mapOrigin, short mapLength, char mapDepth, short x, short y, short imageWidth, short imageHeight, char bitsPixs, char imageDesc)
{
	idLength = id;
	colorMapType = mapType;
	dataTypeCode = typeCode;
	colorMapOrigin = mapOrigin;
	colorMapLength = mapLength;
	colorMapDepth = mapDepth;
	xOrigin = x;
	yOrigin = y;
	width = imageWidth;
	height = imageHeight;
	bitsPerPixel = bitsPixs;
	imageDescriptor = imageDesc;
}

void Image::addPixel(Pixels& pixel)
{
	pixels.push_back(pixel);
}

Pixels::Pixels()
{
	blue = ' ';
	green = ' ';
	red = ' ';
}

Pixels::Pixels(unsigned char blue, unsigned char green, unsigned char red)
{
	this->blue = blue;
	this->green = green;
	this->red = red;
}
