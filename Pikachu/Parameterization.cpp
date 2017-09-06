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
			boundaryVertices.push_back(pos);
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
				// ���ѡ�еĵ��к͵�ǰ�ĵ�֮�䶼��һ��ָ����Ե�����ߣ���ô��������㡣
				if (m_adjacentVV->get(vertexIndex, currVertexIndex) != 1)
				{
					currVertexIndex = vertexIndex;
					boundaryVertices.push_back(currVertexIndex);
					findTag[currVertexIndex] = true;
					break;
				}
			}
		}
	}
	
	for (uint i = 0; i < boundaryVertices.count(); i++)
	{
		m_boundaryVertexIndices.push_back(boundaryVertices.at(i));
	}

	// ============== Innaer vertices ====================
	for (uint index = 0; index < m_vertexCount; index++)
	{
		if (!m_boundaryVertexIndices.contains(index))
		{
			m_innerVertexIndices.push_back(index);
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

		weight.push_back(m_boundaryLength);
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
			if (weight[i] < 0.25 && weight[(i + 1) % m_boundaryVertexCount] > 0.25)
			{
				m_boundaryVerticesResult.push_back(0.25 * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.push_back(-edgeLenth / 2.0f);
			}
			else if (weight[i] <= 0.25)
			{
				m_boundaryVerticesResult.push_back(weight.at(i) * 4 * edgeLenth - edgeLenth / 2.0f);
				m_boundaryVerticesResult.push_back(-edgeLenth / 2.0f);
			}
			else if (weight[i] < 0.5 && weight[(i + 1) % m_boundaryVertexCount] > 0.5)
			{
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f);
				m_boundaryVerticesResult.push_back((0.5 - 0.25) * 4 * edgeLenth - edgeLenth / 2.0f);
			}
			else if (weight[i] <= 0.50)
			{
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f);
				m_boundaryVerticesResult.push_back((weight.at(i) - 0.25) * 4 * edgeLenth - edgeLenth / 2.0f);
			}
			else if (weight[i] < 0.75 && weight[(i + 1) % m_boundaryVertexCount] > 0.75)
			{
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f - (0.75 - 0.5) * 4 * edgeLenth);
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f);
			}
			else if (weight[i] <= 0.75)
			{
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f - (weight.at(i) - 0.5) * 4 * edgeLenth);
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f);
			}
			else if (weight[i] <= 1.0)
			{
				m_boundaryVerticesResult.push_back(-edgeLenth / 2.0f);
				m_boundaryVerticesResult.push_back(edgeLenth / 2.0f - (weight.at(i) - 0.75) * 4 * edgeLenth);
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
			m_boundaryVerticesResult.push_back(radius * qCos(weight[i] / m_boundaryLength));
			m_boundaryVerticesResult.push_back(radius * qSin(weight[i] / m_boundaryLength));
		}
		break;
	}
	default:
		break;
	}

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
				vertexWeight.push_back(1.0f / neighborCount);
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

			// �ж�����ڽӵ����ڲ��㻹�Ǳ߽��
			index = m_innerVertexIndices.indexOf(neighborIndex);

			if (index == -1)
			{
				// �߽��
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
				// �ڲ��� ���ӣ�
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
		m_innerVerticesResult.push_back(u(i, 0));
		m_innerVerticesResult.push_back(u(i, 1));
	}
}



