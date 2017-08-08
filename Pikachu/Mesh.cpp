#include "Mesh.h"


Mesh::Mesh()
{
	m_vertexCount = 0;
	m_faceCount = 0;

	m_meshBarycenter = QVector3D(0.0f, 0.0f, 0.0f);
	m_maxCoord = QVector3D(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	m_minCoord = QVector3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	
}

Mesh::~Mesh()
{

}

bool Mesh::setupMeshByAimesh(aiMesh *mesh)
{
	m_vertexCount = mesh->mNumVertices;
	m_faceCount = mesh->mNumFaces;

	float x, y, z;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		x = mesh->mVertices[i].x;
		y = mesh->mVertices[i].y;
		z = mesh->mVertices[i].z;

		m_vertexPos.append(x);
		m_vertexPos.append(y);
		m_vertexPos.append(z);

		m_originalPos.append(x);
		m_originalPos.append(y);
		m_originalPos.append(z);

		m_maxCoord = zcg::maxBBOXCoord(m_maxCoord, QVector3D(x, y, z));
		m_minCoord = zcg::minBBOXCoord(m_minCoord, QVector3D(x, y, z));
	}

	for (uint i = 0; i < m_faceCount; i++)
	{
		aiFace face = mesh->mFaces[i];
		// triangular face has 3 indices per face
		for (uint j = 0; j < face.mNumIndices; i++)
		{
			m_faceIndex.append(face.mIndices[j]);
		}
	}

	scaleToUnitBox();
}

void Mesh::scaleToUnitBox()
{
	QVector3D dist = m_maxCoord - m_minCoord;
	
	float scale = (dist.x() > dist.y()) ? (dist.x() > dist.z() ? dist.x() : dist.z()) : (dist.y() > dist.z() ? dist.y() : dist.z());

	if (scale <= 0)
	{
		return;
	}

	for (uint i = 0; i < m_vertexPos.count(); i++)
	{
		m_vertexPos[i] /= scale;
	}
}

void Mesh::moveToCenter()
{
	QVector3D center = (m_maxCoord + m_minCoord) / 2.0f;

	for (uint i = 0; i < m_vertexCount; i++)
	{
		m_vertexPos[i * 3] -= center.x();
		m_vertexPos[i * 3 + 1] -= center.y();
		m_vertexPos[i * 3 + 2] -= center.z();
	}
}

void Mesh::computeNormal()
{

}
