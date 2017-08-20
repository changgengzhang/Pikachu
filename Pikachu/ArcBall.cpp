#include "ArcBall.h"



ArcBall::ArcBall(float windowWidth, float windowHeight, float mouseSensitivity)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_adjustCoord = m_windowWidth < m_windowHeight ? m_windowWidth : m_windowHeight;
}


ArcBall::~ArcBall()
{
}


glm::vec3 ArcBall::toSphereCoord(float x, float y)
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