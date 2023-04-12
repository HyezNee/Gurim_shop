#pragma once

#include "stdafx.h"
#include "ImgKernel.h"
#include <functional>


class CAdvImg : public CImage{
	public:
	CAdvImg();
	~CAdvImg();

	void applyKernel(ImgKernel& kernel, CImage* dest = NULL);
	void applyNegative(CImage* dest = NULL);
	void applyGray(CImage* dest = NULL);
	void rotateHue(int degree);
	void applyPerPixels(void (*func)(BYTE* r, BYTE* g, BYTE* b));
	void applyLinearFunc(double alpha, double beta);
	void addHSV(int hue, int saturation, int value);
	void applyPixelization(int blockRowPixels, int blockColPixels);	//모자이크 함수

	static void rgb2hsv(int r, int g, int b, double* hsv);
	static void hsv2rgb(double h, double s, double v, BYTE* rgb);

	void applyAlphaBlend(CImage* img, double alpha, int r, int g, int b, bool ignoreAlphaChannel);
	void applyColorSwitch(BYTE r1, BYTE g1, BYTE b1, BYTE r2, BYTE g2, BYTE b2);
};

