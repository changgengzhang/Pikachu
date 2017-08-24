#include "Mesh.h"
/************************************************************************/
/*		Assimp has uniform the layout that vertex index start from zero	*/
/************************************************************************/


// =========== construct and destruct =================
Mesh::Mesh()
{
	m_vertexCount = 0;
	m_faceCount = 0;

	m_maxCoord = glm::vec3(std::numeric_limits<float>::min());
	m_minCoord = glm::vec3(std::numeric_limits<float>::max());
	
	m_adjacentVV = nullptr;
	m_adjacentVF = nullptr;
	m_adjacentFF = nullptr;
}

Mesh::~Mesh()
{
	/*delete[] m_vertexPos;
	delete[] m_originalPos;
	delete[] m_vertexNormal;

	delete[] m_faceIndex;
	delete[] m_faceNormal;
	delete[] m_dualVertexPos;

	delete[] m_flag;
	delete[] m_isBoundary;

	delete[] m_color;*/
}


// 20170812  only obj format
bool Mesh::buildMesh(QString fileName)
{
	// only support .obj
	QString type = fileName.split(".").last();
	if (type == "obj")
	{
		bool ret = this->parseMeshFromObjFile(fileName);
		if (ret)
		{
			return true;
		}
		else
		{
			qDebug() << "ERROR::MESH::buildMesh::parseFromObjFile: Failed";
			return false;
		}
		
	}
	else if (type == "ply")
	{
		return false;
	}
	return false;
}


// ======== prase molde file to mesh ============
bool Mesh::parseMeshFromObjFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "ERROR::MESH::parseFromObjFile: model file open failed";
		return false;
	}
	QTextStream stream(&file);

	QString line, dataType;
	QStringList tokens, tokens2;
	float x, y, z;
	while (stream.readLineInto(&line))
	{
		tokens = line.split(QRegExp("\\s+"));

		dataType = tokens.first().toLower();

		if (dataType == "v")
		{
			x = tokens.at(1).toFloat();
			y = tokens.at(2).toFloat();
			z = tokens.at(3).toFloat();
			m_vertexPos.append(x);
			m_vertexPos.append(y);
			m_vertexPos.append(z);
			m_originalPos.append(x);
			m_originalPos.append(y);
			m_originalPos.append(z);
		}
		else if (dataType == "f")
		{
			for (int i = 1; i < tokens.count(); i++)
			{
				tokens2 = tokens.at(i).split("/");
				// the face index of obj format file is start from one, however in OpenGL the index musr strt form zero while use glDrawElements
				m_faceIndex.append(tokens2.at(0).toUInt() - 1);
			}
		}
	}
	
	this->initMesh();

	return true;
}


// ========= malloc memory for mesh ==============
void Mesh::initMesh()
{
	m_vertexCount = m_vertexPos.count() / 3;
	m_faceCount = m_faceIndex.count() / 3;


	this->scaleToUnitBox();
	this->moveToCenter();
	this->computeNormal();
	this->buildAdjacentVV();
	this->buildAdjacentVF();
	this->buildAdjacentFF();
	this->findBoundaryVertex();

	m_parameterization = new Parameterization(m_vertexPos, m_faceIndex, m_isBoundary, *m_adjacentVV, m_boundaryVertexCount);
	m_parameterization->calculate(ParameterizedType::BOUNDARY_SQUARE);

}


void Mesh::scaleToUnitBox()
{
	glm::vec3 dist = this->computeMaxCoord() - this->computeMinCoord();
	
	float scale = (dist.x > dist.y) ? (dist.x > dist.z ? dist.x : dist.z) : (dist.y > dist.z ? dist.y : dist.z);

	if (scale <= 0)
	{
		return;
	}

	for (uint i = 0; i < m_vertexPos.count(); i++)
	{
		m_vertexPos[i] /= scale;
	}
}

