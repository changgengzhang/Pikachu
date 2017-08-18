#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <glm/glm.hpp>

namespace zcg {
	enum MeshPolygonWay
	{
		NONE,
		FILL,
		LINE,
		POINT,
	};

	inline glm::vec3 MaxBBOXCoord(glm::vec3 va, glm::vec3 vb)
	{
		return glm::vec3(
			va.x > vb.x ? va.x : vb.x,
			va.y > vb.y ? va.y : vb.y,
			va.z > vb.z ? va.z : vb.z
		);
	}

	inline glm::vec3 MinBBOXCoord(glm::vec3 va, glm::vec3 vb)
	{
		return glm::vec3(
			va.x < vb.x ? va.x : vb.x,
			va.y < vb.y ? va.y : vb.y,
			va.z < vb.z ? va.z : vb.z
		);
	}

	template<typename T> void printArray(const T *array, uint size)
	{
		for (uint i = 0; i < size; i++)
		{
			qDebug() << array[i];
		}
	}

	template<uint> void printArray(const uint *array, uint size);

}

#endif // !UTIL_H
