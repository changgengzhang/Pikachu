#include "Parameterization.h"

Parameterization::Parameterization(const QVector<float> &vertexPos, const QVector<uint> &faceIndex,
	const QVector<bool> &isBoundary, const SparseMatrix<int> &adjacentVV, const int boundaryVertexCount)
	: m_vertexPos(vertexPos), m_faceIndex(faceIndex), m_isBoundary(isBoundary), m_adjacentVV(adjacentVV), m_boundaryVertexCount(boundaryVertexCount)
{
	m_vertexCount = m_vertexPos.count() / 3;
	m_faceCount = m_faceIndex.count() / 3;
}

Parameterization::~Parameterization()
{
}


void Parameterization::calculate(ParameterizationBoundaryType boundaryType, ParameterizationInnerType innerType)
{
	this->findBoundaryAndInnerVertices();
	this->boundaryVerticesParameterize(boundaryType);
	this->innerVerticesParameterize(innerType);
	this->mergeBoundaryAndInnerParameterizedResult();
	this->dumpToObjeFile();
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
		m_boundaryVertexIndices.append(boundaryVertices.at(i));
	}

	// ============== Innaer vertices ====================
	for (uint index = 0; index < m_vertexCount; index++)
	{
		if (!m_boundaryVertexIndices.contains(index))
		{
			m_innerVertexIndices.append(index);
		}
	}
}


void Parameterization::boundaryVerticesParameterize(ParameterizationBoundaryType boundaryType)
{
	assert(boundaryType == ParameterizationBoundaryType::SQUARE || boundaryType == ParameterizationBoundaryType::CIRCLE);
	m_boundaryLength = 0.0f;
	
	// calculate boundary length and per vertex weight
	QVector<float> weight;
	float distance;
	int current, follow;

	for (int pos = 0; pos < m_boundaryVertexCount; pos++)
	{
		current = m_boundaryVertexIndices.at(pos % m_boundaryVertexCount);
		follow = m_boundaryVertexIndices.at((pos + 1) % m_boundaryVertexCount);
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
	case ParameterizationBoundaryType::SQUARE:
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
	case ParameterizationBoundaryType::CIRCLE:
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


void Parameterization::innerVerticesParameterize(ParameterizationInnerType innerType)
{
	assert(innerType == ParameterizationInnerType::AVERAGE || innerType == ParameterizationInnerType::SHAP_PRESERVING);

	int innerCount = m_innerVertexIndices.count();
	int boundaryCount = m_boundaryVertexIndices.count();
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
		int vertexIndex = m_innerVertexIndices.at(i);
		int neighborCount = m_adjacentVV.getOneRowElemNum(vertexIndex);
		QVector<int> neighborVertexIndices = m_adjacentVV.getOneRowColIndex(vertexIndex);

		QVector<float> matrixCofficient;
		switch (innerType)
		{
		case ParameterizationInnerType::AVERAGE:
		{
			for (int w = 0; w < neighborCount; w++)
			{
				matrixCofficient.append(1.0f / neighborCount);
			}
			break;
		}
		case ParameterizationInnerType::SHAP_PRESERVING:
		{
			QVector<float> edgeWeight;
			float totalLength = 0.0f, edgeLenght;

			for (int w = 0; w < neighborCount; w++)
			{
				edgeLenght = 1.0f / qSqrt(
					qPow(m_vertexPos.at(vertexIndex * 3) - m_vertexPos.at(neighborVertexIndices.at(w) * 3), 2)
					+ qPow(m_vertexPos.at(vertexIndex * 3 + 1) - m_vertexPos.at(neighborVertexIndices.at(w) * 3 + 1), 2)
					+ qPow(m_vertexPos.at(vertexIndex * 3 + 2) - m_vertexPos.at(neighborVertexIndices.at(w) * 3 + 2), 2));

				edgeWeight.append(edgeLenght);
				totalLength += edgeLenght;
			}

			for (int w = 0; w < neighborCount; w++)
			{
				matrixCofficient.append(edgeWeight[w] / totalLength);
			}

			break;
		}
		default:
			break;
		}

		// 
		int index;
		for (int j = 0; j < neighborCount; j++)
		{
			int neighborIndex = neighborVertexIndices.at(j);

			// 判断这个邻接点是内部点还是边界点
			index = m_innerVertexIndices.indexOf(neighborIndex);

			if (index == -1)
			{
				// 边界点
				index = m_boundaryVertexIndices.indexOf(neighborIndex);
				if (index == -1)
				{
					qFatal("Boooooommmmmm");
				}
				else
				{
					b.x += matrixCofficient.at(j) * m_boundaryVerticesResult.at(index * 3);
					b.y += matrixCofficient.at(j) * m_boundaryVerticesResult.at(index * 3 + 1);
				}
			}
			else
			{
				// 内部点 叠加？
				matrixA(i, index) = -matrixCofficient.at(j);
			}
		}
		matrixB(i, 0) = b.x;
		matrixB(i, 1) = b.y;
		b = glm::vec2(0.0f);
	}

	Eigen::MatrixXf u = matrixA.colPivHouseholderQr().solve(matrixB);

	for (int i = 0; i < u.rows(); i++)
	{
		m_innerVerticesResult.append(u(i, 0));
		m_innerVerticesResult.append(u(i, 1));
		m_innerVerticesResult.append(0);
	}
}


void Parameterization::mergeBoundaryAndInnerParameterizedResult()
{
	int index;
	m_parameterizedResult.resize(m_vertexCount * 3);
	
	for (int i = 0; i < m_boundaryVertexIndices.count(); i++)
	{
		index = m_boundaryVertexIndices.at(i);
		m_parameterizedResult[index * 3] = m_boundaryVerticesResult[i * 3];
		m_parameterizedResult[index * 3 + 1] = m_boundaryVerticesResult[i * 3 + 1];
		m_parameterizedResult[index * 3 + 2] = m_boundaryVerticesResult[i * 3 + 2];
	}

	for (int i = 0; i < m_innerVertexIndices.count(); i++)
	{
		index = m_innerVertexIndices.at(i);
		m_parameterizedResult[index * 3] = m_innerVerticesResult[i * 3];
		m_parameterizedResult[index * 3 + 1] = m_innerVerticesResult[i * 3 + 1];
		m_parameterizedResult[index * 3 + 2] = m_innerVerticesResult[i * 3 + 2];
	}
}


void Parameterization::dumpToObjeFile()
{
	m_dumpFileName = "param.obj";
	QFile dumpFile(m_dumpFileName);
	if (!dumpFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		return;
	}

	QTextStream dumpStream(&dumpFile);

	dumpStream << "# parameterization result\n";
	
	// vertex
	for (int i = 0; i < m_vertexCount; i++)
	{
		dumpStream << "v " << m_parameterizedResult[i * 3] << " " << m_parameterizedResult[i * 3 + 1] << " " << m_parameterizedResult[i * 3 + 2] << "\n";
	}

	// face
	for (int i = 0; i < m_faceCount; i++)
	{
		dumpStream << "f " << m_faceIndex[i * 3] + 1 << " " << m_faceIndex[i * 3 + 1] + 1 << " " << m_faceIndex[i * 3 + 2] + 1 << "\n";
	}

	dumpFile.close();

}