#ifndef PARAMETERIZATION_H
#define PARAMETERIZATION_H

// =========== Qt =============
#include <QObject>
#include <qvector.h>
#include <qdebug.h>
#include <qmath.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
 
// =========== Eigen ===========
#include <Eigen\Dense>

#include "SparseMatrix.h"
#include "Util.h"

using namespace zcg;

class Parameterization : public QObject
{
	Q_OBJECT

public:
	Parameterization(const QVector<float> &vertexPos, const QVector<uint> &faceIndex, 
					const QVector<bool> &isBoundary, const SparseMatrix<int> *adjacentVV, const int boundaryVertexCount);
	//Parameterization(Mesh &mesh);

	~Parameterization();
	void calculate(ZVALUE boundaryType, ZVALUE innerType);
	void dumpToObjeFile(QString fileName) const;
	const QVector<float>& getParameterizedResult() const;

private:
	// ========== function =============
	void findBoundaryAndInnerVertices();
	void boundaryVerticesParameterize(ZVALUE boundaryType);
	void innerVerticesParameterize(ZVALUE innerType);
	void mergeBoundaryAndInnerParameterizedResult();
	
	void ComputeShapPreservingWeight(int currIndex, QVector<float> &vertexWeight);
	bool sortNeighborVertex(QVector<int> &neighborVertexIndices, QVector<int> &localNeighborVertexIndices, int index);

	// ========= values ===============
	int m_vertexCount;
	int m_faceCount;
	int m_boundaryVertexCount;
	float m_boundaryLength;

	const QVector<float> &m_vertexPos;
	const QVector<uint> &m_faceIndex;
	const QVector<bool> &m_isBoundary;
	const SparseMatrix<int> *m_adjacentVV;

	QVector<uint> m_boundaryVertexIndices;
	QVector<float> m_boundaryVerticesResult;
	QVector<uint> m_innerVertexIndices;
	QVector<float> m_innerVerticesResult;
	QVector<float> m_parameterizedResult;

};

#endif // !PARAMETERIZATION_H