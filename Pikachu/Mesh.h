#ifndef MESH_H
#define MESH_H
// Qt
#include <qvector.h>
#include <qdebug.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

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
	bool buildMesh(QString fileName);
	
	// ========= get value ===================
	const int getVertexCount() const;
	const int getFaceCount() const;
	const float* getVertexPos() const;
	const float* getOriginalPos() const;
	const uint* getFaceIndex() const;
	const float* getVertexNormal() const;
	const float* getFaceNormal() const;

	const glm::vec3 getMeshCenter() const;

private:
	// ======== prase molde file to mesh ============
	bool parseMeshFromObjFile(QString fileName);

	// ========= malloc memory for mesh ==============
	void initMeshValue(QVector<float> &vlist, QVector<uint> &flist);

	// ========== operation to vertex function ==========
	void scaleToUnitBox();
	void moveToCenter();
	void computeNormal();
	void buildAdjacentVV();
	void buildAdjacentVF();
	void buildAdjacentFF();
	void findBoundaryVertex();

	// ============ tools function ============
	const glm::vec3 computeMaxCoord() const;
	const glm::vec3 computeMinCoord() const;
	inline glm::vec3 getOneVertex(uint index) const;
	bool isFaceContainVertex(uint fIndex, uint vIndex) const;

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

	// ============ sparse matrix for vertex-vertex vertex-face face-face =============
	SparseMatrix<uint>* m_adjacentVV;
	SparseMatrix<uint>* m_adjacentVF;
	SparseMatrix<uint>* m_adjacentFF;

	
};

#endif // !MESH_H



