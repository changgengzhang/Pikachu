#include "Model.h"

Model::Model(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

Model::~Model()
{
}

bool Model::loadModelFromFile(QString fileName)
{
	// Read file via ASSIMP, 
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
		return false;
	}
	// Most of the file formats don't really support complex scenes, but a single model only.
	aiMesh *mesh = scene->mMeshes[0];
	
}