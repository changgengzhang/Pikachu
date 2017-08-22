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

	static const float RenderViewWidth = 1000.0f;
	static const float RenderViewHeight = 800.0f;
	enum ParameterizationType
	{
		BOUNDARY_SQUARE,
		BOUNDARY_CIRCLE,
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
