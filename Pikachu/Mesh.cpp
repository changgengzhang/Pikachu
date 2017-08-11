#include "Mesh.h"
/************************************************************************/
/*			the vertex index is start from one, not zero				*/
/*		 so we decide the face index is also start from one				*/
/************************************************************************/


// =========== construct and destruct =================
Mesh::Mesh()
{
	m_vertexCount = 0;
	m_faceCount = 0;

	m_meshCenter = glm::vec3(0.0f);
	m_maxCoord = glm::vec3(std::numeric_limits<float>::min());
	m_minCoord = glm::vec3(std::numeric_limits<float>::max());
	
	m_adjacentVV = nullptr;
	m_adjacentVF = nullptr;
	m_adjacentFF = nullptr;
}

Mesh::~Mesh()
{

}

bool Mesh::setupMeshByAimesh(aiMesh *mesh)
{
	m_vertexCount = mesh->mNumVertices;
	m_faceCount = mesh->mNumFaces;

	m_vertexPos = new float(m_vertexCount * 3);
	m_originalPos = new float(m_vertexCount * 3);
	m_vertexNormal = new float(m_vertexCount * 3);

	m_faceIndex = new uint(m_faceCount * 3);
	m_faceNormal = new float(m_faceCount * 3);
	m_dualVertexPos = new float(m_faceCount * 3);

	m_flag = new uchar(m_vertexCount);
	m_isBoundary = new bool(m_vertexCount);

	m_color = new float(m_faceCount);


	float x, y, z;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		x = mesh->mVertices[i].x;
		y = mesh->mVertices[i].y;
		z = mesh->mVertices[i].z;

		m_vertexPos[i * 3] = x;
		m_vertexPos[i * 3 + 1] = y;
		m_vertexPos[i * 3 + 2] = z;

		m_originalPos[i * 3] = x;
		m_originalPos[i * 3 + 1] = y;
		m_originalPos[i * 3 + 2] = z;


		m_maxCoord = this->maxBBOXCoord(m_maxCoord, glm::vec3(x, y, z));
		m_minCoord = this->minBBOXCoord(m_minCoord, glm::vec3(x, y, z));
	}

	for (uint i = 0; i < m_faceCount; i++)
	{
		aiFace face = mesh->mFaces[i];
		// triangular face has 3 indices per face
		m_faceIndex[i * 3] = face.mIndices[0];
		m_faceIndex[i * 3 + 1] = face.mIndices[1];
		m_faceIndex[i * 3 + 2] = face.mIndices[2];
	}
	
	this->scaleToUnitBox();
	this->moveToCenter(); 
	this->computeNormal();
	this->buildAdjacentVV();
	this->buildAdjacentVF();
	this->buildAdjacentFF();

	return true;
}


void Mesh::scaleToUnitBox()
{
	glm::vec3 dist = m_maxCoord - m_minCoord;
	
	float scale = (dist.x > dist.y) ? (dist.x > dist.z ? dist.x : dist.z) : (dist.y > dist.z ? dist.y : dist.z);

	if (scale <= 0)
	{
		return;
	}

	for (uint i = 0; i < m_vertexCount * 3; i++)
	{
		m_vertexPos[i] /= scale;
	}
}

// this function only means that move the mesh to the center of a unit box.
// do not take the center as the center of view coordinate
void Mesh::moveToCenter()
{
	glm::vec3 center = (m_maxCoord + m_minCoord) / 2.0f;
	m_meshCenter = center;

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
		vnormal[v0] = vnormal[v0] + faceNormal * (1.0f / (lenV01 + lenV02));
		vnormal[v1] = vnormal[v1] + faceNormal * (1.0f / (lenV01 + lenV12));
		vnormal[v2] = vnormal[v2] + faceNormal * (1.0f / (lenV02 + lenV12));

		faceNormal = glm::normalize(faceNormal);
		m_faceNormal[i * 3] = faceNormal.x;
		m_faceNormal[i * 3 + 1] = faceNormal.y;
		m_faceNormal[i * 3 + 2] = faceNormal.z;
	}

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vnormal[i] = glm::normalize(vnormal[i]);
		m_vertexNormal[i * 3] = vnormal[i].x;
		m_vertexNormal[i * 3 + 1] = vnormal[i].y;
		m_vertexNormal[i * 3 + 2] = vnormal[i].z;
	}

	delete[] vnormal;
}

