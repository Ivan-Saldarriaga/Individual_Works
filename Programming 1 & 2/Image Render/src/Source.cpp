#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Images.h"
using namespace std;
Image getImageData(string filePath) {
	ifstream inputFile(filePath, ios_base::binary);
	
		char lengthId;
		inputFile.read((char*)&lengthId, 1);
		char colorMapType;
		inputFile.read((char*)&colorMapType, 1);
		char dataTypeCode;
		inputFile.read((char*)&dataTypeCode, 1);
		short colorMapOrigin;
		inputFile.read((char*)&colorMapOrigin, 2);
		short colorMapLength;
		inputFile.read((char*)&colorMapLength, 2);
		char colorMapDepth;
		inputFile.read((char*)&colorMapDepth, 1);
		short xOrigin;
		inputFile.read((char*)&xOrigin, 2);
		short yOrigin;
		inputFile.read((char*)&yOrigin, 2);
		short width;
		inputFile.read((char*)&width, 2);
		short height;
		inputFile.read((char*)&height, 2);
		char bitsPerPixel;
		inputFile.read((char*)&bitsPerPixel, 1);
		char imageDescriptor;
		inputFile.read((char*)&imageDescriptor, 1);
		Image newPicture(lengthId, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth, xOrigin, yOrigin, width, height, bitsPerPixel, imageDescriptor);
		vector<Pixels> pixels;
		for (unsigned int i = 0; i < newPicture.GetNumPixels(); i++) {
			unsigned char blue;
			inputFile.read((char*)&blue, sizeof(blue));
			unsigned char green;
			inputFile.read((char*)&green, sizeof(green));
			unsigned char red;
			inputFile.read((char*)&red, sizeof(red));
			Pixels newPixel(blue, green, red);
			newPicture.addPixel(newPixel);
		}
		return newPicture;
}
void setHeader(Image& resultingImage, Image& headerDataImage) {
	resultingImage.setIdLength(headerDataImage.getIdLength());
	resultingImage.setColorMapType(headerDataImage.GetColorMapType());
	resultingImage.setDataType(headerDataImage.GetDataType());
	resultingImage.setColorMapOrigin(headerDataImage.GetColorMapOrigin());
	resultingImage.setColorMapLength(headerDataImage.GetColorMapLength());
	resultingImage.setColorMapDepth(headerDataImage.GetColorMapDepth());
	resultingImage.setxOrigin(headerDataImage.GetxOrigin());
	resultingImage.setyOrigin(headerDataImage.GetyOrigin());
	resultingImage.setWidth(headerDataImage.GetWidth());
	resultingImage.setHeight(headerDataImage.GetHeight());
	resultingImage.setBitsPerPixel(headerDataImage.GetBitsPerPixels());
	resultingImage.setImageDescriptor(headerDataImage.GetImageDescriptor());
}
void viewImageData(vector<Image>& pictures) {
	for (unsigned int i = 0; i < pictures.size(); i++) {
		cout << "Image width: " << pictures[i].GetWidth() << endl;
		cout << "Image height: " << pictures[i].GetHeight() << endl;
		cout << "Number of pixels: " << pictures[i].GetNumPixels() << endl;
	}
}
// test multiply with new idea
Image multiply(Image& picture1, Image& picture2) {
	Image resultingImage;
	setHeader(resultingImage, picture1);
	vector<Pixels> temp1 = picture1.GetPixels();
	vector<Pixels> temp2 = picture2.GetPixels();
	for (unsigned int i = 0; i < picture1.GetNumPixels(); i++) {

		float blueCheck = ((temp1[i].GetBlue() * temp2[i].GetBlue()) / (float)255);
		unsigned char blue;
		if (blueCheck > 255) {
			blueCheck = 255;
			blue = 255;
		}
		else if (blueCheck < 0) {
			blueCheck = 0;
			blue = 0;
		}
		else {
			blue = (unsigned char)(blueCheck + 0.5f);
		}
		float greenCheck = ((temp1[i].GetGreen() * temp2[i].GetGreen()) / (float)255);
		unsigned char green;
		if (greenCheck > 255) {
			greenCheck = 255;
			green = 255;
		}
		else if (greenCheck < 0) {
			greenCheck = 0;
			green = 0;
		}
		else {
			green = (unsigned char)(greenCheck + 0.5f);
		}
		float redCheck = ((temp1[i].GetRed() * temp2[i].GetRed()) / (float)255);
		unsigned char red;
		if (redCheck > 255) {
			redCheck = 255;
			red = 255;
		}
		else if (redCheck < 0) {
			redCheck = 0;
			red = 0;
		}
		else {
			red = (unsigned char)(redCheck + 0.5f);
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
void writeTGA(string filePath, Image pictures) {
	ofstream outputFile(filePath, ios_base::binary);
	if (outputFile.is_open()) {
		char idLength = pictures.getIdLength();
		outputFile.write((char*)&idLength, 1);
		char colorMapType = pictures.GetColorMapType();
		outputFile.write((char*)&colorMapType, 1);
		char dataTypeCode = pictures.GetDataType();
		outputFile.write((char*)&dataTypeCode, 1);
		short colorMapOrigin = pictures.GetColorMapOrigin();
		outputFile.write((char*)&colorMapOrigin, 2);
		short colorMapLength = pictures.GetColorMapLength();
		outputFile.write((char*)&colorMapLength, 2);
		char colorMapDepth = pictures.GetColorMapDepth();
		outputFile.write((char*)&colorMapDepth, 1);
		short xOrigin = pictures.GetxOrigin();
		outputFile.write((char*)&xOrigin, 2);
		short yOrigin = pictures.GetyOrigin();
		outputFile.write((char*)&yOrigin, 2);
		short width = pictures.GetWidth();
		outputFile.write((char*)&width, 2);
		short height = pictures.GetHeight();
		outputFile.write((char*)&height, 2);
		char bitsPerPixel = pictures.GetBitsPerPixels();
		outputFile.write((char*)&bitsPerPixel, 1);
		char imageDescriptor = pictures.GetImageDescriptor();
		outputFile.write((char*)&imageDescriptor, 1);
		vector<Pixels> temp = pictures.GetPixels();
		for (unsigned int i = 0; i < pictures.GetNumPixels(); i++) {
			unsigned char blue = temp[i].GetBlue();
			outputFile.write((char*)&blue, sizeof(blue));
			unsigned char green = temp[i].GetGreen();
			outputFile.write((char*)&green, sizeof(green));
			unsigned char red = temp[i].GetRed();
			outputFile.write((char*)&red, sizeof(red));
		}
		outputFile.close();
	}
	else {
		cout << "SHITS CLOSED" << endl;
	}
}
// test subtract with new idea
Image subtractTGA(Image& top, Image& bottom) {
	Image resultingImage;
	setHeader(resultingImage, top);
	vector<Pixels> topLayer = top.GetPixels();
	vector<Pixels> bottomLayer = bottom.GetPixels();
	for (unsigned int i = 0; i < top.GetNumPixels(); i++) {
		unsigned char blue;
		if ((bottomLayer[i].GetBlue() - topLayer[i].GetBlue()) < 0) {
			blue = 0;
		}
		else {
			blue = (bottomLayer[i].GetBlue() - topLayer[i].GetBlue());
		}
		unsigned char red;
		if ((bottomLayer[i].GetRed() - topLayer[i].GetRed()) < 0) {
			red = 0;
		}
		else {
			red = (bottomLayer[i].GetRed() - topLayer[i].GetRed());
		}
		unsigned char green;
		if ((bottomLayer[i].GetGreen() - topLayer[i].GetGreen()) < 0) {
			green = 0;
		}
		else {
			green = (bottomLayer[i].GetGreen() - topLayer[i].GetGreen());
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
// test 3
Image screenTGA(Image& top, Image& bottom) {
	Image resultingImage;
	setHeader(resultingImage, top);
	vector<Pixels> topLayer = top.GetPixels();
	vector<Pixels> bottomLayer = bottom.GetPixels();
	for (unsigned int i = 0; i < top.GetNumPixels(); i++) {
		unsigned char blue;
		float blueNormal1 = (topLayer[i].GetBlue() / (float)255);
		float blueNormal2 = (bottomLayer[i].GetBlue() / (float)255);
		float blueTest = (1 - (1 - blueNormal1) * (1 - blueNormal2)) * 255;
		if (blueTest > 255) {
			blueTest = 255;
			blue = 255;
		}
		else if (blueTest < 0) {
			blueTest = 0;
			blue = 0;
		}
		else {
			blue = (unsigned char)(blueTest + 0.5f);
		}
		unsigned char green;
		float greenNormal1 = (topLayer[i].GetGreen() / (float)255);
		float greenNormal2 = (bottomLayer[i].GetGreen() / (float)255);
		float greenTest = (1 - (1 - greenNormal1) * (1 - greenNormal2)) * 255;
		if (greenTest > 255) {
			greenTest = 255;
			green = 255;
		}
		else if (greenTest < 0) {
			greenTest = 0;
			green = 0;
		}
		else {
			green = (unsigned char)(greenTest + 0.5f);
		}
		unsigned char red;
		float redNormal1 = (topLayer[i].GetRed() / (float)255);
		float redNormal2 = (bottomLayer[i].GetRed() / (float)255);
		float redTest = (1 - (1 - redNormal1) * (1 - redNormal2)) * 255;
		if (redTest > 255) {
			redTest = 255;
			red = 255;
		}
		else if (redTest < 0) {
			redTest = 0;
			red = 0;
		}
		else {
			red = (unsigned char)(redTest + 0.5f);
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image overlayTGA(Image& top, Image& bottom) {
	Image resultingImage;
	setHeader(resultingImage, top);
	vector<Pixels> topLayer = top.GetPixels();
	vector<Pixels> bottomLayer = bottom.GetPixels();
	for (unsigned int i = 0; i < top.GetNumPixels(); i++) {
		unsigned char blue;
		float blueNormal1 = (topLayer[i].GetBlue() / (float)255);
		float blueNormal2 = (bottomLayer[i].GetBlue() / (float)255);
		float blueTest;
		if (blueNormal2 <= 0.5) {
			blueTest = (2 * blueNormal1 * blueNormal2) * 255;
			blue = (unsigned char)(blueTest + 0.5f);
		}
		else if (blueNormal2 > 0.5) {
			blueTest = (1 - (2 * (1 - blueNormal1) * (1 - blueNormal2))) * 255;

			blue = (unsigned char)(blueTest + 0.5f);
		}
		unsigned char green;
		float greenNormal1 = (topLayer[i].GetGreen() / (float)255);
		float greenNormal2 = (bottomLayer[i].GetGreen() / (float)255);
		float greenTest;
		if (greenNormal2 <= 0.5) {
			greenTest = (2 * greenNormal1 * greenNormal2) * 255;
			green = (unsigned char)(greenTest + 0.5f);
		}
		else if (greenNormal2 > 0.5) {
			greenTest = (1 - (2 * (1 - greenNormal1) * (1 - greenNormal2))) * 255;
			green = (unsigned char)(greenTest + 0.5f);
		}
		unsigned char red;
		float redNormal1 = (topLayer[i].GetRed() / (float)255);
		float redNormal2 = (bottomLayer[i].GetRed() / (float)255);
		float redTest;
		if (redNormal2 <= 0.5) {
			redTest = (2 * redNormal1 * redNormal2) * 255;
			red = (unsigned char)(redTest + 0.5f);
		}
		else if (redNormal2 > 0.5) {
			redTest = (1 - (2 * (1 - redNormal1) * (1 - redNormal2))) * 255;
			red = (unsigned char)(redTest + 0.5f);
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
bool Test(Image& expected, Image& measured) {
	bool check = true;
	if (measured.getIdLength() != expected.getIdLength()) {
		return false;
	}
	if (measured.GetColorMapType() != expected.GetColorMapType()) {
		return false;
	}
	if (measured.GetDataType() != expected.GetDataType()) {
		return false;
	}
	if (measured.GetColorMapOrigin() != expected.GetColorMapOrigin()) {
		return false;
	}
	if (measured.GetColorMapLength() != expected.GetColorMapLength()) {
		return false;
	}
	if (measured.GetColorMapDepth() != expected.GetColorMapDepth()) {
		return false;
	}
	if (measured.GetxOrigin() != expected.GetxOrigin()) {
		return false;
	}
	if (measured.GetyOrigin() != expected.GetyOrigin()) {
		return false;
	}
	if (measured.GetWidth() != expected.GetWidth()) {
		return false;
	}
	if (measured.GetHeight() != expected.GetHeight()) {
		return false;
	}
	if (measured.GetBitsPerPixels() != expected.GetBitsPerPixels()) {
		return false;
	}
	if (measured.GetImageDescriptor() != expected.GetImageDescriptor()) {
		return false;
	}
	vector<Pixels> measuredPix = measured.GetPixels();
	vector<Pixels> expectedPix = expected.GetPixels();
	for (unsigned int i = 0; i < measured.GetNumPixels(); i++) {
		if (measuredPix[i].GetBlue() != expectedPix[i].GetBlue()) {
			return false;
		}
		if (measuredPix[i].GetGreen() != expectedPix[i].GetGreen()) {
			return false;
		}
		if (measuredPix[i].GetRed() != expectedPix[i].GetRed()) {
			return false;
		}
	}
	return true;


}
Image addGreen(Image& picture, unsigned char amount) {
	Image resultingImage;
	setHeader(resultingImage, picture);
	vector<Pixels> pixels = picture.GetPixels();
	for (unsigned int i = 0; i < picture.GetNumPixels(); i++) {
		unsigned char blue = pixels[i].GetBlue();
		unsigned char red = pixels[i].GetRed();
		unsigned char green;
		if ((pixels[i].GetGreen() + amount) > 255) {
			green = 255;
		}
		else {
			green = (pixels[i].GetGreen() + amount);
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image multiplyChannel(Image& picture, unsigned char redAmount, unsigned char blueAmount) {
	Image resultingImage;
	setHeader(resultingImage, picture);
	vector<Pixels> pixels = picture.GetPixels();
	for (unsigned int i = 0; i < picture.GetNumPixels(); i++) {
		unsigned char blue = (pixels[i].GetBlue() * blueAmount);
		unsigned char green = pixels[i].GetGreen();
		unsigned char red;
		if ((pixels[i].GetRed() * redAmount) > 255) {
			red = 255;
		}
		else {
			red = (pixels[i].GetRed() * redAmount);
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image seperateChannels(Image& picture, string channel) {
	Image resultingImage;
	setHeader(resultingImage, picture);
	vector<Pixels> pixels = picture.GetPixels();
	for (unsigned int i = 0; i < picture.GetNumPixels(); i++) {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		if (channel == "blue") {
			blue = pixels[i].GetBlue();
			green = pixels[i].GetBlue();
			red = pixels[i].GetBlue();
		}
		else if (channel == "green") {
			blue = pixels[i].GetGreen();
			green = pixels[i].GetGreen();
			red = pixels[i].GetGreen();
		}
		else if (channel == "red") {
			blue = pixels[i].GetRed();
			green = pixels[i].GetRed();
			red = pixels[i].GetRed();
		}
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image combineChannels(Image& redChannel, Image& greenChannel, Image& blueChannel) {
	Image resultingImage;
	setHeader(resultingImage, redChannel);
	vector<Pixels> redPixels = redChannel.GetPixels();
	vector<Pixels> greenPixels = greenChannel.GetPixels();
	vector<Pixels> bluePixels = blueChannel.GetPixels();
	for (unsigned int i = 0; i < redChannel.GetNumPixels(); i++) {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		blue = bluePixels[i].GetBlue();
		green = greenPixels[i].GetGreen();
		red = redPixels[i].GetRed();
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image rotate180(Image& picture) {
	Image resultingImage;
	setHeader(resultingImage, picture);
	vector<Pixels> pixels = picture.GetPixels();
	for (int i = picture.GetNumPixels() - 1; i >= 0; i--) {
		unsigned char blue = pixels[i].GetBlue();
		unsigned char green = pixels[i].GetGreen();
		unsigned char red = pixels[i].GetRed();
		Pixels newPixel(blue, green, red);
		resultingImage.addPixel(newPixel);
	}
	return resultingImage;
}
Image extraCredit(Image& picture1, Image& picture2, Image& picture3, Image& picture4) {
	Image resultingImage;
	setHeader(resultingImage, picture1);
	resultingImage.setHeight(picture1.GetHeight() * 2);
	resultingImage.setWidth(picture1.GetWidth() * 2);
	vector<Pixels> pixels3 = picture3.GetPixels();
	vector<Pixels> pixels4 = picture4.GetPixels();
	int count3 = 0;
	int count4 = 0;
	for (unsigned int i = 0; i < picture3.GetHeight(); i++) {
		for (unsigned int j = 0; j < resultingImage.GetWidth(); j++) {
			unsigned char blue, green, red;
			if (j < picture3.GetWidth()) {
				blue = pixels3[count3].GetBlue();
				green = pixels3[count3].GetGreen();
				red = pixels3[count3].GetRed();
				Pixels newPixel(blue, green, red);
				resultingImage.addPixel(newPixel);
				count3++;
			}
			else {
				blue = pixels4[count4].GetBlue();
				green = pixels4[count4].GetGreen();
				red = pixels4[count4].GetRed();
				Pixels newPixel(blue, green, red);
				resultingImage.addPixel(newPixel);
				count4++;
			}
		}
	}
	//height for top half
	vector<Pixels> pixels1 = picture1.GetPixels();
	vector<Pixels> pixels2 = picture2.GetPixels();
	int count1 = 0;
	int count2 = 0;
	for (unsigned int i = 0; i < picture1.GetHeight(); i++) {
		//width of top half
		for (unsigned int j = 0; j < resultingImage.GetWidth(); j++) {
			unsigned char blue, green, red;
			if (j < picture1.GetWidth()) {
				blue = pixels1[count1].GetBlue();
				green = pixels1[count1].GetGreen();
				red = pixels1[count1].GetRed();
				Pixels newPixel(blue, green, red);
				resultingImage.addPixel(newPixel);
				count1++;
			}
			else {
				blue = pixels2[count2].GetBlue();
				green = pixels2[count2].GetGreen();
				red = pixels2[count2].GetRed();
				Pixels newPixel(blue, green, red);
				resultingImage.addPixel(newPixel);
				count2++;
			}
		}
	}

	return resultingImage;
}
int main() {

	//test restructuring!! </3
	Image multiplyImage1 = getImageData("input/layer1.tga");
	Image multiplyImage2 = getImageData("input/pattern1.tga");
	Image task1 = multiply(multiplyImage1, multiplyImage2);
	writeTGA("output/part1.tga", task1);
	Image test1 = getImageData("examples/EXAMPLE_part1.tga");
	if (Test(test1, task1)) {
		cout << "Test #1...... Passed!" << endl;
	}
	else {
		cout << "Test #1...... Failed!" << endl;
	}
	//test2!
	Image subtractImage1 = getImageData("input/layer2.tga");
	Image subtractImage2 = getImageData("input/car.tga");
	Image task2 = subtractTGA(subtractImage1, subtractImage2);
	writeTGA("output/part2.tga", task2);
	Image test2 = getImageData("examples/EXAMPLE_part2.tga");
	if (Test(test2, task2)) {
		cout << "Test #2...... Passed!" << endl;
	}
	else {
		cout << "Test #2...... Failed!" << endl;
	}
	//test3!
	Image multiplyImage3 = getImageData("input/layer1.tga");
	Image multiplyImage4 = getImageData("input/pattern2.tga");
	Image test3_bottom = multiply(multiplyImage3, multiplyImage4);
	Image test3_top = getImageData("input/text.tga");
	Image task3 = screenTGA(test3_top, test3_bottom);
	writeTGA("output/part3.tga", task3);
	Image test3 = getImageData("examples/EXAMPLE_part3.tga");
	if (Test(test3, task3)) {
		cout << "Test #3...... Passed!" << endl;
	}
	else {
		cout << "Test #3...... Failed!" << endl;
	}
	//test4
	Image muliplyImage5 = getImageData("input/layer2.tga");
	Image multiplyImage6 = getImageData("input/circles.tga");
	Image task4_bottom = multiply(muliplyImage5, multiplyImage6);
	Image task4_top = getImageData("input/pattern2.tga");
	Image task4 = subtractTGA(task4_top, task4_bottom);
	writeTGA("output/part4.tga", task4);
	Image test4 = getImageData("examples/EXAMPLE_part4.tga");
	if (Test(test4, task4)) {
		cout << "Test #4...... Passed!" << endl;
	}
	else {
		cout << "Test #4...... Failed!" << endl;
	}
	//test 5
	Image overlay1_top = getImageData("input/layer1.tga");
	Image overlay1_bottom = getImageData("input/pattern1.tga");
	Image task5 = overlayTGA(overlay1_top, overlay1_bottom);
	writeTGA("output/part5.tga", task5);
	Image test5 = getImageData("examples/EXAMPLE_part5.tga");
	if (Test(test5, task5)) {
		cout << "Test #5...... Passed!" << endl;
	}
	else {
		cout << "Test #5...... Failed!" << endl;
	}
	//test 6
	Image imageAddGreen = getImageData("input/car.tga");
	Image task6 = addGreen(imageAddGreen, 200);
	writeTGA("output/part6.tga", task6);
	Image test6 = getImageData("examples/EXAMPLE_part6.tga");
	if (Test(test6, task6)) {
		cout << "Test #6...... Passed!" << endl;
	}
	else {
		cout << "Test #6...... Failed!" << endl;
	}
	//test 7 
	Image imageChannelMultiply = getImageData("input/car.tga");
	Image task7 = multiplyChannel(imageChannelMultiply, 4, 0);
	writeTGA("output/part7.tga", task7);
	Image test7 = getImageData("examples/EXAMPLE_part7.tga");
	if (Test(test7, task7)) {
		cout << "Test #7...... Passed!" << endl;
	}
	else {
		cout << "Test #7...... Failed!" << endl;
	}
	//test 8r
	Image imageSeperateChannelsr = getImageData("input/car.tga");
	Image task8r = seperateChannels(imageSeperateChannelsr, "red");
	writeTGA("output/part8_r.tga", task8r);
	Image test8r = getImageData("examples/EXAMPLE_part8_r.tga");
	if (Test(test8r, task8r)) {
		cout << "Test #8r...... Passed!" << endl;
	}
	else {
		cout << "Test #8r...... Failed!" << endl;
	}
	//test 8b
	Image imageSeperateChannelsb = getImageData("input/car.tga");
	Image task8b = seperateChannels(imageSeperateChannelsb, "blue");
	writeTGA("output/part8_b.tga", task8b);
	Image test8b = getImageData("examples/EXAMPLE_part8_b.tga");
	if (Test(test8b, task8b)) {
		cout << "Test #8b...... Passed!" << endl;
	}
	else {
		cout << "Test #8b...... Failed!" << endl;
	}
	//test 8g
	Image imageSeperateChannelsg = getImageData("input/car.tga");
	Image task8g = seperateChannels(imageSeperateChannelsg, "green");
	writeTGA("output/part8_g.tga", task8g);
	Image test8g = getImageData("examples/EXAMPLE_part8_g.tga");
	if (Test(test8g, task8g)) {
		cout << "Test #8g...... Passed!" << endl;
	}
	else {
		cout << "Test #8g...... Failed!" << endl;
	}
	//test 9
	Image imageCombineChannelsR = getImageData("input/layer_red.tga");
	Image imageCombineChannelsG = getImageData("input/layer_green.tga");
	Image imageCombineChannelsB = getImageData("input/layer_blue.tga");
	Image task9 = combineChannels(imageCombineChannelsR, imageCombineChannelsG, imageCombineChannelsB);
	writeTGA("output/part9.tga", task9);
	Image test9 = getImageData("examples/EXAMPLE_part9.tga");
	if (Test(test9, task9)) {
		cout << "Test #9...... Passed!" << endl;
	}
	else {
		cout << "Test #9...... Failed!" << endl;
	}
	//test 10
	Image imageRotate = getImageData("input/text2.tga");
	Image task10 = rotate180(imageRotate);
	writeTGA("output/part10.tga", task10);
	Image test10 = getImageData("examples/EXAMPLE_part10.tga");
	if (Test(test10, task10)) {
		cout << "Test #10...... Passed!" << endl;
	}
	else {
		cout << "Test #10...... Failed!" << endl;
	}
	Image extraImage1 = getImageData("input/car.tga");
	Image extraImage2 = getImageData("input/circles.tga");
	Image extraImage3 = getImageData("input/text.tga");
	Image extraImage4 = getImageData("input/pattern1.tga");
	Image extraCreditImage = extraCredit(extraImage1, extraImage2, extraImage3, extraImage4);
	writeTGA("output/extracredit.tga", extraCreditImage);
	Image testExtra = getImageData("examples/EXAMPLE_extracredit.tga");
	if (Test(testExtra, extraCreditImage)) {
		cout << "Extra Credit...... Passed!" << endl;
	}
	else {
		cout << "Extra Credit...... Failed!" << endl;
	}
	return 0;
}