
#include "stdafx.h"
#include "CAdvImg.h"

#include <time.h>


CAdvImg::CAdvImg() {
}


CAdvImg::~CAdvImg() {
}

void CAdvImg::applyKernel(ImgKernel& kernel, CImage* dest) {

	const int w = GetWidth();
	const int h = GetHeight();
	const int pitch = GetPitch();
	const int bytePerPixel = GetBPP() / 8;

	const int kernelRowCenter = (kernel.getRowCount() + 1) / 2;
	const int kernelColumnCenter = (kernel.getColCount() + 1) / 2;
	const int kernelRowLoopEnd = kernel.getRowCount() - kernelRowCenter;
	const int kernelColumnLoopEnd = kernel.getColCount() - kernelColumnCenter;
	const bool separated = kernel.isSeparated();

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	/*if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}*/
	
	CImage* temp = dest == 0 ? new CImage() : dest;
	temp->Create(w, h, GetBPP());

	BYTE* orgBits = (BYTE*)GetBits();
	BYTE* destBits = (BYTE*)temp->GetBits();

	// for not separated filter
	if (!separated) {
		double** kernelMat = kernel.getArray();
		double kernelVal = 0.0;
		int ti, tj;
		int pos;

		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w ; ++j) {
				double t_pixel[3] = { 0,0,0 };
				for (int m = -kernelRowCenter; m < kernelRowLoopEnd; ++m) {
					for (int n = -kernelColumnCenter; n < kernelColumnLoopEnd; ++n) {

						tj = j + n;
						if (tj < 0)
							tj = 0;
						else if (tj >= w)
							tj = w - 1;

						ti = i + m;
						if (ti < 0)
							ti = 0;
						else if (ti >= h)
							ti = h -1;

						pos = ti * pitch + bytePerPixel * tj;
						kernelVal = kernelMat[m + kernelRowCenter][n + kernelColumnCenter];
						t_pixel[0] += kernelVal * orgBits[pos + bp]; //b
						t_pixel[1] += kernelVal * orgBits[pos + gp]; //g
						t_pixel[2] += kernelVal * orgBits[pos + rp]; //r

					}
				}

				for (int i = 0; i < 3; ++i) {
					t_pixel[i] = t_pixel[i] < 0.0 ? 0.0 : t_pixel[i] > 255.0 ? 255.0 : t_pixel[i];
				}
				//destBits[i*dPitch + 4 * j ] = 0x00;
				destBits[i * pitch + bytePerPixel * j + bp] = (BYTE)t_pixel[0];
				destBits[i * pitch + bytePerPixel * j + gp] = (BYTE)t_pixel[1];
				destBits[i * pitch + bytePerPixel * j + rp] = (BYTE)t_pixel[2];
			}
		}
	}
	else { // for separated filter
		double* srv = kernel.getSepRowVec();
		double* scv = kernel.getSepColVec();
		const int absPitch = pitch > 0 ? pitch : -pitch;
		double kernelVal = 0;
		int ti, tj;
		int pos;

		BYTE* tempBits = new BYTE[absPitch * h * bytePerPixel]();

		// apply row vector
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				double t_pixel[3] = { 0.0, };

				for (int n = -kernelColumnCenter; n < kernelColumnLoopEnd; ++n) {
					tj = j + n;
					if (tj < 0) 
						tj = 0;
					else if (tj >= w) 
						tj = w - 1;

					pos = i * pitch + bytePerPixel * tj;

					kernelVal = srv[kernelColumnCenter + n];
					t_pixel[0] += kernelVal * orgBits[ pos + bp]; //b
					t_pixel[1] += kernelVal * orgBits[ pos + gp]; //g
					t_pixel[2] += kernelVal * orgBits[ pos + rp]; //r
				}

				for (int k = 0; k < 3; ++k) {
					t_pixel[k] = t_pixel[k] < 0.0 ? 0.0 : t_pixel[k] > 255.0 ? 255.0 : t_pixel[k];
				}

				tempBits[i * absPitch + j * bytePerPixel + bp] = (BYTE) t_pixel[0];
				tempBits[i * absPitch + j * bytePerPixel + gp] = (BYTE) t_pixel[1];
				tempBits[i * absPitch + j * bytePerPixel + rp] = (BYTE) t_pixel[2];
			}
		}

		// apply column vector
		for (int i =0; i < h; ++i) {
			for (int j =0; j < w ; ++j) {
				double t_pixel[3] = { 0.0, };
				for (int m = -kernelRowCenter; m < kernelRowLoopEnd ; ++m) {

					ti = i + m;
					if (ti < 0) 
						ti = 0;
					else if (ti >= h) 
						ti = h -1;

					pos = ti * absPitch + bytePerPixel * j;

					kernelVal = scv[kernelRowCenter + m];
					t_pixel[0] += kernelVal * tempBits[ pos + bp]; //b
					t_pixel[1] += kernelVal * tempBits[ pos + gp]; //g
					t_pixel[2] += kernelVal * tempBits[ pos + rp]; //r

				}

				for (int k = 0; k < 3; ++k) {
					t_pixel[k] = t_pixel[k] < 0.0 ? 0.0 : t_pixel[k] > 255.0 ? 255.0 : t_pixel[k];
				}

				destBits[i * pitch + j * bytePerPixel + bp] = t_pixel[0];
				destBits[i * pitch + j * bytePerPixel + gp] = t_pixel[1];
				destBits[i * pitch + j * bytePerPixel + rp] = t_pixel[2];

			}
		}
		delete[] tempBits;
	}



	// If there's dest is 0 , copy pixels in temp to this, then destroy temp
	if (dest == NULL) {
		temp->BitBlt(this->GetDC(), 0, 0);
		this->ReleaseDC();



		delete temp;
	}

}

