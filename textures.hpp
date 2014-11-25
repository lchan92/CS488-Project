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
	void loadImage(const char *imagePath, const char *fileType);
};

#endif