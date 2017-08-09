#ifndef MESH_H
#define MESH_H

#include <qvector.h>

#include <assimp/scene.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "Util.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool setupMeshByAimesh(aiMesh *mesh);

private:
	void scaleToUnitBox();
	void moveToCenter();
	// void computeVertexNormal();
	// void computeFaceNormal();
	void computeNormal();
	inline glm::vec3 getOneVertex(uint pos);

private:
	int m_vertexCount;
	int m_faceCount; 
	float *m_vertexPos;
	float *m_originalPos;
	float *m_vertexNormal;
	uint *m_faceIndex;
	uint *m_dualFaceIndex;
	float *m_faceNormal;
	float *m_dualVertexPos;
	uchar *m_flag;
	bool *m_isBoundary;
	float *m_color;

	glm::vec3 m_meshBarycenter;
	glm::vec3 m_maxCoord;
	glm::vec3 m_minCoord;

};

#endif // !MESH_H