// this function only means that move the mesh to the center of a unit box.
// do not take the center as the center of view coordinate
void Mesh::moveToCenter()
{
	glm::vec3 center = (this->computeMaxCoord() + this->computeMinCoord()) / 2.0f;

	QString out;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		m_vertexPos[i * 3] -= center.x;
		m_vertexPos[i * 3 + 1] -= center.y;
		m_vertexPos[i * 3 + 2] -= center.z;
	}
}

// compute face normal and vertex normal, modification from trimesh2
void Mesh::computeNormal()
{
	glm::vec3 *vnormal;
	glm::vec3 faceNormal;
	glm::vec3 p0, p1, p2;
	glm::vec3 v01, v02, v12;
	uint v0, v1, v2;
	float lenV01, lenV02, lenV12;

	vnormal = new glm::vec3[m_vertexCount];

	for (uint i = 0; i < m_faceCount; i++)
	{
		v0 = m_faceIndex[i * 3];
		v1 = m_faceIndex[i * 3 + 1];
		v2 = m_faceIndex[i * 3 + 2];

		p0 = getOneVertex(v0);
		p1 = getOneVertex(v1);
		p2 = getOneVertex(v2);

		/*
		in trimesh2 compute normal in this way
		QVector3D a = p0-p1, b = p1-p2, c = p2-p0;
		QVector3D facenormal = flip ? (b CROSS a) : (a CROSS b);
		*/
		v01 = p1 - p0;
		v02 = p2 - p0;
		v12 = p2 - p1;

		lenV01 = v01.length();
		lenV02 = v02.length();
		lenV12 = v12.length();

		faceNormal = glm::cross(v01, v02);
		
		// vertex normal is average of face normal which belongs to
		vnormal[v0] += faceNormal * (1.0f / (lenV01 + lenV02));
		vnormal[v1] += faceNormal * (1.0f / (lenV01 + lenV12));
		vnormal[v2] += faceNormal * (1.0f / (lenV02 + lenV12));

		faceNormal = glm::normalize(faceNormal);
		m_faceNormal.append(faceNormal.x);
		m_faceNormal.append(faceNormal.y);
		m_faceNormal.append(faceNormal.z);
	}

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vnormal[i] = glm::normalize(vnormal[i]);
		m_vertexNormal.append(vnormal[i].x);
		m_vertexNormal.append(vnormal[i].y);
		m_vertexNormal.append(vnormal[i].z);
	}

	delete[] vnormal;
}

// compute vertex-vertex adjacent matrix
void Mesh::buildAdjacentVV()
{
	m_adjacentVV = new SparseMatrix<int>(m_vertexCount);

	int p0, p1, p2;
	for (uint i = 0; i < m_faceCount; i++)
	{
		p0 = m_faceIndex[i * 3];
		p1 = m_faceIndex[i * 3 + 1];
		p2 = m_faceIndex[i * 3 + 2];

		// (i, j) = 1 means that there is edge between i and j, the direction is i to j
		// (i, j) = -1 means that there is edge between i and j, the direction is j to i
		m_adjacentVV->set(1, p0, p1);
		m_adjacentVV->set(1, p1, p2);
		m_adjacentVV->set(1, p2, p0);
		m_adjacentVV->setIfNotExist(-1, p0, p2);
		m_adjacentVV->setIfNotExist(-1, p2, p1);
		m_adjacentVV->setIfNotExist(-1, p1, p0);
	}
}

void Mesh::buildAdjacentVF()
{
	m_adjacentVF = new SparseMatrix<int>(m_vertexCount, m_faceCount);

	for (uint i = 0; i < m_faceCount; i++)
	{
		// the face index start form zero
		m_adjacentVF->set(1, m_faceIndex[i * 3], i);
		m_adjacentVF->set(1, m_faceIndex[i * 3 + 1], i);
		m_adjacentVF->set(1, m_faceIndex[i * 3 + 2], i);
	}
}

