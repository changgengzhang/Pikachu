#ifndef MESH_H
#define MESH_H

#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	int m_vertexCount;
	int m_faceCount;
	float *m_vertexPos;
	float *m_originalPos;
	float *m_vertexNormal;
	int *m_faceIndex;
	int *m_dualFaceIndex;
	float *m_faceNormal;
	float *m_dualVertexPos;
	unsigned char *m_flag;
	bool *m_isBoundary;
	float *m_color;


};

#endif // !MESH_H



