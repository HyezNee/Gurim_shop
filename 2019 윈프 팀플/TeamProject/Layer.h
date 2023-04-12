#pragma once

#include "CAdvImg.h"

class Layer
{
public:
	Layer();
	Layer(LPCTSTR imgPath,CString name=_T(""));
	Layer(int width,int height,int bitPerPixel, CString name = _T(""));
	Layer(CImage* img, CString name = _T(""));
	
	~Layer();

	// bool to use transparentColor or not
	bool useTransparentColor;

	CString name;


	bool show;
	void blend(Layer* layer);
	void changeTransparentColor(COLORREF cref,bool changeImg=false);
	void changeTransparentColor(BYTE r, BYTE g, BYTE b,bool changeImg=false);

	void setAlpha(double alpha);
	double getAlpha();

	COLORREF getTransparentColor();

	CAdvImg* getImg();

private:

	BYTE transparentColorR; //  color to make transparent if bit per pixel is 24
	BYTE transparentColorG;
	BYTE transparentColorB;

	
	double alpha;
	CAdvImg* img;

	bool shouldImgDestroyed;
	
};