// must build vertex-face matrix before call the function
void Mesh::buildAdjacentFF()
{
	m_adjacentFF = new SparseMatrix<int>(m_faceCount);
	int v0, v1, v2;
	uint faceIndex;
	QVector<int> oneRow;

	for (uint i = 0; i < m_faceCount; i++)
	{
		v0 = m_faceIndex[3 * i];
		v1 = m_faceIndex[3 * i + 1];
		v2 = m_faceIndex[3 * i + 2];

		oneRow = m_adjacentVF->getOneRowColIndex(v0);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			// must share one edge if two face is adjacent.
			if (faceIndex != i && isFaceContainVertex(faceIndex, v1))
			{
				// assume face index is start from zero
				m_adjacentFF->set(1, i, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getOneRowColIndex(v1);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != i && isFaceContainVertex(faceIndex, v2))
			{
				m_adjacentFF->set(1, i, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getOneRowColIndex(v2);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != i && isFaceContainVertex(faceIndex, v0))
			{
				m_adjacentFF->set(1, i, faceIndex);
			}
		}
	}
}


void Mesh::findBoundaryVertex()
{
	int numAdjV, numAdjF;

	m_boundaryVertexCount = 0;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		numAdjV = m_adjacentVV->getOneRowElemNum(i);
		numAdjF = m_adjacentVF->getOneRowElemNum(i);
		m_isBoundary.append(numAdjV != numAdjF);
		m_boundaryVertexCount += numAdjV != numAdjF ? 1 : 0;
	}
}

// ====================== tool function ================================
const glm::vec3  Mesh::computeMaxCoord() const
{
	glm::vec3 maxCoord = glm::vec3(std::numeric_limits<float>::min());
	glm::vec3 vertex;

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vertex = glm::vec3(m_vertexPos[i * 3], m_vertexPos[i * 3 + 1], m_vertexPos[i * 3 + 2]);
		maxCoord = glm::vec3(
			vertex.x > maxCoord.x ? vertex.x : maxCoord.x,
			vertex.y > maxCoord.y ? vertex.y : maxCoord.y,
			vertex.z > maxCoord.z ? vertex.z : maxCoord.z
		);
	}

	return maxCoord;
}


const glm::vec3 Mesh::computeMinCoord() const
{
	glm::vec3 minCoord = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 vertex;

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vertex = glm::vec3(m_vertexPos[i * 3], m_vertexPos[i * 3 + 1], m_vertexPos[i * 3 + 2]);
		minCoord = glm::vec3(
			vertex.x < minCoord.x ? vertex.x : minCoord.x,
			vertex.y < minCoord.y ? vertex.y : minCoord.y,
			vertex.z < minCoord.z ? vertex.z : minCoord.z
		);
	}

	return minCoord;
}


glm::vec3 Mesh::getOneVertex(uint index) const
{
	return glm::vec3(m_vertexPos[index * 3], m_vertexPos[index * 3 + 1], m_vertexPos[index * 3 + 2]);
}


// we assume that face index is start from one
bool Mesh::isFaceContainVertex(uint fIndex, uint vIndex) const
{
	int v0, v1, v2;
	v0 = m_faceIndex[fIndex * 3];
	v1 = m_faceIndex[fIndex * 3 + 1];
	v2 = m_faceIndex[fIndex * 3 + 2];

	return (v0 == vIndex) || (v1 == vIndex) || (v2 == vIndex);
}

// ========= get value ===================
const int Mesh::getVertexCount() const
{
	return m_vertexCount;
}


const int Mesh::getFaceCount() const
{
	return m_faceCount;
}


const float* Mesh::getVertexPos() const
{
	return m_vertexPos.constData();
}


const float* Mesh::getOriginalPos() const
{
	return m_originalPos.constData();
}


const uint* Mesh::getFaceIndex() const
{
	return m_faceIndex.constData();
}

const float* Mesh::getVertexNormal() const
{
	return m_vertexNormal.constData();
}


const float* Mesh::getFaceNormal() const
{
	return m_faceNormal.constData();
}



template<typename T> void  Mesh::printQVector(QVector<T> &v, QString name)
{
	QString out;
	out += name;
	out += "\n";
	for (uint i = 0; i < v.count(); i++)
	{
		out += v.at(i);
	}

	qInfo() << out.toStdString().c_str();
}