void CAdvImg::applyNegative(CImage * dest)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	/*if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}*/

	CImage* temp = dest == 0 ? new CImage() : dest;
	temp->Create(w, h, GetBPP());

	BYTE* destBits = (BYTE*)temp->GetBits();

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			destBits[i * pitch + bytePerPixel * j + rp] = 255 - orgBits[i * pitch + bytePerPixel * j + rp]; //r
			destBits[i * pitch + bytePerPixel * j + gp] = 255 - orgBits[i * pitch + bytePerPixel * j + gp]; //g
			destBits[i * pitch + bytePerPixel * j + bp] = 255 - orgBits[i * pitch + bytePerPixel * j + bp]; //b
		}
	}

	if (dest == NULL) {
		temp->BitBlt(this->GetDC(), 0, 0);
		this->ReleaseDC();
		delete temp;
	}
}

void CAdvImg::applyGray(CImage * dest)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();

	CImage* temp = dest == 0 ? new CImage() : dest;
	temp->Create(w, h, GetBPP());

	BYTE* destBits = (BYTE*)temp->GetBits();

	BYTE r = 0, g = 0, b = 0, y = 0;

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;
	int offset = 0;

	//if (bytePerPixel == 4) {
	//	++rp;
	//	++gp;
	//	++bp;
	//	++offset;
	//}

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			r = orgBits[i * pitch + bytePerPixel * j + rp];
			g = orgBits[i * pitch + bytePerPixel * j + gp];
			b = orgBits[i * pitch + bytePerPixel * j + bp];
			y = (BYTE) 0.2126f * r + 0.7152f * g + 0.0722f * b;

			//if (y > 255) y = 255;
			//else if (y < 0) y = 0;

			if (y == 255) {
				CString str;
				str.Format(_T("pos %d %d"), i, j);
				TRACE(str);
			}

			for (int k = 0; k < 3; ++k) {
				destBits[i * pitch + bytePerPixel * j + k + offset] = y;
			}
		}
	}

	if (dest == NULL) {
		temp->BitBlt(this->GetDC(), 0, 0);
		this->ReleaseDC();
		delete temp;
	}

}

