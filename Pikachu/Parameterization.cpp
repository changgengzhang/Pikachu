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


void Parameterization::calculate(ParameterizedType boundaryType)
{
	this->findBoundaryAndInnerVertices();
	this->boundaryVerticesParameterize(boundaryType);
	this->innerVerticesParameterize(ParameterizedType::INNER_AVERAGE);
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


void Parameterization::boundaryVerticesParameterize(ParameterizedType boundaryType)
{
	assert(boundaryType == ParameterizedType::BOUNDARY_SQUARE || boundaryType == ParameterizedType::BOUNDARY_CIRCLE);
	m_boundaryLength = 0.0f;
	
	// calculate boundary length and per vertex weight
	QVector<float> weight;
	float distance;
	int current, follow;

	for (int pos = 0; pos < m_boundaryVertexCount; pos++)
	{
		current = m_boundaryVertexIndex.at(pos % m_boundaryVertexCount);
		follow = m_boundaryVertexIndex.at((pos + 1) % m_boundaryVertexCount);
		distance = qPow(m_vertexPos.at(current * 3) - m_vertexPos.at(follow * 3), 2)
			+ qPow(m_vertexPos.at(current * 3 + 1) - m_vertexPos.at(follow * 3 + 1), 2)
			+ qPow(m_vertexPos.at(current * 3 + 2) - m_vertexPos.at(follow * 3 + 2), 2);
		distance = qSqrt(distance);
		m_boundaryLength += distance;

		weight.append(m_boundaryLength);
	}

	for (int pos = 0; pos < m_boundaryVertexCount; pos++)
	{
		weight[pos] /= m_boundaryLength;
	}

	// vertex parameterize
	switch (boundaryType)
	{
	case zcg::BOUNDARY_SQUARE:
	{
		float edgeLenth = 1.0f;
		for (uint i = 0; i < m_boundaryVertexCount; i++)
		{
			if (weight[i] < 0.25)
			{
				m_boundaryVerticesResult.append(weight.at(i) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (weight[i] < 0.50)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
				m_boundaryVerticesResult.append((weight.at(i) - 0.25) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (weight[i] < 0.75)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (weight.at(i) - 0.5) * 4 * edgeLenth);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(0.0f);
			}
			else if (weight[i] <= 1.0)
			{
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (weight.at(i) - 0.75) * 4 * edgeLenth);
				m_boundaryVerticesResult.append(0.0f);
			}
		}
		break;
	}
	case zcg::BOUNDARY_CIRCLE:
	{
		float radius = 0.5f;
		float circle = 2 * M_PI;
		for (uint i = 0; i < m_boundaryVertexCount; i++)
		{
			m_boundaryVerticesResult.append(radius * qCos(weight[i] / m_boundaryLength));
			m_boundaryVerticesResult.append(radius * qSin(weight[i] / m_boundaryLength));
			m_boundaryVerticesResult.append(0.0f);
		}
		break;
	}
	default:
		break;
	}
}


void Parameterization::innerVerticesParameterize(ParameterizedType innerType)
{
	assert(innerType == ParameterizedType::INNER_AVERAGE || innerType == ParameterizedType::INNER_SHAP_PRESERVING);

	int innerCount = m_innerVertexIndex.count();
	int boundaryCount = m_boundaryVertexIndex.count();
	int totalCount = innerCount + boundaryCount;

	// A*u = b
	// A*u.x = b.x
	// A*u.y = b.y
	Eigen::MatrixXf matrixA = Eigen::MatrixXf::Zero(innerCount, innerCount);
	Eigen::MatrixXf matrixB = Eigen::MatrixXf::Zero(innerCount, 2);

	// initialize A
	for (int i = 0; i < innerCount; i++)
	{
		matrixA(i, i) = 1.0f;
	}

	glm::vec2 b(0.0f);

	for (int i = 0; i < innerCount; i++)
	{
		int vertexIndex = m_innerVertexIndex.at(i);
		int neighborCount = m_adjacentVV.getOneRowElemNum(vertexIndex);
		QVector<int> neighborVertices = m_adjacentVV.getOneRowColIndex(vertexIndex);

		QVector<float> *myu;
		switch (innerType)
		{
		case zcg::INNER_AVERAGE:
		{
			myu = new QVector<float>;
			for (int w = 0; w < neighborCount; w++)
			{
				myu->append(1.0f / neighborCount);
			}
			break;
		}
		case zcg::INNER_SHAP_PRESERVING:
		{

			break;
		}
		default:
			break;
		}

		// 
		int index;
		for (int j = 0; j < neighborCount; j++)
		{
			int neighborIndex = neighborVertices.at(j);

			// 判断这个邻接点是内部点还是边界点
			index = m_innerVertexIndex.indexOf(neighborIndex);

			if (index == -1)
			{
				// 边界点
				index = m_boundaryVertexIndex.indexOf(neighborIndex);
				if (index == -1)
				{
					qFatal("Boooooommmmmm");
				}
				else
				{
					b.x += myu->at(j) * m_boundaryVerticesResult.at(index * 3);
					b.y += myu->at(j) * m_boundaryVerticesResult.at(index * 3 + 1);
				}
			}
			else
			{
				// 内部点
				matrixA(i, index) = -myu->at(j);
			}
		}
		matrixB(i, 0) = b.x;
		matrixB(i, 1) = b.y;
		b = glm::vec2(0.0f);
	}

	Eigen::MatrixXf u = matrixA.colPivHouseholderQr().solve(matrixB);

	
	QString tmp;

	for (int i = 0; i < matrixA.size(); i++)
	{
		if (i % matrixA.cols() == 0)
		{
			tmp += "\n";
		}
		tmp += QString::number(matrixA.data()[i]);
		tmp += " ";
	}
	qInfo() << tmp.toStdString().c_str();

	tmp.clear();
	for (int i = 0; i < matrixB.size(); i++)
	{
		if (i % matrixB.cols() == 0)
		{
			tmp += "\n";
		}
		tmp += QString::number(matrixB.data()[i]);
		tmp += " ";
	}
	qInfo() << tmp.toStdString().c_str();

	tmp.clear();
	for (int i = 0; i < u.size(); i += 2)
	{
		tmp += QString::number(u.data()[i]);
		tmp += " ";
		tmp += QString::number(u.data()[i + 1]);
		tmp += "\n";
	}
	
	qInfo() << tmp.toStdString().c_str();
}