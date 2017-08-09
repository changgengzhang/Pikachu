#include "SparseMatrix.h"


template<typename T>
SparseMatrix<T>::SparseMatrix(int n)
{
	this->construct(n, n);
}

template<typename T>
SparseMatrix<T>::SparseMatrix(int rows, int columns)
{
	this->construct(rows, columns);
}

template<typename T>
SparseMatrix<T>::~SparseMatrix()
{

}

template<typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> & m)
{

}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator = (const SparseMatrix<T> & m)
{

}

template<typename T>
void SparseMatrix<T>::construct(int rows, int columns)
{
	if (rows < 1 || columns < 1) {
		throw InvalidDimensionsException("Matrix dimensions cannot be zero or negative.");
	}

	m_numRows = rows;
	m_numCols = columns;

	this->initVector(m_rowPtr, 0, m_numRows + 1);
}


template<typename T>
void SparseMatrix<T>::initVector(QVector<T> &v, T value, uint size)
{
	v.clear();
	for (uint i = 0; i < size; i++)
	{
		v.append(value);
	}
}

template<typename T>
void SparseMatrix<T>::validateCoordinates(int row, int col) const
{
	if (row < 1 || col < 1 || row > m_numRows || col > m_numCols) {
		throw InvalidCoordinatesException("Coordinates out of range.");
	}
}
