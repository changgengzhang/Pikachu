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


};

#endif // !MESH_H



