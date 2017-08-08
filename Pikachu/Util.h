#ifndef UTIL_H
#define UTIL_H

#include <QtGlobal>
#include <qvector3d.h>

namespace zcg {

	inline QVector3D maxBBOXCoord(QVector3D va, QVector3D vb)
	{
		return QVector3D(
			va.x() > vb.x() ? va.x() : vb.x(),
			va.y() > vb.y() ? va.y() : vb.y(),
			va.z() > vb.z() ? va.z() : vb.z()
		);
	}

	inline QVector3D minBBOXCoord(QVector3D va, QVector3D vb)
	{
		return QVector3D(
			va.x() < vb.x() ? va.x() : vb.x(),
			va.y() < vb.y() ? va.y() : vb.y(),
			va.z() < vb.z() ? va.z() : vb.z()
		);
	}

}

#endif // !UTIL_H
