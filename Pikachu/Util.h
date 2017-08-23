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
	
	template<typename T> void releaseQVector(QVector<T> &v)
	{
		v.clear();
		v.squeeze();
	}

//	template<typename T> void releaseQVector(QVector<uint> &v);

}

#endif // !UTIL_H
