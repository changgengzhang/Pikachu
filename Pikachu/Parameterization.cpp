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
	// ============== boundary vertices ====================
	QVector<bool> findTag(m_vertexCount, false);
	QVector<uint> boundaryVertices;
	QVector<int> adjVVIndex, adjVVValue;
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
	
	// order the boundary vertex
	for (int count = 1; count < m_boundaryVertexCount; count++)
	{
		adjVVValue = m_adjacentVV.getOneRowValue(currVertexIndex);
		adjVVIndex = m_adjacentVV.getOneRowColIndex(currVertexIndex);

		for (uint pos = 0; pos < adjVVValue.count(); pos++)
		{
			vertexIndex = adjVVIndex.at(pos);
			currValue = adjVVValue.at(pos);
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

	// ============== Innaer vertices ====================
	for (uint index = 0; index < m_vertexCount; index++)
	{
		if (!m_boundaryVertexIndex.contains(index))
		{
			m_innerVertexIndex.append(index);
		}
	}
}


void Parameterization::boundaryVerticesParameterize(ParameterizationType boundaryType)
{
	assert(boundaryType == ParameterizationType::BOUNDARY_SQUARE || boundaryType == ParameterizationType::BOUNDARY_CIRCLE);
	
	this->calculateBoundaryLength();

	switch (boundaryType)
	{
	case zcg::BOUNDARY_SQUARE:
		float edgeLenth = 1.0f;
		for (uint i = 0; i < m_boundaryVertexCount; i++)
		{
			if (m_boundaryWeight[i] < 0.25)
			{
				m_boundaryVerticesResult.append(m_boundaryWeight.at(i) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (m_boundaryWeight[i] < 0.50)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
				m_boundaryVerticesResult.append((m_boundaryWeight.at(i) - 0.25) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (m_boundaryWeight[i] < 0.75)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (m_boundaryWeight.at(i) - 0.5) * 4 * edgeLenth);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (m_boundaryWeight[i] < 1.0)
			{
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (m_boundaryWeight.at(i) - 0.75) * 4 * edgeLenth);
				m_boundaryVerticesResult.append(0.0f);
			}
		}
		break;
	case zcg::BOUNDARY_CIRCLE:
		float radius = 0.5f;
		float circle = 2 * M_PI;
		for (uint i = 0; i< m_boundaryVertexCount; i++)
		{
			m_boundaryVerticesResult.append(radius * qCos(m_boundaryWeight[i] / m_boundaryLength));
			m_boundaryVerticesResult.append(radius * qSin(m_boundaryWeight[i] / m_boundaryLength));
			m_boundaryVerticesResult.append(0.0f);
		}
		break;
	default:
		break;
	}

}


void Parameterization::calculateBoundaryLength()
{
	m_boundaryLength = 0.0f;
	m_boundaryWeight.clear();
	float distance;
	int current, follow;

	for (int pos = 0; pos < m_boundaryVertexCount; pos++)
	{
		current = m_faceIndex.at(m_boundaryVertexIndex.at(pos % m_boundaryVertexCount));
		follow = m_faceIndex.at(m_boundaryVertexIndex.at((pos + 1) % m_boundaryVertexCount));
		distance = qPow(m_vertexPos.at(current * 3) - m_vertexPos.at(follow * 3), 2)
			+ qPow(m_vertexPos.at(current * 3 + 1) - m_vertexPos.at(follow * 3 + 1), 2)
			+ qPow(m_vertexPos.at(current * 3 + 2) - m_vertexPos.at(follow * 3 + 2), 2);
		distance = qSqrt(distance);
		m_boundaryLength += distance;

		m_boundaryWeight.append(m_boundaryLength);
	}
}