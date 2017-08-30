#include "Model.h"

Model::Model(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_canRender = false;

	m_meshReady = false;
	m_mesh = nullptr;
	m_polygonWay = Z_NONE;
	
	m_textureReady = false;
	m_paramInnerType = Z_NONE;

	m_shaderProgram = nullptr;
	m_textureID = 0;

	m_arcball = nullptr;
}


Model::~Model()
{
	this->cleanup();
}


void Model::buildMesh(QString vertexShaderFile, QString fragmentShaderFile,QString modelFile)
{
	m_modelFileName = modelFile;
	m_vertexShaderFile = vertexShaderFile;
	m_fragmentShaderFile = fragmentShaderFile;

	this->loadMeshFromFile(m_modelFileName);
	this->buildShaderProgram(m_vertexShaderFile, m_fragmentShaderFile);
	this->buildVAOAndVBO();

	// mesh ready
	m_meshReady = true;
	
	// update canRender value
	// this->setCanRender(true);
	m_canRender = m_meshReady && (m_polygonWay != Z_NONE);

	// =========== initalize arcball =======
	m_arcball = new ArcBall(RenderViewWidth, RenderViewWidth, 0.1f);
}


void Model::cleanup()
{
	m_meshReady = false;
	m_canRender = false;

	m_textureReady = false;
	m_textureFileName.clear();

	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
	}

	if (m_mesh != nullptr)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}

	if (m_shaderProgram != nullptr)
	{
		delete m_shaderProgram;
		m_shaderProgram = nullptr;
	}

	if (m_vao.isCreated())
		m_vao.destroy();

	if (m_vbo.isCreated())
		m_vbo.destroy();

	if (m_arcball != nullptr)
	{
		delete m_arcball;
		m_arcball = nullptr;
	}

}


void Model::draw()
{
	makeCurrent();
	if (!m_canRender){
		update();
		return;
	}

	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);

	// model matrix manage by arcball. One model has one arcball
	m_modelMat = m_arcball->getModelMatrix(m_viewMat);
	// set uniform value
	this->setUniformValue();

	m_shaderProgram->bind();
	m_vao.bind();

	// draw mode
	switch (m_polygonWay)
	{
	case Z_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case Z_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case Z_POINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	if (m_textureReady) glBindTexture(GL_TEXTURE_2D, m_textureID);

	glDrawElements(GL_TRIANGLES, m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, m_mesh->getFaceIndex().constData());
	
	m_vao.release();
	m_shaderProgram->release();
}


// ========= OpenGL context build function =============
bool Model::loadMeshFromFile(QString modelFile)
{
	// is created
	if (m_mesh != nullptr) delete m_mesh;
	
	m_mesh = new Mesh();
	bool ret = m_mesh->buildMesh(modelFile);

	if (ret)
		return true;
	else
	{
		qErrnoWarning("ERROR::MODEL::loadMeshFromFile: buildMesh Failed");
		return false;
	}
}


void Model::buildShaderProgram(QString vertexFile, QString fragmentFile)
{
	// is build
	if (m_shaderProgram != nullptr) delete m_shaderProgram;

	makeCurrent();
	initializeOpenGLFunctions();

	m_shaderProgram = new QOpenGLShaderProgram;
	
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile);
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile);

	// link for compile shader
	m_shaderProgram->link();
}


// build vao and vbo, and initialize arcballl
void Model::buildVAOAndVBO()
{
	if (m_mesh == nullptr) return;

	if (m_shaderProgram == nullptr) return;

	makeCurrent();
	// initial
	if (m_vao.isCreated())
		m_vao.destroy();
	
	if (m_vbo.isCreated())
		m_vbo.destroy();

	// build
	m_shaderProgram->bind();
	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

	// allocate memory for vertex position and normal
	int unitSize = m_mesh->getVertexCount() * sizeof(float);
	int offset = 0;

	if (m_textureReady)
		m_vbo.allocate(unitSize * (3 + 3 + 2));
	else
		m_vbo.allocate(unitSize * (3 + 3 ));

	// position
	glEnableVertexAttribArray(0);
	m_vbo.write(offset, m_mesh->getVertexPos().constData(), unitSize * 3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	offset += unitSize * 3;

	// normal
	glEnableVertexAttribArray(1);
	m_vbo.write(offset, m_mesh->getVertexNormal().constData(), unitSize * 3);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(offset));
	offset += unitSize * 3;

	// texture 
	if (m_textureReady)
	{
		glEnableVertexAttribArray(2);
		m_vbo.write(offset, m_mesh->getTextureCoordinate().constData(), unitSize * 2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(offset));
	}

	m_vbo.release();
	m_vao.release();
	m_shaderProgram->release();	
}


