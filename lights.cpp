#include "lights.hpp"

int Lights::index = 0;

Lights::Lights() {
	// camera light
	createLight(QVector4D(0, 0, 0, 1),
				QVector3D(1, 0.8, 0.5),
				QVector3D(1.0, 0.0, 0.0));
	// other lights
	createLight(QVector4D(0, 1000, 0,1),
				QVector3D(0.3, 0.2, 0),
				QVector3D(1.0, 0.0, 0.0));
}

Lights::~Lights() {}

void Lights::createLight(QVector4D position, QVector3D colour, QVector3D falloff) {
	mPositions[index] 	= position;
	mColours[index] 	= colour;
	mFalloffs[index] 	= falloff;

	index++;
}



