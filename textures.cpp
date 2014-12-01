#include "textures.hpp"
#include <stdlib.h>
#include <iostream>
#include <QtOpenGL>

Textures::Textures() {}

Textures::~Textures() {}



void Textures::load() {
	// load texture sequentially   						INDEX
	loadImage("textures/wood1.bmp", "BMP");			 	// 0
	loadImage("textures/wood2.jpg", "JPG"); 			// 1
	loadImage("textures/glass.jpg", "JPG");				// 2
	loadImage("textures/toy.jpg", "JPG");				// 3

	loadImage("textures/abstract1.jpg", "JPG"); 		// 4

}

void Textures::loadImage(const char *imagePath, const char *fileType) {
	// cookbook texture page
	QImage image = QGLWidget::convertToGLFormat(QImage(imagePath, fileType));

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	mIDs.push_back(textureID);
}
