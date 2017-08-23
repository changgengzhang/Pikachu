#ifndef PARAMETERIZATION_H
#define PARAMETERIZATION_H

#include <QObject>
#include <qvector.h>
#include <qdebug.h>
#include <qmath.h>

#include "SparseMatrix.h"
#include "Util.h"
using namespace zcg;

class Parameterization : public QObject
{
	Q_OBJECT

public:
	Parameterization(const QVector<float> &vertexPos, const QVector<uint> &faceIndex, 
					const QVector<bool> &isBoundary, const SparseMatrix<int> &adjacentVV, const int boundaryVertexCount);
	~Parameterization();
	

private:
	// ========== function =============
	void findBoundaryAndInnerVertices();
	void boundaryVerticesParameterize(ParameterizationType boundaryType);
	void calculateBoundaryLength();

	// ========= values ===============
	int m_vertexCount;
	int m_faceCount;
	int m_boundaryVertexCount;
	float m_boundaryLength;


	const QVector<float> &m_vertexPos;
	const QVector<uint> &m_faceIndex;
	const QVector<bool> &m_isBoundary;
	const SparseMatrix<int> &m_adjacentVV;


	QVector<uint> m_boundaryVertexIndex;
	QVector<float> m_boundaryVerticesResult;
	QVector<uint> m_innerVertexIndex;
	QVector<float> m_boundaryWeight;

};

#endif // !PARAMETERIZATION_H