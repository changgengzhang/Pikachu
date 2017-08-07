#ifndef MODEL_H
#define MODEL_H

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopengl.h>
#include <qstring.h>
#include <qdebug.h>
// ASSIMP library for load object
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	Model(QWidget *parent);
	~Model();
	bool loadModelFromFile(QString fileName);

private:
	QString m_fileName;


};

#endif // !MODEL_H


