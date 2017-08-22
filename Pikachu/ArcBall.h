#ifndef ARCBALL_H
#define ARCBALL_H

#include <qobject.h>
#include <qglobal.h>
#include <qdebug.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

class ArcBall : public QObject
{
	Q_OBJECT
public:
	ArcBall(float windowWidth, float windowHeight, float mouseSensitivity);
	~ArcBall();
	void mousePressCallback(Qt::MouseButton mouseButton, float x, float y);
	void mouseReleaseCallback();
	void mouseMoveCallback(float x, float y);
	void wheelEventCallback(float zoomValue);

	glm::mat4 getModelMatrix(glm::mat4& viewMatrix);
	
private:
	glm::vec3 mapToSphereCoord(float x, float y);

private:
	float m_windowWidth;
	float m_windowHeight;
	float m_adjustCoord;
	float m_mouseSensitivity;
	
	Qt::MouseButton m_mouseButton;
	glm::vec2 m_preScrCoord;
	glm::vec2 m_currScrCoord;

	float m_angle;
	glm::vec3 m_rotaryAxis;

	glm::vec3 m_panValue;

	glm::mat4 m_modelMat;


};

#endif
