#include "textures.hpp"
#include <stdlib.h>
#include <iostream>
#include <QtOpenGL>

Textures::Textures() {}

Textures::~Textures() {}



void Textures::load() {
	// load texture sequentially   		INDEX
	loadBMP("textures/wood1.bmp"); 		// 0
	loadJPG("textures/wood2.jpg"); 		// 1

	loadJPG("textures/abstract1.jpg"); 	// 2
	loadJPG("textures/abstract2.jpg"); 	// 3
}

void Textures::loadBMP(const char *imagePath) {
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

	unsigned char header[54]; // 54 byte header
	unsigned int dataPos; // data offset
	unsigned int width, height;
 	unsigned int imageSize; // width * height * 3

	unsigned char * data; //RGB data

	FILE *file = fopen(imagePath,"rb");
	if (!file) {
		std::cout << "Image could not be opened" << std::endl;
	}

	if (fread(header, 1, 54, file) != 54 ||
		header[0] != 'B' || header[1] != 'M') {
		std::cout << "Not a BMP file" << std::endl;
	}

	dataPos 	= *(int*)&(header[0x0A]);
	imageSize 	= *(int*)&(header[0x22]);
	width 		= *(int*)&(header[0x12]);
	height 		= *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width*height*3;
	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	std::cout << "texture id: " << textureID << "    path: " << imagePath << std::endl;
	mTexIDs.push_back(textureID);
}

void Textures::loadJPG(const char *imagePath) {
	QImage image = QGLWidget::convertToGLFormat(QImage(imagePath, "JPG"));

	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	std::cout << "texture id: " << textureID << "    path: " << imagePath << std::endl;
	mTexIDs.push_back(textureID);
}