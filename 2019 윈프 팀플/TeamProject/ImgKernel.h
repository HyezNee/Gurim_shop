#pragma once
class ImgKernel {
public:
	// initializer

	ImgKernel();
	ImgKernel(int n_rows, int n_cols, bool isSeparated = false);
	template<typename T>
	ImgKernel(T** matrix, int n_rows, int n_cols);
	~ImgKernel();

	// kernel creation functions

	template<typename T>
	void createKernelFromArray(T** matrix, int n_rows, int n_cols);
	void createFull(int n_rows, int n_cols, double n = 0, bool isSeparated = false);
	void createIdentity(int n);
	void createGaussian(double stdev);

	void destroy();

	int getRowCount();
	int getColCount();


	// scalar calculation

	void multiply(double scalar);
	void multiplySepRowVec(double scalar);
	void multiplySepColVec(double scalar);

	//Getter, setter for matrix or vectors
	// functions using reference that enables access to kernel value.
	// these functions are not enable when kernel is separated.

	double& iloc(int ir, int ic);
	double& operator()(int row, int col);

	// For non-separated kernel, returns value in created matrix.
	// If kernel is separated, returns value calculated with rowvector and column vector.
	double get(int ir, int ic);

	template<typename T>
	void setSepRowVec(T* arr, int n);
	template<typename T>
	void setSepColVec(T* arr, int n);

	// returns _matrix
	double** getArray();

	double* getSepRowVec();
	double* getSepColVec();

	template <typename T>
	ImgKernel& operator*(T scalar);

	bool isSeparated();

private:
	bool _isCreated;
	int _n_rows, _n_cols;
	double** _matrix;

	bool _isSeparated; // if this is true, this kernel is separated and _matrix is NULL;
	double* _sepRowVec; // length of this row vector is _n_cols
	double* _sepColVec; // length of this column vector is _n_rows

	double gaussian(double stdev,int kernelLength,int pos);
};

template<typename T>
inline ImgKernel::ImgKernel(T** matrix, int n_rows, int n_cols) {
	this->createKernelFromArray(matrix, n_rows, n_cols);
}

template<typename T>
inline void ImgKernel::createKernelFromArray(T** matrix, int n_rows, int n_cols) {
	if (_isCreated) {
		_matrix = new double* [n_rows];
		for (int i = 0; i < n_cols; ++i) {
			_matrix[i] = new double[n_cols];
			for (int j = 0; j < n_cols; ++j) {
				_matrix[i][j] = (double)matrix[i][j];
			}
		}
	}
	else {
		//TODO : raise exception
	}
}

template<typename T>
inline ImgKernel& ImgKernel::operator*(T scalar)
{
	multiply(scalar);
	return this;
}

template<typename T>
inline void ImgKernel::setSepRowVec(T* arr, int n)
{
	assert(_isSeparated);

	if (_sepRowVec != 0)
		delete _sepRowVec;

	_n_cols = n;
	_sepRowVec = new double[n];
	memcpy_s(_sepRowVec, sizeof(double) * n, arr, sizeof(T)*n)
}

template<typename T>
inline void ImgKernel::setSepColVec(T* arr, int n)
{
	assert(_isSeparated);

	if (_sepColVec != 0)
		delete _sepColVec;

	_n_rows = n;
	_sepColVec = new double[n];
	memcpy_s(_sepColVec, sizeof(double) * n, arr, sizeof(T) * n)
}

