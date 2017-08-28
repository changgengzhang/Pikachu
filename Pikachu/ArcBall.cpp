#include "ArcBall.h"



ArcBall::ArcBall(float windowWidth, float windowHeight, float mouseSensitivity = 0.1)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_adjustCoord = m_windowWidth < m_windowHeight ? m_windowWidth : m_windowHeight;
	m_mouseSensitivity = mouseSensitivity;


	m_angle = 0.0f;
	m_rotaryAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_panValue = glm::vec3(0.0f);
	m_modelMat = glm::mat4(1.0f);
}


ArcBall::~ArcBall()
{
}


glm::vec3 ArcBall::mapToSphereCoord(float x, float y)
{
	glm::vec3 coord(0.0f, 0.0f, 0.0f);

	coord.x = (2 * x - m_windowWidth) / m_adjustCoord;
	coord.y = -(2 * y - m_windowHeight) / m_adjustCoord;

	coord.x = glm::clamp(coord.x, -1.0f, 1.0f);
	coord.y = glm::clamp(coord.y, -1.0f, 1.0f);

	float length = coord.x * coord.x + coord.y * coord.y;

	if (length <= 1.0f)
	{
		coord.z = sqrt(1.0f - length);
	}
	else
	{
		coord = glm::normalize(coord);
	}

	return coord;
}


void ArcBall::mousePressEvent(QMouseEvent *mouseEvent)
{
	float x = mouseEvent->x();
	float y = mouseEvent->y();
	m_preScrCoord = glm::vec2(x, y);
}


void ArcBall::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	m_angle = 0.0f;
	m_rotaryAxis = glm::vec3(0.0f, 1.0f, 0.0f);
}


void ArcBall::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	float x = mouseEvent->x();
	float y = mouseEvent->y();

	m_currScrCoord = glm::vec2(x, y);
	
	if (mouseEvent->buttons() & Qt::LeftButton)
	{
		glm::vec3 prePosition, currPosition;
		prePosition = this->mapToSphereCoord(m_preScrCoord.x, m_preScrCoord.y);
		currPosition = this->mapToSphereCoord(m_currScrCoord.x, m_currScrCoord.y);

		if (glm::length(prePosition - currPosition) < std::numeric_limits<float>::epsilon())
		{
			m_angle = 0.0f;
		}
		else
		{
			m_angle = acos(std::min(1.0f, glm::dot(currPosition, prePosition)));
			m_rotaryAxis = glm::cross(prePosition, currPosition);
		}

		m_preScrCoord = m_currScrCoord;
	}

	if (mouseEvent->buttons() & Qt::RightButton)
	{

	}

}


void ArcBall::wheelEventCallback(float zoomValue)
{


}

glm::mat4 ArcBall::getModelMatrix(glm::mat4& viewMatrix)
{
	glm::vec3 axis = glm::inverse(glm::mat3(viewMatrix)) * m_rotaryAxis;
	m_modelMat = glm::rotate(glm::degrees(m_angle) * m_mouseSensitivity * 0.1f, axis) * m_modelMat;
	
	return m_modelMat;
}