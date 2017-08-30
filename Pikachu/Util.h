#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <glm/glm.hpp>

namespace zcg {

	static const float RenderViewWidth = 1000.0f;
	static const float RenderViewHeight = 900.0f;

	static const float Z_EPSILON = 0.001;

	typedef	int ZVALUE;
	static const int Z_NONE = 0x0;

	static const int Z_FILL = 0x1;
	static const int Z_LINE = 0x2;
	static const int Z_POINT = 0x3;

	static const int Z_SQUARE = 0x4;
	static const int Z_CIRCLE = 0x5;

	static const int Z_UNIFORM = 0x6;
	static const int Z_SHAP_PRESERVING = 0x7;

	static const int Z_1D = 0x8;
	static const int Z_2D = 0x9;
	static const int Z_3D = 0xA;

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
