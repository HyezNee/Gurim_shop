
#include "stdafx.h"
#include "LayerManager.h"
#include "TeamProject.h"


LayerManager::LayerManager()
{
}


LayerManager::~LayerManager()
{
	for (int i = 0; i < layers.size(); ++i) {
		delete layers[i];
	}
}

void LayerManager::getAllBlendedImage(CImage* img)
{
	Layer temp(img);
	
	for (int i = 0; i < layers.size(); ++i) {
		if (layers[i]->show) {
			temp.blend(layers[i]);
		}
		
	}
}

void LayerManager::addLayer(CImage* img, CString name)
{
	Layer* newLayer = new Layer(img,name);
	layers.push_back(newLayer);
}

void LayerManager::addLayer(LPCTSTR filePath ,CString name)
{
	Layer* newLayer = new Layer(filePath,name);
	layers.push_back(newLayer);
}

void LayerManager::addLayer(int width, int height, int bitPerPixel, CString name)
{
	Layer* newLayer = new Layer(width,height,bitPerPixel, name);
	layers.push_back(newLayer);
}

void LayerManager::addLayer()
{
	Layer* newLayer = new Layer(1920,1080,24);
	layers.push_back(newLayer);
}

void LayerManager::blend(int i1, int i2)
{
	layers[i1]->blend(layers[i2]);
	delete layers[i2];
	layers.erase(layers.begin() + i2);
}

void LayerManager::remove(int i)
{
	layers.erase(layers.begin() + i);
}

void LayerManager::getOneBlendedImage(CImage * img, int i)
{
	Layer temp(img);

	if (layers[i]->show)
		temp.blend(layers[i]);
}

CAdvImg* LayerManager::getLayerImage(int i)
{
	return layers[i]->getImg();
}

Layer* LayerManager::getLayer(int i)
{
	return layers[i];
}

int LayerManager::getLayerCounts()
{
	return layers.size();
}
