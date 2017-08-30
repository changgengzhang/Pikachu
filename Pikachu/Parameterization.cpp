#include "Parameterization.h"

Parameterization::Parameterization(const QVector<float> &vertexPos, const QVector<uint> &faceIndex,
	const QVector<bool> &isBoundary, const SparseMatrix<int> *adjacentVV, const int boundaryVertexCount)
	: m_vertexPos(vertexPos), m_faceIndex(faceIndex), m_isBoundary(isBoundary), m_adjacentVV(adjacentVV), m_boundaryVertexCount(boundaryVertexCount)
{
	m_vertexCount = m_vertexPos.count() / 3;
	m_faceCount = m_faceIndex.count() / 3;
}


Parameterization::~Parameterization()
{
}


void Parameterization::calculate(ZVALUE boundaryType, ZVALUE innerType)
{
	this->findBoundaryAndInnerVertices();
	this->boundaryVerticesParameterize(boundaryType);
	this->innerVerticesParameterize(innerType);
	this->mergeBoundaryAndInnerParameterizedResult();
}


void Parameterization::dumpToObjeFile(QString fileName) const
{
	QFile dumpFile(fileName);
	if (!dumpFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		return;
	}

	QTextStream dumpStream(&dumpFile);

	dumpStream << "# parameterization result\n";

	// vertex
	for (int i = 0; i < m_vertexCount; i++)
	{
		dumpStream << "v " << m_parameterizedResult[i * 2] << " " << m_parameterizedResult[i * 2 + 1] << " " << 0.0 << "\n";
	}

	// face
	for (int i = 0; i < m_faceCount; i++)
	{
		dumpStream << "f " << m_faceIndex[i * 3] + 1 << " " << m_faceIndex[i * 3 + 1] + 1 << " " << m_faceIndex[i * 3 + 2] + 1 << "\n";
	}

	dumpFile.close();

}


