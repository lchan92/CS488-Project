#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <GL/glu.h>

class CubeMap {
public:
	CubeMap();
	virtual ~CubeMap();

	void load();

	GLuint mTexID;
private:
	const char* mImagePaths[6];
};


#endif