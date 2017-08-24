#ifndef MESH_H
#define MESH_H
// Qt
#include <qvector.h>
#include <qdebug.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "SparseMatrix.h"
#include "Util.h"
#include "Parameterization.h"

using namespace zcg;

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

private:
	// ======== prase molde file to mesh ============
	bool parseMeshFromObjFile(QString fileName);
	void initMesh();

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
	int m_boundaryVertexCount;
	QVector<float> m_vertexPos;
	QVector<float> m_originalPos;
	QVector<float> m_vertexNormal;
	QVector<float> m_textureCoordinate;
	QVector<uint> m_faceIndex;
	QVector<float> m_faceNormal;
	QVector<bool> m_isBoundary;

	// ============ value for unit box ============
	glm::vec3 m_maxCoord;
	glm::vec3 m_minCoord;

	// ============ sparse matrix for vertex-vertex vertex-face face-face =============
	SparseMatrix<int>* m_adjacentVV;
	SparseMatrix<int>* m_adjacentVF;
	SparseMatrix<int>* m_adjacentFF;


	// =============== parameterization =============
	Parameterization* m_parameterization;

private:
	template<typename T> void printQVector(QVector<T> &v, QString name);

};


#endif // !MESH_H



