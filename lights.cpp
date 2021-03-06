#include "lights.hpp"

int Lights::index = 0;

Lights::Lights() {
	// camera light
	createLight(QVector4D(0, 0, 0, 1),
				QVector3D(0.5, 0.3, 0),
				QVector3D(1.0, 0.0, 0.0));
	
	// sun light
	createLight(QVector4D(0, 1000, 0,1),
				QVector3D(0.5, 0.3, 0.2),
				QVector3D(1.0, 0.0, 0.0));
	// water light
	createLight(QVector4D(0,-500, 500,1),
				QVector3D(0, 0.2, 0.7),
				QVector3D(1.0, 0.0, 0.0));

}

Lights::~Lights() {}

void Lights::createLight(QVector4D position, QVector3D colour, QVector3D falloff) {
	mPositions[index] 	= position;
	mColours[index] 	= colour;
	mFalloffs[index] 	= falloff;

	index++;
}

void Lights::resetPositions() {
	mPositions[0] = QVector4D(0,0,0,1);
}



