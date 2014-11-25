#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sounds {
public:
	Sounds();
	virtual ~Sounds();

	void playFootsteps();
	void playJump();
private:
	void init();
	void loadFiles();

	Mix_Chunk *footsteps, *jump;
};

#endif