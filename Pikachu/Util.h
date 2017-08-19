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

	template<typename T> void printArray(char* arrayName,const T *arrayValue, uint arraySize)
	{
		QString out;
		for (uint i = 0; i < arraySize; i++)
		{
			out += arrayValue[i];
		}
	}

	template<uint> void printArray(char* arrayName, const uint *arrayValue, uint arraySize);

}

#endif // !UTIL_H
