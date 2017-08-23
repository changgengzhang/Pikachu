#include "Parameterization.h"

Parameterization::Parameterization(const QVector<float> &vertexPos, const QVector<uint> &faceIndex,
	const QVector<bool> &isBoundary, const SparseMatrix<int> &adjacentVV, const int boundaryVertexCount)
	: m_vertexPos(vertexPos), m_faceIndex(faceIndex), m_isBoundary(isBoundary), m_adjacentVV(adjacentVV), m_boundaryVertexCount(boundaryVertexCount)
{
	m_vertexCount = m_vertexPos.count() / 3;

}

Parameterization::~Parameterization()
{
}


void Parameterization::findBoundaryAndInnerVertices()
{
	QVector<bool> findTag(m_vertexCount, false);
	QVector<uint> boundaryVertices;
	QVector<int> adjVIndex, adjVValue;
	uint currVertexIndex, vertexIndex;
	int currValue;
	// find the start vertex of boundary
	for (uint pos = 0; pos < m_vertexCount; pos++)
	{
		if (m_isBoundary.at(pos))
		{
			boundaryVertices.append(pos);
			currVertexIndex = pos;
			findTag[pos] = true;
			break;
		}
	}
	
	for (int count = 1; count < m_boundaryVertexCount; count++)
	{
		adjVValue = m_adjacentVV.getOneRowValue(currVertexIndex);
		adjVIndex = m_adjacentVV.getOneRowColIndex(currVertexIndex);

		for (uint pos = 0; pos < adjVValue.count(); pos++)
		{
			vertexIndex = adjVIndex.at(pos);
			currValue = adjVValue.at(pos);
			if (currValue == 1 && !findTag[vertexIndex] && m_isBoundary.at(vertexIndex))
			{
				// 如果选中的点有和当前的点之间都有一条指向各自的有向边，那么舍弃这个点。
				if (m_adjacentVV.get(vertexIndex, currVertexIndex) != 1)
				{
					currVertexIndex = vertexIndex;
					boundaryVertices.append(currVertexIndex);
					findTag[currVertexIndex] = true;
					break;
				}
			}
		}
	}
	
	for (uint i = 0; i < boundaryVertices.count(); i++)
	{
		m_boundaryVertexIndex.append(boundaryVertices.at(i));
	}
	
	
	

}