#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <glm/glm.hpp>

namespace zcg {

	static const float RenderViewWidth = 1000.0f;
	static const float RenderViewHeight = 800.0f;

	enum MeshPolygonType
	{
		NONE = 0x1,
		FILL = 0x2,
		LINE = 0x3,
		POINT = 0x4,
	};
	
	enum ParameterizationBoundaryType
	{
		SQUARE = 0x5,
		CIRCLE = 0x6,
	};

	enum  ParameterizationInnerType
	{
		AVERAGE = 0x7,
		SHAP_PRESERVING = 0x8,
	};

	
	
	template<typename T> void releaseQVector(QVector<T> &v)
	{
		v.clear();
		v.squeeze();
	}

//	template<typename T> void releaseQVector(QVector<uint> &v);

}

#endif // !UTIL_H
