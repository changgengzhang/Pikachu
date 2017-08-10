#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <qvector.h>
#include "Exceptions.h"

template<typename T>
class SparseMatrix
{
public:
	// ========== construct and destruct ===============
	SparseMatrix(int n);
	SparseMatrix(int rows, int columns);
	SparseMatrix(const SparseMatrix<T> & m);
	~SparseMatrix();

	// ========== operation overload ==================
	SparseMatrix<T>& operator = (const SparseMatrix<T> & m);
	
	// ========= priavte values get ==============
	int getRowCount() const;
	int getColCount() const;
	int getNonzeroCount() const;

	// ======== set get exist value =====================
	T get(int row, int col) const;
	SparseMatrix<T>& set(T value, int row, int col);
	bool isExist(int row, int col) const;

private:
	// ========== assistant function  ===============
	void construct(int rows, int columns);
	void copyData(const SparseMatrix<T> &m);
	void destruct();
	void initVector(QVector<T> &v, T value, uint size);

	// ========== parameter valid check ============
	void validateCoordinates(int row, int col) const;
	

private:
	// ========== values ==========================
	int m_numRows;
	int m_numCols;
	int m_numNonzero;

	QVector<T> m_value;
	QVector<int> m_colIndex;
	QVector<int> m_rowPtr;

};


#endif // !SPARSE_MATRIX_H
