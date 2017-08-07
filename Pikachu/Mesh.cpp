#include "Mesh.h"


Mesh::Mesh()
{
	m_vertexCount = 0;
	m_faceCount = 0;
	m_vertexPos = nullptr;
	m_originalPos = nullptr;
	m_vertexNormal = nullptr;
	m_faceIndex = nullptr;
	m_dualFaceIndex = nullptr;
	m_faceNormal = nullptr;
	m_dualVertexPos = nullptr;
	m_flag = nullptr;
	m_isBoundary = nullptr;
	m_color = nullptr;
}

Mesh::~Mesh()
{
}
