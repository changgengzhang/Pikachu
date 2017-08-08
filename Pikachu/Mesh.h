#ifndef MESH_H
#define MESH_H

#include <qvector.h>
#include <qvector3d.h>

#include <assimp/scene.h>

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
	void computeNormal();

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

	QVector3D m_meshBarycenter;
	QVector3D m_maxCoord;
	QVector3D m_minCoord;

};

#endif // !MESH_H



