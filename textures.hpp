#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <GL/glu.h>
#include <vector>

class Textures {
public:
	Textures();
	virtual ~Textures();

	void load();
	void loadImage(const char *imagePath, const char *fileType);

	std::vector<int> mIDs;
};

#endif