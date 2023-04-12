#pragma once
#include <vector>
#include "Layer.h"


class LayerManager
{
public:
	LayerManager();
	~LayerManager();


	void getAllBlendedImage(CImage* img);
	void addLayer(CImage* img, CString name = _T(""));
	void addLayer(LPCTSTR filePath, CString name = _T(""));
	void addLayer(int width, int height, int bitPerPixel, CString name = _T(""));
	void addLayer();
	void blend(int i1, int i2);
	void remove(int i);
	void getOneBlendedImage(CImage* img, int i);	//레이어 각각의 이미지 불러오는 함수
	CAdvImg* getLayerImage(int i);
	Layer* getLayer(int i);
	int getLayerCounts();

private :
	std::vector<Layer*> layers;
	//int width;
	//int height;
	//int bytePerPixel;
};