// compute vertex-vertex adjacent matrix
void Mesh::buildAdjacentVV()
{
	m_adjacentVV = new SparseMatrix<uint>(m_vertexCount);

	int p0, p1, p2;
	for (uint i = 0; i < m_faceCount; i++)
	{
		p0 = m_faceIndex[i * 3];
		p1 = m_faceIndex[i * 3 + 1];
		p2 = m_faceIndex[i * 3 + 2];

		m_adjacentVV->set(1, p0, p1);
		m_adjacentVV->set(1, p0, p2);
		m_adjacentVV->set(1, p1, p0);
		m_adjacentVV->set(1, p1, p2);
		m_adjacentVV->set(1, p2, p0);
		m_adjacentVV->set(1, p2, p1);
	}
}

void Mesh::buildAdjacentVF()
{
	m_adjacentVF = new SparseMatrix<uint>(m_vertexCount, m_faceCount);

	for (uint i = 0; i < m_faceCount; i++)
	{
		// the face index start form one for consistent  with vertex index
		m_adjacentVF->set(1, m_faceIndex[i * 3], i + 1);
		m_adjacentVF->set(1, m_faceIndex[i * 3 + 1], i + 1);
		m_adjacentVF->set(1, m_faceIndex[i * 3 + 1], i + 1);
	}
}

// must build vertex-face matrix before call the function
void Mesh::buildAdjacentFF()
{
	m_adjacentFF = new SparseMatrix<uint>(m_faceCount);
	int v0, v1, v2;
	uint faceIndex;
	QVector<uint> oneRow;

	for (uint i = 0; i < m_faceCount; i++)
	{
		v0 = m_faceIndex[3 * i];
		v1 = m_faceIndex[3 * i + 1];
		v2 = m_faceIndex[3 * i + 2];

		oneRow = m_adjacentVF->getRow(v0);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			// must share one edge if two face is adjacent.
			if (faceIndex != (i + 1) && isFaceContainVertex(faceIndex, v1))
			{
				// assume face index is start from one
				m_adjacentFF->set(1, i + 1, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getRow(v1);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != (i + 1) && isFaceContainVertex(faceIndex, v2))
			{
				m_adjacentFF->set(1, i + 1, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getRow(v2);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != (i + 1) && isFaceContainVertex(faceIndex, v1))
			{
				m_adjacentFF->set(1, i + 1, faceIndex);
			}
		}
	}
}

// ====================== tool function ================================

glm::vec3 Mesh::getOneVertex(uint index) const
{
	return glm::vec3(m_vertexPos[index * 3], m_vertexPos[index * 3 + 1], m_vertexPos[index * 3 + 2]);
}

inline glm::vec3 Mesh::maxBBOXCoord(glm::vec3 va, glm::vec3 vb) const
{
	return glm::vec3(
		va.x > vb.x ? va.x : vb.x,
		va.y > vb.y ? va.y : vb.y,
		va.z > vb.z ? va.z : vb.z
	);
}

inline glm::vec3 Mesh::minBBOXCoord(glm::vec3 va, glm::vec3 vb) const
{
	return glm::vec3(
		va.x < vb.x ? va.x : vb.x,
		va.y < vb.y ? va.y : vb.y,
		va.z < vb.z ? va.z : vb.z
	);
}

// we assume that face index is start from one
bool Mesh::isFaceContainVertex(uint fIndex, uint vIndex) const
{
	int v0, v1, v2;
	v0 = m_faceIndex[(fIndex - 1) * 3];
	v1 = m_faceIndex[(fIndex - 1) * 3 + 1];
	v2 = m_faceIndex[(fIndex - 1) * 3 + 1];

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
	return m_vertexPos;
}


const float* Mesh::getOriginalPos() const
{
	return m_originalPos;
}


const float* Mesh::getVertexNormal() const
{
	return m_vertexNormal;
}


const float* Mesh::getFaceNormal() const
{
	return m_faceNormal;
}