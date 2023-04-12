
#include "stdafx.h"
#include "ImgKernel.h"


ImgKernel::ImgKernel(){
	_n_rows = 0;
	_n_cols = 0;
	_matrix = 0;
	_sepRowVec = 0;
	_sepColVec = 0;
	_isSeparated = false;
	_isCreated = false;
}

ImgKernel::ImgKernel(int n_rows, int n_cols, bool isSeparated){
	_n_rows = n_rows;
	_n_cols = n_cols;
	_isSeparated = isSeparated;
	_matrix = 0;
	_isCreated = false;
	_sepRowVec = 0;
	_sepColVec = 0;
	createFull(n_rows,n_cols,isSeparated);
}


ImgKernel::~ImgKernel(){
	destroy();
}

void ImgKernel::createFull(int n_rows, int n_cols,double n, bool isSeparated){
	
	if(!_isCreated){
		_isCreated = true;
		_n_rows = n_rows;
		_n_cols = n_cols;

		if (isSeparated) {
			_sepRowVec = new double[n_cols];
			_sepColVec = new double[n_rows];
			for (int i = 0; i < n_cols; ++i) _sepRowVec[i] = n;
			for (int i = 0; i < n_rows; ++i) _sepColVec[i] = n;
		}
		else {
			_matrix = new double* [n_rows];
			for (int i = 0; i < n_rows; ++i) {
				_matrix[i] = new double[n_cols];
				for (int j = 0; j < n_cols; ++j) {
					_matrix[i][j] = n;
				}
			}
		}
		
	} else{
		//TODO : raise exception
	}
}



void ImgKernel::createIdentity(int n){
	if(!_isCreated){
		_isCreated = true;
		_isSeparated = false;
		_n_rows = _n_cols = n;
		_matrix = new double*[n];

		for(int i = 0; i < n; ++i){
			_matrix[i] = new double[n]();
			_matrix[i][i] =1;
		}
	} else{
		//TODO : raise exception
	}
	
}

void ImgKernel::createGaussian(double stdev)
{
	if (!_isCreated) {
		_isCreated = true;
		_isSeparated = true;

		int len = 6 * stdev;
		if (!(len % 2)) ++len;

		_n_rows = _n_cols = len;

		_sepRowVec = new double[len];
		_sepColVec = new double[len];

		double value = 0;
		double var = stdev * stdev;
		int center = len / 2;
		int dx = 0;
		double precon = 1.0/(pow(2 * 3.141592, 0.5) * stdev);
		

		for (int i = 0; i < center; ++i) {
			dx = center - i;
			value = exp(-(dx * dx) / (2 * var)) * precon;
			_sepRowVec[i] = value;
			_sepRowVec[len-i-1] = value;
			_sepColVec[i] = value;
			_sepColVec[len-i-1] = value;
		}
		_sepRowVec[center] = precon;
		_sepColVec[center] = precon;
	}
	else {

	}
}

void ImgKernel::destroy()
{
	if (!_isCreated) return;

	if (_isSeparated) {
		delete[] _sepColVec;
		delete[] _sepRowVec;
		_isSeparated = false;
	}
	else {
		for (int i = 0; i < _n_rows; ++i) {
			delete[] _matrix[i];
		}
		delete _matrix;
	}

	_n_cols = 0;
	_n_rows = 0;
	_isCreated = false;
}

double& ImgKernel::iloc(int ir, int ic){
	/*if(ir < 0 || ic < 0 || ir >= _n_rows || ic >= _n_rows){
		throw "Out of index";
	}*/
	assert(!_isSeparated);
	return _matrix[ir][ic];
}

int ImgKernel::getRowCount(){
	return _n_rows;
}

int ImgKernel::getColCount(){
	return _n_cols;
}

double ImgKernel::get(int ir, int ic)
{
	if (!_isSeparated) {
		return _matrix[ir][ic];
	}
	else {
		return _sepColVec[ir] * _sepRowVec[ic];
	}
}


double ** ImgKernel::getArray(){
	assert(!_isSeparated);
	return _matrix;
}

double* ImgKernel::getSepRowVec()
{
	return _sepRowVec;
}

double* ImgKernel::getSepColVec()
{
	return _sepColVec;
}

bool ImgKernel::isSeparated()
{
	return _isCreated && _isSeparated;
}

double ImgKernel::gaussian(double stdev, int kernelLength, int pos)
{
	int dx = kernelLength / 2 - pos;
	double var = stdev * stdev;
	if (dx > 0) dx -= dx;

	return exp(-(dx * dx) / (2 * var)) / (2 * 3.141592 * var);
}

void ImgKernel::multiply(double scalar){

	if (!_isSeparated) {
		for (int i = 0; i < _n_rows; ++i) {
			for (int j = 0; j < _n_cols; ++j) {
				_matrix[i][j] *= scalar;
			}
		}
	}
	else {
		multiplySepColVec(scalar);
		multiplySepRowVec(scalar);
	}
	
}

void ImgKernel::multiplySepRowVec(double scalar)
{
	assert(_sepRowVec != 0);
	for (int i = 0; i < _n_cols; ++i) {
		_sepRowVec[i] *= scalar;
	}
}

void ImgKernel::multiplySepColVec(double scalar)
{
	assert(_sepRowVec != 0);
	for (int i = 0; i < _n_rows; ++i) {
		_sepColVec[i] *= scalar;
	}
}

double & ImgKernel::operator()(int ir, int ic){
	return this->iloc(ir,ic);
}
