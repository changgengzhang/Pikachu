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
	QVector<float> m_vertexPos;
	QVector<float> m_originalPos;
	QVector<float> m_vertexNormal;
	QVector<uint> m_faceIndex;
	QVector<uint> m_dualFaceIndex;
	QVector<float> m_faceNormal;
	QVector<float> m_dualVertexPos;
	QVector<unsigned char> m_flag;
	QVector<bool> m_isBoundary;
	QVector<float> m_color;

	glm::vec3 m_meshBarycenter;
	glm::vec3 m_maxCoord;
	glm::vec3 m_minCoord;

};

#endif // !MESH_H



