#include "cubemap.hpp"
#include <QtOpenGL>
#include <iostream>

CubeMap::CubeMap() {
	mImagePaths[0] = "cubemap/left.png";
	mImagePaths[1] = "cubemap/right.png";
	mImagePaths[2] = "cubemap/down.png";
	mImagePaths[3] = "cubemap/top.png";
	mImagePaths[4] = "cubemap/front.png";
	mImagePaths[5] = "cubemap/back.png";
}

CubeMap::~CubeMap() {}

void CubeMap::load() {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &mTexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	for (int i = 0; i < 6; i++) {
		QImage image = QGLWidget::convertToGLFormat(QImage(mImagePaths[i], "PNG"));

		glTexImage2D(targets[i], 0, GL_RGBA,
						image.width(), image.height(),
						0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}