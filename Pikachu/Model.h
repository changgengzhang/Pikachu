#ifndef MODEL_H
#define MODEL_H

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopengl.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

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


