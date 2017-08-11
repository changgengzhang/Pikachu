#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <glm/glm.hpp>

namespace zcg {
	enum MeshDisplayWay
	{
		FILL,
		LINE,
		POINT,
	};

	inline glm::vec3 maxBBOXCoord(glm::vec3 va, glm::vec3 vb)
	{
		return glm::vec3(
			va.x > vb.x ? va.x : vb.x,
			va.y > vb.y ? va.y : vb.y,
			va.z > vb.z ? va.z : vb.z
		);
	}

	inline glm::vec3 minBBOXCoord(glm::vec3 va, glm::vec3 vb)
	{
		return glm::vec3(
			va.x < vb.x ? va.x : vb.x,
			va.y < vb.y ? va.y : vb.y,
			va.z < vb.z ? va.z : vb.z
		);
	}


}

#endif // !UTIL_H