void CAdvImg::rotateHue(int degree)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();
	double hsv[3] = { 0.0, };
	BYTE rgb[3] = { 0, };
	int d = 0;

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}


	BYTE r = 0, g = 0, b = 0, y = 0;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {

			r = orgBits[i * pitch + bytePerPixel * j + rp];
			g = orgBits[i * pitch + bytePerPixel * j + gp];
			b = orgBits[i * pitch + bytePerPixel * j + bp];

			rgb2hsv(r, g, b, hsv);
			d = hsv[0] * 360;
			d += degree + 360;
			d %= 360;
			hsv[0] = d / 360.0;
			hsv2rgb(hsv[0], hsv[1], hsv[2], rgb);
			
			orgBits[i * pitch + bytePerPixel * j + rp] = rgb[0];
			orgBits[i * pitch + bytePerPixel * j + gp] = rgb[1];
			orgBits[i * pitch + bytePerPixel * j + bp] = rgb[2];
		}
	}

}


void CAdvImg::applyPerPixels(void(*func)(BYTE* r,BYTE*g,BYTE*b))
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();
	//int rgb[3];

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			func(orgBits + (i * pitch + bytePerPixel * j + rp),
				orgBits + (i * pitch + bytePerPixel * j + gp),
				orgBits + (i * pitch + bytePerPixel * j + bp));
		}
	}

}

void CAdvImg::applyLinearFunc(double alpha, double beta)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	double _t_pixel[3];

	/*if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}*/

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			_t_pixel[0] = orgBits[i * pitch + bytePerPixel * j + rp] * alpha + beta; //r
			_t_pixel[1] = orgBits[i * pitch + bytePerPixel * j + gp] * alpha + beta; //g
			_t_pixel[2] = orgBits[i * pitch + bytePerPixel * j + bp] * alpha + beta; //b

			for (int k = 0; k < 3; ++k) {
				_t_pixel[k] = _t_pixel[k] < 0.0 ? 0.0 : _t_pixel[k] > 255.0 ? 255.0 : _t_pixel[k];
			}

			orgBits[i * pitch + bytePerPixel * j + rp] = _t_pixel[0];
			orgBits[i * pitch + bytePerPixel * j + gp] = _t_pixel[1];
			orgBits[i * pitch + bytePerPixel * j + bp] = _t_pixel[2];
		}
	}
}

void CAdvImg::addHSV(int hue, int saturation, int value)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();
	double hsv[3] = { 0.0, };
	BYTE rgb[3] = { 0, };
	int _d = 0;
	int _s = 0;
	int _v = 0;

	// preset for 24bit rgb format (in mfc, sequence is bgr)
	int rp = 2;
	int gp = 1;
	int bp = 0;

	/*if (bytePerPixel == 4) {
		++rp;
		++gp;
		++bp;
	}*/


	BYTE r = 0, g = 0, b = 0, y = 0;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {

			r = orgBits[i * pitch + bytePerPixel * j + rp];
			g = orgBits[i * pitch + bytePerPixel * j + gp];
			b = orgBits[i * pitch + bytePerPixel * j + bp];

			rgb2hsv(r, g, b, hsv);

			// hue
			_d = hsv[0] * 360;
			_d += hue + 360;
			_d %= 360;
			hsv[0] = _d / 360.0;

			// saturation
			_s = hsv[1] * 100;
			_s += saturation;
			hsv[1] = _s / 100.0;
			hsv[1] = hsv[1] < 0.0 ? 0.0 : hsv[1] > 1.0 ? 1.0 : hsv[1];

			// value
			_s = hsv[2] * 100;
			_s += value;
			hsv[2] = _s / 100.0;
			hsv[2] = hsv[2] < 0.0 ? 0.0 : hsv[2] > 1.0 ? 1.0 : hsv[2];

			hsv2rgb(hsv[0], hsv[1], hsv[2], rgb);

			orgBits[i * pitch + bytePerPixel * j + rp] = rgb[0];
			orgBits[i * pitch + bytePerPixel * j + gp] = rgb[1];
			orgBits[i * pitch + bytePerPixel * j + bp] = rgb[2];
		}
	}
}

