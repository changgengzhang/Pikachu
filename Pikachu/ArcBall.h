#ifndef ARCBALL_H
#define ARCBALL_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

class ArcBall
{
public:
	ArcBall(float windowWidth, float windowHeight, float mouseSensitivity);
	~ArcBall();

private:
	glm::vec3 toSphereCoord(float x, float y);

private:
	float m_windowWidth;
	float m_windowHeight;
	float m_adjustCoord;
};

#endif