const QVector<float>& Parameterization::getParameterizedResult() const
{
	return m_parameterizedResult;
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
		adjVVValue = m_adjacentVV->getOneRowValue(currVertexIndex);
		adjVVIndex = m_adjacentVV->getOneRowColIndex(currVertexIndex);

		for (uint pos = 0; pos < adjVVValue.count(); pos++)
		{
			vertexIndex = adjVVIndex.at(pos);
			currValue = adjVVValue.at(pos);
			if (currValue == 1 && !findTag[vertexIndex] && m_isBoundary.at(vertexIndex))
			{
				// 如果选中的点有和当前的点之间都有一条指向各自的有向边，那么舍弃这个点。
				if (m_adjacentVV->get(vertexIndex, currVertexIndex) != 1)
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


void Parameterization::boundaryVerticesParameterize(ZVALUE boundaryType)
{
	assert(boundaryType == Z_SQUARE || boundaryType == Z_CIRCLE);
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
	case Z_SQUARE:
	{
		float edgeLenth = 1.0f;
		for (uint i = 0; i < m_boundaryVertexCount; i++)
		{
			if (weight[i] <= 0.25)
			{
				m_boundaryVerticesResult.append(weight.at(i) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
			}
			else if (weight[i] <= 0.50)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
				m_boundaryVerticesResult.append((weight.at(i) - 0.25) * 4 * edgeLenth - edgeLenth / 2.0f);
			}
			else if (weight[i] <= 0.75)
			{
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (weight.at(i) - 0.5) * 4 * edgeLenth);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f);
			}
			else if (weight[i] <= 1.0)
			{
				m_boundaryVerticesResult.append(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.append(edgeLenth / 2.0f - (weight.at(i) - 0.75) * 4 * edgeLenth);
			}
		}
		break;
	}
	case Z_CIRCLE:
	{
		float radius = 0.5f;
		float circle = 2 * M_PI;
		for (uint i = 0; i < m_boundaryVertexCount; i++)
		{
			m_boundaryVerticesResult.append(radius * qCos(weight[i] / m_boundaryLength));
			m_boundaryVerticesResult.append(radius * qSin(weight[i] / m_boundaryLength));
		}
		break;
	}
	default:
		break;
	}

	//QFile dumpFile("./boundary.txt");

	//if (!dumpFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//{
	//	return;
	//}

	//QTextStream dumpStream(&dumpFile);

	//for (uint i = 0; i < m_boundaryVerticesResult.count(); i++)
	//{
	//	dumpStream.setRealNumberPrecision(10);

	//	if (i % 2 == 0)
	//	{
	//		dumpStream << m_boundaryVerticesResult.at(i) << ",";
	//	}
	//	else
	//	{
	//		dumpStream << m_boundaryVerticesResult.at(i) << "\n";
	//	}
	//}
}


void Parameterization::innerVerticesParameterize(ZVALUE innerType)
{
	assert(innerType == Z_UNIFORM || innerType == Z_SHAP_PRESERVING);

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
		int neighborCount = m_adjacentVV->getOneRowElemNum(vertexIndex);
		QVector<int> neighborVertexIndices = m_adjacentVV->getOneRowColIndex(vertexIndex);

		QVector<float> vertexWeight;
		switch (innerType)
		{
		case Z_UNIFORM:
		{
			for (int w = 0; w < neighborCount; w++)
			{
				vertexWeight.append(1.0f / neighborCount);
			}
			break;
		}
		case Z_SHAP_PRESERVING:
		{
			vertexWeight.resize(neighborCount);
			this->ComputeShapPreservingWeight(vertexIndex, vertexWeight);
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
					b.x += vertexWeight.at(j) * m_boundaryVerticesResult.at(index * 2);
					b.y += vertexWeight.at(j) * m_boundaryVerticesResult.at(index * 2 + 1);
				}
			}
			else
			{
				// 内部点 叠加？
				matrixA(i, index) -= vertexWeight.at(j);
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
	}
}



void Parameterization::mergeBoundaryAndInnerParameterizedResult()
{
	int index, step;

	m_parameterizedResult.resize(m_vertexCount * 2);
	m_parameterizedResult.squeeze();
	
	for (int i = 0; i < m_boundaryVertexIndices.count(); i++)
	{
		index = m_boundaryVertexIndices.at(i);
		m_parameterizedResult[index * 2] = m_boundaryVerticesResult[i * 2];
		m_parameterizedResult[index * 2 + 1] = m_boundaryVerticesResult[i * 2 + 1];
	}

	for (int i = 0; i < m_innerVertexIndices.count(); i++)
	{
		index = m_innerVertexIndices.at(i);
		m_parameterizedResult[index * 2] = m_innerVerticesResult[i * 2];
		m_parameterizedResult[index * 2 + 1] = m_innerVerticesResult[i * 2 + 1];
	}
}


void Parameterization::ComputeShapPreservingWeight(int currIndex, QVector<float> &vertexWeight)
{
	int neighborCount = m_adjacentVV->getOneRowElemNum(currIndex);
	QVector<int> neighborVertexIndices = m_adjacentVV->getOneRowColIndex(currIndex);

	assert(vertexWeight.count() == neighborCount);
	for (int i = 0; i < neighborCount; i++)
	{
		vertexWeight[i] = 0;
	}

	// 局部参数化
	// 计算每个点的角度和长度，
	QVector<float> neighborAngle;
	QVector<float> neighborLength;
	float sumAngles = 0;
	glm::vec3 p, pa, pb;
	p.x = m_vertexPos.at(currIndex * 3 + 0);
	p.y = m_vertexPos.at(currIndex * 3 + 1);
	p.z = m_vertexPos.at(currIndex * 3 + 2);
	for (int i = 0 ; i < neighborCount; i++)
	{
		int paIndex = neighborVertexIndices.at(i % neighborCount);
		int pbIndex = neighborVertexIndices.at((i + 1) % neighborCount);
		pa.x = m_vertexPos.at(paIndex * 3 + 0);
		pa.y = m_vertexPos.at(paIndex * 3 + 1);
		pa.z = m_vertexPos.at(paIndex * 3 + 2);

		pb.x = m_vertexPos.at(pbIndex * 3 + 0);
		pb.y = m_vertexPos.at(pbIndex * 3 + 1);
		pb.z = m_vertexPos.at(pbIndex * 3 + 2);

		float angle = qAcos(glm::dot(pa - p, pb - p) / (glm::length(pa - p) * glm::length(pb - p)));

		sumAngles += angle;
		neighborAngle.append(sumAngles);
		neighborLength.append(glm::length(pa - p));
	}
	// 计算每条边与X轴的夹角，为了后面转换成极坐标的值
	for (int i = 0; i < neighborCount; i++)
	{
		neighborAngle[i] = 2 * M_PI * neighborAngle[i] / sumAngles;
	}
	// 计算局部参数化坐标
	QVector<float> localParameterizedResult;
	for (int i = 0; i < neighborCount; i++)
	{
		localParameterizedResult.append(neighborLength.at(i) * qCos(neighborAngle.at(i)));
		localParameterizedResult.append(neighborLength.at(i) * qSin(neighborAngle.at(i)));
	}

	// 可以假设 p = 0
	int indexV1, indexV2, indexV3;
	glm::vec2 v1, v2, v3, vp;
	vp = glm::vec2(0.0f);
	for (int i = 0; i < neighborCount; i++)
	{
		indexV1 = i;
		v1 = glm::vec2(localParameterizedResult[indexV1 * 2], localParameterizedResult[indexV1 * 2 + 1]);

		for (int j = 0; j < neighborCount; j++)
		{
			if (j == i || j + 1 == i) continue;

			indexV2 = j % neighborCount;
			indexV3 = (j + 1) % neighborCount;

			v2 = glm::vec2(localParameterizedResult[indexV2 * 2], localParameterizedResult[indexV2 * 2 + 1]);
			v3 = glm::vec2(localParameterizedResult[indexV3 * 2], localParameterizedResult[indexV3 * 2 + 1]);

			// 计算三角形V1V2V3的面积，注意V1V2V3三点共线
			float areaV1V2V3 = qAbs(v1.x * v2.y + v2.x * v3.y + v3.x * v1.y - v2.x * v1.y - v3.x * v2.y - v1.x * v3.y) * 0.5;
			if (areaV1V2V3 < Z_EPSILON)
			{
				// 三点共线
				continue;
			}

			// 计算P点与任意两点组成的三角形的面积
			float areaPV2V3 = qAbs(vp.x * v2.y + v2.x * v3.y + v3.x * vp.y - v2.x * vp.y - v3.x * v2.y - vp.x * v3.y) * 0.5;
			float areaV1PV3 = qAbs(v1.x * vp.y + vp.x * v3.y + v3.x * v1.y - vp.x * v1.y - v3.x * vp.y - v1.x * v3.y) * 0.5;
			float areaV1V2P = qAbs(v1.x * v2.y + v2.x * vp.y + vp.x * v1.y - v2.x * v1.y - vp.x * v2.y - v1.x * vp.y) * 0.5;

			// p 点可能在三角形 V1V2V3 之外
			if (qAbs(areaV1V2V3 - (areaPV2V3 + areaV1PV3 + areaV1V2P)) > Z_EPSILON) continue;
			
			vertexWeight[indexV1] += areaPV2V3 / areaV1V2V3;
			vertexWeight[indexV2] += areaV1PV3 / areaV1V2V3;
			vertexWeight[indexV3] += areaV1V2P / areaV1V2V3;
			
			break;
		}
	}

	// 均值
	for (int i = 0; i < neighborCount; i++)
	{
		vertexWeight[i] /= neighborCount;
	}
}
