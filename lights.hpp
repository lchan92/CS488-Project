#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include <QVector3D>
#include <QVector4D>

#define NUM_LIGHTS 3

class Lights {
public:
	Lights();
	virtual ~Lights();

	QVector4D mPositions[NUM_LIGHTS];
	QVector3D mColours[NUM_LIGHTS];
	QVector3D mFalloffs[NUM_LIGHTS];
private:
	static int index;

	void createLight(QVector4D position, QVector3D colour, QVector3D falloff);
};


#endif