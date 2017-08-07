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
	QFile file(fileName);
	QTextStream textStream(&file);


}