void Parameterization::mergeBoundaryAndInnerParameterizedResult()
{
	int index, step;

	m_parameterizedResult.resize(m_vertexCount * 2);
	
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


void Parameterization::ComputeShapPreservingWeight(int index, QVector<float> &vertexWeight)
{

	int neighborCount = m_adjacentVV->getOneRowElemNum(index);
	// �����weight vector�Ĵ�Сһ��Ҫ����Ҫ��
	assert(vertexWeight.count() == neighborCount);
	
	QVector<int> neighborVertexIndices = m_adjacentVV->getOneRowColIndex(index);
 
	/*
		��p����ڽӵ�������Ϊ�Ұ�Χ��������εĶ���p1p2p3ʱ��p2p3һ�������ڵģ������������һ��ò�����֤
		������Ҫ������ȱ������ڽӵ����� ��Ϊ���ܳ����������������
		��ȷ�����p0 --> p1  --> p2��
		�����ܻ���� p0 --> p2��©��p1�������

						   *p2							
						*   *
					 *	  *	*
				  *		*	*
			   *	  *		*
		  p	*  *  * p1		*
			   *	  *		*
				  *		*	*
				     *	  *	*	
						*	*						
						   *p0
	*/
	QVector<int> localNeighborVertexIndices;
	{
		int currIndex = neighborVertexIndices.at(0);
		localNeighborVertexIndices.push_back(currIndex);
		this->sortNeighborVertex(neighborVertexIndices, localNeighborVertexIndices, currIndex);
	}
	
	// �ֲ�������
	// ����ÿ����ĽǶȺͳ��ȣ�
	QVector<float> neighborAngle;
	QVector<float> neighborRadius;
	float sumAngles = 0;
	glm::vec3 p, pa, pb;
	p.x = m_vertexPos.at(index * 3 + 0);
	p.y = m_vertexPos.at(index * 3 + 1);
	p.z = m_vertexPos.at(index * 3 + 2);
	for (int i = 0 ; i < neighborCount; i++)
	{
		int paIndex = localNeighborVertexIndices.at(i % neighborCount);
		int pbIndex = localNeighborVertexIndices.at((i + 1) % neighborCount);
		pa.x = m_vertexPos.at(paIndex * 3 + 0) - p.x;
		pa.y = m_vertexPos.at(paIndex * 3 + 1) - p.y;
		pa.z = m_vertexPos.at(paIndex * 3 + 2) - p.z;

		pb.x = m_vertexPos.at(pbIndex * 3 + 0) - p.x;
		pb.y = m_vertexPos.at(pbIndex * 3 + 1) - p.y;
		pb.z = m_vertexPos.at(pbIndex * 3 + 2) - p.z;

		float angle = qAcos(glm::dot(pa, pb) / (glm::length(pa) * glm::length(pb)));

		sumAngles += angle;
		neighborAngle.push_back(sumAngles);
		neighborRadius.push_back(glm::length(pa));
	}

	// ����ÿ������X��ļнǣ�Ϊ�˺���ת���ɼ������ֵ
	for (int i = 0; i < neighborCount; i++)
	{
		neighborAngle[i] = 2 * M_PI * neighborAngle[i] / sumAngles;
	}

	// ����ֲ�����������
	// ���ｫ��������10��
	QVector<float> localParameterizedResult;
	for (int i = 0; i < neighborCount; i++)
	{
		float x = 10 * neighborRadius.at(i) * qCos(neighborAngle.at(i));
		float y = 10 * neighborRadius.at(i) * qSin(neighborAngle.at(i));
		localParameterizedResult.push_back(x);
		localParameterizedResult.push_back(y);
	}
	
	// ���Լ��� p = 0
	int indexV1, indexV2, indexV3;
	glm::vec2 v1, v2, v3, vp;
	QVector<float> localWeight;

	localWeight.resize(neighborCount);
	for (int i = 0; i < neighborCount; i++)
	{
		localWeight[i] = 0.0f;
	}

	vp = glm::vec2(0.0f);
	for (int i = 0; i < neighborCount; i++)
	{
		indexV1 = i;
		v1 = glm::vec2(localParameterizedResult[indexV1 * 2], localParameterizedResult[indexV1 * 2 + 1]);

		for (int j = 0; j < neighborCount; j++)
		{
			indexV2 = j % neighborCount;
			indexV3 = (j + 1) % neighborCount;

			if (indexV2 == indexV1 || indexV3 == indexV1) continue;

			v2 = glm::vec2(localParameterizedResult[indexV2 * 2], localParameterizedResult[indexV2 * 2 + 1]);
			v3 = glm::vec2(localParameterizedResult[indexV3 * 2], localParameterizedResult[indexV3 * 2 + 1]);

			// ����������V1V2V3�������ע��V1V2V3���㹲��
			float areaV1V2V3 = qAbs(v1.x * v2.y + v2.x * v3.y + v3.x * v1.y - v2.x * v1.y - v3.x * v2.y - v1.x * v3.y) * 0.5;
			if (areaV1V2V3 < Z_EPSILON)
			{
				// ���㹲��
				continue;
			}

			// ����P��������������ɵ������ε����
			float areaPV2V3 = qAbs(vp.x * v2.y + v2.x * v3.y + v3.x * vp.y - v2.x * vp.y - v3.x * v2.y - vp.x * v3.y) * 0.5;
			float areaV1PV3 = qAbs(v1.x * vp.y + vp.x * v3.y + v3.x * v1.y - vp.x * v1.y - v3.x * vp.y - v1.x * v3.y) * 0.5;
			float areaV1V2P = qAbs(v1.x * v2.y + v2.x * vp.y + vp.x * v1.y - v2.x * v1.y - vp.x * v2.y - v1.x * vp.y) * 0.5;

			// p ������������� V1V2V3 ֮��
			if (qAbs(areaV1V2V3 - (areaPV2V3 + areaV1PV3 + areaV1V2P)) > Z_EPSILON) continue;
			
			localWeight[indexV1] += areaPV2V3 / areaV1V2V3;
			localWeight[indexV2] += areaV1PV3 / areaV1V2V3;
			localWeight[indexV3] += areaV1V2P / areaV1V2V3;
			
			break;
		}
	}

	// �õ���������֮��ģ�����Ҫ�ָ�ԭ����˳��
	for (int i = 0; i < neighborCount; i++)
	{
		int currIndex = localNeighborVertexIndices.at(i);
		int pos = neighborVertexIndices.indexOf(currIndex);
		
		assert(pos != -1);

		vertexWeight[pos] = localWeight.at(i) / neighborCount;
	}
}


bool Parameterization::sortNeighborVertex(QVector<int> &neighborVertexIndices, QVector<int> &localNeighborVertexIndices, int index)
{
	// �ݹ����
	if (localNeighborVertexIndices.count() == neighborVertexIndices.count())
	{
		return true;
	}

	QVector<int> neighborIndex = m_adjacentVV->getOneRowColIndex(index);
	for (int j = 0; j < neighborIndex.count(); j++)
	{
		int index = neighborIndex.at(j);
		if (neighborVertexIndices.contains(index) && !localNeighborVertexIndices.contains(index))
		{
			localNeighborVertexIndices.push_back(index);
			if (this->sortNeighborVertex(neighborVertexIndices, localNeighborVertexIndices, index))
			{
				return true;
			}
			else
			{
				localNeighborVertexIndices.pop_back();
			}
		}
	}

	return false;
}

