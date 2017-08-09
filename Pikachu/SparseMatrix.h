#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <qvector.h>

#include "Exceptions.h"

template<typename T>
class SparseMatrix
{
public:
	SparseMatrix(int n);
	SparseMatrix(int rows, int columns);
	SparseMatrix(const SparseMatrix<T> & m);
	SparseMatrix<T>& operator = (const SparseMatrix<T> & m);
	~SparseMatrix();

private:
	void construct(int rows, int columns);


	void validateCoordinates(int row, int col) const
	void initVector(QVector<T> &v, T value, uint size);
private:
	int m_numRows;
	int m_numCols;

	QVector<T> m_value;
	QVector<int> m_rowPtr;
	QVector<int> m_colIndex;

};


#endif // !SPARSE_MATRIX_H
