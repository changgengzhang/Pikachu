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
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> & m)
{
	this->copyData(m);
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator = (const SparseMatrix<T> & m)
{
	if (&m != this)
	{
		this->destruct();
		this->copyData();
	}
	return *this;
}

template<typename T>
SparseMatrix<T>::~SparseMatrix()
{
	m_value.freeData();
	m_colIndex.freeData();
	m_rowPtr.freeData();
}

// ================== priavte values get ===============
template<typename T>
int SparseMatrix<T>::getRowCount() const
{
	return m_numRows;
}

template<typename T>
int SparseMatrix<T>::getColCount() const
{
	return m_numCols;
}

template<typename T>
int SparseMatrix<T>::getNonzeroCount() const
{
	return m_numNonzero;
}

// =================== set get exist value ==============

template<typename T>
T SparseMatrix<T>::get(int row, int col) const
{
	this->validateCoordinates(row, col);

	int currCol = 0;
	int pos = m_rowPtr.at(row - 1);

	for (; pos < m_rowPtr.at(row); pos++)
	{
		currCol = m_colIndex.at(pos);

		if (currCol == col)
		{
			return  m_value.at(pos);
		}
		else if (currCol > col)
		{
			break;
		}
	}
	return T();
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::set(T value, int row, int col)
{
	this->validateCoordinates(row, col);

	int currCol = 0;
	int pos = m_rowPtr.at(row - 1);

	for (; pos < m_rowPtr.at(row); pos++)
	{
		currCol = m_colIndex.at(pos);
		if (currCol >= col)
		{
			break;
		}
	}

	if (currCol == col)
	{
		if (value == T())
		{
			// remove this value
			m_value.erase(m_value.begin() + pos);
			m_colIndex.erase(m_value.begin() + pos);
			// update row ptr
			for (int i = row; i <= m_numRows; i++)
			{
				m_rowPtr[i] -= 1;
			}
		}
		else
		{
			m_value[pos] = value;
		}
		    
	}
	else
	{
		m_value.insert(m_value.begin() + pos, value);
		m_colIndex.insert(m_colIndex.begin() + pos, col);

		// update row ptr
		for (int i = row; i <= m_numRows; i++)
		{
			m_rowPtr[i] += 1;
		}
	}
	return *this;
}

template<typename T>
bool SparseMatrix<T>::isExist(int row, int col) const
{
	this->validateCoordinates(row, col);

	int currCol = 0;
	int pos = m_rowPtr.at(row - 1);

	for (; pos < m_rowPtr.at(row); pos++)
	{
		currCol = m_colIndex.at(pos);

		if (currCol == col)
		{
			return  true;
		}
		else if (currCol > col)
		{
			break;
		}
	}
	return false;
}


// ===================  assistant function ==============

template<typename T>
void SparseMatrix<T>::construct(int rows, int columns)
{
	if (rows < 1 || columns < 1)
	{
		throw InvalidDimensionsException("Matrix dimensions cannot be zero or negative.");
	}

	m_numRows = rows;
	m_numCols = columns;
	m_numNonzero = 0;

	this->initVector(m_rowPtr, 0, m_numRows + 1);
}


template<typename T>
void SparseMatrix<T>::copyData(const SparseMatrix<T> &m)
{
	m_numRows = m.m_numRows;
	m_numCols = m.m_numCols;
	m_numNonzero = m.m_numNonzero;

	m_value = m.m_value;
	m_colIndex = m.m_value;
	m_rowPtr = m.m_rowPtr;
}

template<typename T>
void SparseMatrix<T>::destruct()
{
	m_value.freeData();
	m_colIndex.freeData();
	m_rowPtr.freeData();

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
