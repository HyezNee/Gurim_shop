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
	void getOneBlendedImage(CImage* img, int i);	//���̾� ������ �̹��� �ҷ����� �Լ�
	CAdvImg* getLayerImage(int i);
	Layer* getLayer(int i);
	int getLayerCounts();

private :
	std::vector<Layer*> layers;
	//int width;
	//int height;
	//int bytePerPixel;
};