void Model::generateTexture(QString textureFile)
{
	if (m_shaderProgram == nullptr) return;

	makeCurrent();
	m_shaderProgram->bind();
	m_vao.bind();

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_texture = cv::imread(textureFile.toStdString());

	if (m_texture.empty())
		qErrnoWarning("ERROR::IMAGE::setup texture failed");
	else
	{
		// image may align
		glPixelStorei(GL_UNPACK_ALIGNMENT, (m_texture.step & 3) ? 1 : 4);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_texture.step / m_texture.elemSize());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture.cols, m_texture.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_texture.ptr());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	m_vao.release();
	m_shaderProgram->release();
}



// ========= set uniform value ===========
void Model::setCanRender(bool canRender)
{
	m_canRender = canRender && m_meshReady && (m_polygonWay != Z_NONE);
}



void Model::setViewMatValue(glm::mat4 viewMat)
{
	m_viewMat = viewMat;
}


void Model::setProjMatValue(glm::mat4 projMat)
{
	m_projMat = projMat;
}


void Model::setPolygonWay(ZVALUE polygonWay)
{
	m_polygonWay = polygonWay;
	//this->setCanRender();
	m_canRender = m_meshReady && (m_polygonWay != Z_NONE);
}


void Model::setTextureFileName(QString textureFileName)
{
	m_textureFileName = textureFileName;
	m_textureReady = !m_textureFileName.isEmpty() && m_paramInnerType != Z_NONE;

	if (m_textureReady)
	{
		this->attachTexture();
	}
}


void Model::setParameterizationInnerType(ZVALUE innerType)
{
	m_paramInnerType = innerType;
	m_textureReady = !m_textureFileName.isEmpty() && m_paramInnerType != Z_NONE;

	if (m_textureReady)
	{
		this->attachTexture();
	}
}


// ========= arcball ================
void Model::mousePressEvent(QMouseEvent *mouseEvent)
{
	if (!m_canRender)
		return;

	m_arcball->mousePressEvent(mouseEvent);
}


void Model::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (!m_canRender)
		return;

	m_arcball->mouseReleaseEvent(mouseEvent);
}


void Model::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	if (!m_canRender)
		return;

	m_arcball->mouseMoveEvent(mouseEvent);
}


// ========= tools functions ============	
void Model::setUniformValue()
{
	m_shaderProgram->bind();

	glUniformMatrix4fv(m_shaderProgram->uniformLocation("modelMat"), 1, GL_FALSE, glm::value_ptr(m_modelMat));
	glUniformMatrix4fv(m_shaderProgram->uniformLocation("viewMat"), 1, GL_FALSE, glm::value_ptr(m_viewMat));
	glUniformMatrix4fv(m_shaderProgram->uniformLocation("projMat"), 1, GL_FALSE, glm::value_ptr(m_projMat));
	glUniform1i(m_shaderProgram->uniformLocation("hasTexture"), m_textureReady);

	m_shaderProgram->release();
}


void  Model::deleteTexture()
{
	glDeleteTextures(1, &m_textureID);
	m_textureFileName.clear();
	m_textureReady = false;
	m_textureID = 0;
	this->buildVAOAndVBO();
}


void  Model::attachTexture()
{
	m_mesh->parameterizeMesh(Z_SQUARE, m_paramInnerType);
	this->buildVAOAndVBO();
	this->generateTexture(m_textureFileName);
	
	QString fileName;
	if (m_paramInnerType == Z_UNIFORM)
	{
		fileName = "../Data/Model/" + m_modelFileName.split("/").last().split(".").first() + "_" + "uniform.obj";
	}
	else if (m_paramInnerType == Z_SHAP_PRESERVING)
	{
		fileName = "../Data/Model/" + m_modelFileName.split("/").last().split(".").first() + "_" + "shap_preserving.obj";
	}
	m_mesh->dumpParameterizationToFile(fileName);
}