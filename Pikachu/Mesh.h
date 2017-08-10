#ifndef MESH_H
#define MESH_H
// Qt
#include <qvector.h>

// Assimp
#include <assimp/scene.h>
// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "SparseMatrix.h"

class Mesh
{
public:
	// =========== construct and destruct =================
	Mesh();
	~Mesh();

	// ========== interactive with outer =================
	bool setupMeshByAimesh(aiMesh *mesh);

private:
	// ========== operation to vertex ==========
	void scaleToUnitBox();
	void moveToCenter();
	void computeNormal();

	void buildAdjacentVV();
	void buildAdjacentVF();
	
	// ============ tools function ============
	inline glm::vec3 maxBBOXCoord(glm::vec3 va, glm::vec3 vb);
	inline glm::vec3 minBBOXCoord(glm::vec3 va, glm::vec3 vb);
	inline glm::vec3 getOneVertex(uint pos);

private:
	// ============ values for mesh ==============
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

	// ============ value for unit box ============
	glm::vec3 m_meshCenter;
	glm::vec3 m_maxCoord;
	glm::vec3 m_minCoord;

	// ============ sparse matrix for vertex-vertex =============
	SparseMatrix<uint>* m_adjacentVV;
	SparseMatrix<uint>* m_adjacentVF;
	SparseMatrix<uint>* m_adjacentFF;
};

#endif // !MESH_H