void CAdvImg::rgb2hsv(int r, int g, int b, double* hsv)
{
	double h,s, v; // h : 0~1 , s : 0~1 , v : 0~1

	double max = max(r, max(g, b));
	double min = min(r, min(g, b));
	double delta = max - min;
	
	// hue calculation
	if (delta == 0) {
		h = 0;
	}
	else if (max == r) {
		h = ((double) g - b) / delta ;
	}
	else if (max == g) {
		h = (((double)b - r) / delta +2);
	}
	else if (max == b) {
		h = (((double)r - g) / delta + 4);
	}
	h = h/6 - (int)(h/6);

	// saturation calculation
	if (max == 0) {
		s = 0.0;
	}
	else {
		s = (double)delta / max;
	}

	// value calculation
	v = max / 255.0;

	hsv[0] = h;
	hsv[1] = s;
	hsv[2] = v;
}

void CAdvImg::hsv2rgb(double h, double s, double v, BYTE* rgb)
{
	if (s <= 0.001) {
		for (int i = 0; i < 3; ++i)
			rgb[i] = v * 255;
		return;
	}
	int i = h * 6;
	double f = h * 6.0 - i;
	double p = v * (1.0 - s);
	double q = v * (1.0 - s * f);
	double t = v * (1.0 - s * (1.0 - f));
	i %= 6;

	
	
	double trgb[3] = { 0.0, };
	switch (i) {
	case 0:
		trgb[0] = v;
		trgb[1] = t;
		trgb[2] = p;
		break;
	case 1:
		trgb[0] = q;
		trgb[1] = v;
		trgb[2] = p;
		break;
	case 2:
		trgb[0] = p;
		trgb[1] = v;
		trgb[2] = t;
		break;
	case 3:
		trgb[0] = p;
		trgb[1] = q;
		trgb[2] = v;
		break;
	case 4:
		trgb[0] = t;
		trgb[1] = p;
		trgb[2] = v;
		break;
	case 5:
		trgb[0] = v;
		trgb[1] = p;
		trgb[2] = q;
		break;
	}

	for (int i = 0; i < 3; ++i) {
		int val =trgb[i] * 255;
		val = val < 0 ? 0 : val > 255 ? 255 : val;
		rgb[i] = (BYTE)val;
	}

}

void CAdvImg::applyAlphaBlend(CImage* img, double alpha, int r, int g, int b, bool ignoreAlphaChannel)
{
	const int imgWidth = img->GetWidth();
	const int imgHeight = img->GetHeight();
	const int imgPitch = img->GetPitch();
	const int imgBytePerPixel = img->GetBPP() / 8;
	BYTE* imgBits = (BYTE*)img->GetBits();

	const int width = this->GetWidth();
	const int height = this->GetHeight();
	const int pitch = this->GetPitch();
	const int bytePerPixel = this->GetBPP() / 8;
	BYTE* orgBits = (BYTE*)this->GetBits();

	const int minWidth = min(imgWidth, width);
	const int minHeight = min(imgHeight, height);

	const bool transparentColorNone = r < 0 || g < 0 || b < 0 || r > 0xff || g > 0xff || b > 0xff;
	const bool imgAlphaExists = imgBytePerPixel > 3;

	const DWORD RGB_MASK = 0x00FFFFFF;

	// if no transparent color and img Alpha does not Exists, just paste img to this.

	/*for (int w = 0; w < minWidth; ++w) {
	   for (int h = 0; h < minHeight; ++h) {

	   }
	}*/

	for (int w = 0; w < minWidth; ++w) {
		for (int h = 0; h < minHeight; ++h) {
			int imgPos = h * imgPitch + w * imgBytePerPixel;
			int pos = h * pitch + w * bytePerPixel;

			int r1 = imgBits[imgPos + 2];
			int g1 = imgBits[imgPos + 1];
			int b1 = imgBits[imgPos + 0];
			double a1 = -1;

			int r2 = orgBits[pos + 2];
			int g2 = orgBits[pos + 1];
			int b2 = orgBits[pos + 0];
			double a2 = -1;

			// if color is set to represent transparent
			if (r1 == r && g1 == g && b1 == b) {
				continue;
			}

			// if alphachannel is not available or just use alpha parameter
			if (imgBytePerPixel < 4 || ignoreAlphaChannel) {
				a1 = alpha;
			}
			else {
				a1 = imgBits[imgPos + 3];
			}

			if (bytePerPixel < 4) {
				a2 = 1.0;
			}
			else {
				a2 = orgBits[pos + 3] / 255.0;
			}

			double deno = a1 + a2 * (1 - a1);
			orgBits[pos + 2] = (BYTE)((r1 * a1 + r2 * a2 * (1 - a1)) / deno);
			orgBits[pos + 1] = (BYTE)((g1 * a1 + g2 * a2 * (1 - a1)) / deno);
			orgBits[pos + 0] = (BYTE)((b1 * a1 + b2 * a2 * (1 - a1)) / deno);

			if (bytePerPixel == 4) {
				orgBits[pos + 3] = (BYTE)((a1 + (1 - a1) * a2)*255.0);
			}
		}
	}
}

