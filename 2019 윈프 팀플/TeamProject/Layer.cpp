#include "stdafx.h"
#include "Layer.h"
#include "TeamProject.h"


Layer::Layer()
	:alpha(1.0),
	useTransparentColor(true),
	show(true),
	transparentColorR(0),
	transparentColorG(0),
	transparentColorB(0),
	shouldImgDestroyed(false),
	img(0),
	name(_T(""))
{
	
}

Layer::Layer(LPCTSTR imgPath, CString name):Layer()
{
	img = new CAdvImg();
	img->Load(imgPath);
	shouldImgDestroyed = true;
	this->name = name;
}

Layer::Layer(int width, int height, int bitPerPixel, CString name):Layer()
{
	img = new CAdvImg();
	img->Create(width, height, bitPerPixel);
	shouldImgDestroyed = true;
	this->name = name;
}

Layer::Layer(CImage* img, CString name)
{
	Layer();
	shouldImgDestroyed = false;
	this->img = (CAdvImg*) img;
	this->name = name;
}

Layer::~Layer()
{
	if(shouldImgDestroyed)
		delete img;
}

//blend layer over this
void Layer::blend(Layer* layer)
{
	if (!layer->useTransparentColor) {
		this->img->applyAlphaBlend(layer->img, layer->alpha,-1,-1,-1,true);
	}
	else {
		this->img->applyAlphaBlend(layer->img, layer->alpha, layer->transparentColorR, layer->transparentColorG, layer->transparentColorB, true);
	}
	
}

void Layer::changeTransparentColor(COLORREF cref,bool changeImg)
{
	this->changeTransparentColor(GetRValue(cref), GetGValue(cref), GetBValue(cref),changeImg);
}

void Layer::changeTransparentColor(BYTE r, BYTE g, BYTE b, bool changeImg)
{
	if(changeImg && this->useTransparentColor)
		this->img->applyColorSwitch(this->transparentColorR, this->transparentColorG, this->transparentColorB, r, g, b);

	this->transparentColorR = r;
	this->transparentColorG = g;
	this->transparentColorB = b;
	
}

void Layer::setAlpha(double alpha)
{
	this->alpha = 0.0 > alpha ? 0.0 : 1.0 < alpha ? 1.0 : alpha;
}

double Layer::getAlpha()
{
	return alpha;
}

COLORREF Layer::getTransparentColor()
{
	return RGB(transparentColorR,transparentColorG,transparentColorB);
}

CAdvImg* Layer::getImg()
{
	return img;
}
