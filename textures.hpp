#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <GL/glu.h>
#include <vector>

class Textures {
public:
	Textures();
	virtual ~Textures();

	void load();

	std::vector<GLuint> mTexIDs;
private:
	void loadBMP(const char *imagePath);
	void loadJPG(const char *imagePath);
};

#endif