void CAdvImg::applyColorSwitch(BYTE r1, BYTE g1, BYTE b1, BYTE r2, BYTE g2, BYTE b2)
{
	const int width = this->GetWidth();
	const int height = this->GetHeight();
	const int pitch = this->GetPitch();
	const int bytePerPixel = this->GetBPP() / 8;
	BYTE* orgBits = (BYTE*)this->GetBits();

	for (int w = 0; w < width; ++w) {
		for (int h = 0; h < height; ++h) {
			int pos = h * pitch + w * bytePerPixel;
			int r = orgBits[pos + 2];
			int g = orgBits[pos + 1];
			int b = orgBits[pos + 0];

			if (r == r1 && g == g1 && b == b1) {
				orgBits[pos + 2] = r2;
				orgBits[pos + 1] = g2;
				orgBits[pos + 0] = b2;
			}
		}
	}
}

void CAdvImg::applyPixelization(int blockRowPixels, int blockColPixels)
{
	int w = GetWidth();
	int h = GetHeight();
	int pitch = GetPitch();
	int bytePerPixel = GetBPP() / 8;
	BYTE* orgBits = (BYTE*)GetBits();

	int blockCountRow = h / blockRowPixels;
	int blockCountCol = w / blockColPixels;
	if (h % blockRowPixels) ++blockCountRow;
	if (w % blockColPixels) ++blockCountCol;

	for (int br = 0; br < blockCountRow; ++br) {
		for (int bc = 0; bc < blockCountCol; ++bc) {
			int startPixelRow = blockRowPixels * br;
			int startPixelCol = blockColPixels * bc;
			int endPixelRow = blockRowPixels * (br + 1) > h ? h : blockRowPixels * (br + 1);
			int endPixelCol = blockColPixels * (bc + 1) > w ? w : blockColPixels * (bc + 1);
			int pixelCount = (endPixelRow - startPixelRow) * (endPixelCol - startPixelCol);
			double pixelsSum[3] = { 0.0, };
			for (int i = startPixelRow; i < endPixelRow; ++i) {
				for (int j = startPixelCol; j < endPixelCol; ++j) {
					for (int k = 0; k < 3; ++k) {
						pixelsSum[k] += orgBits[i * pitch + bytePerPixel * j + k];
					}
				}
			}

			for (int k = 0; k < 3; ++k) {
				pixelsSum[k] /= pixelCount;
			}

			for (int i = startPixelRow; i < endPixelRow; ++i) {
				for (int j = startPixelCol; j < endPixelCol; ++j) {
					for (int k = 0; k < 3; ++k) {
						orgBits[i * pitch + bytePerPixel * j + k] = (BYTE)pixelsSum[k];
					}
				}
			}
		}
	}
}