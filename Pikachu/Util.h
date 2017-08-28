#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <glm/glm.hpp>

namespace zcg {

	static const float RenderViewWidth = 1000.0f;
	static const float RenderViewHeight = 800.0f;


	typedef	int ZVALUE;
	static const int NONE = 0x0;

	static const int Z_FILL = 0x0;
	static const int Z_LINE = 0x0;
	static const int Z_POINT = 0x0;

	static const int Z_SQUARE = 0x0;
	static const int Z_CIRCLE = 0x0;

	static const int Z_AVERAGE = 0x0;
	static const int Z_SHAP_PRESERVING = 0x0;

	static const int Z_1D = 0x0;
	static const int Z_2D = 0x0;
	static const int Z_3D = 0x0;

	enum MeshPolygonType
	{
		NONE_POLYGON_TYPE = 0x0,
		FILL = 0x1,
		LINE = 0x2,
		POINT = 0x3,
	};
	
	enum ParameterizationBoundaryType
	{
		SQUARE = 0x4,
		CIRCLE = 0x5,
	};

	enum  ParameterizationInnerType
	{
		NONE_INNER_TYPE = 0x6,
		AVERAGE = 0x7,
		SHAP_PRESERVING = 0x8,
	};

	enum SpatialDimension
	{
		D1 = 0x9,
		D2 = 0xA,
		D3 = 0xB
	};

	enum FileType
	{
		MODEL,
		TEXTURE
	};
	
	
	template<typename T> void releaseQVector(QVector<T> &v)
	{
		v.clear();
		v.squeeze();
	}

//	template<typename T> void releaseQVector(QVector<uint> &v);

	template<typename T> inline bool deleteIfNotNull(T *p)
	{
		if (p != nullptr)
		{
			delete p;
			return true;
		}
		return false;
	}

}

#endif // !UTIL_H
