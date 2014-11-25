#include "sounds.hpp"
#include <iostream>

Sounds::Sounds() {
	footsteps = NULL;
	jump = NULL;

	init();
	loadFiles();
}

Sounds::~Sounds() {
	//Mix_FreeMusic
	Mix_FreeChunk(footsteps);
	Mix_FreeChunk(jump);

	Mix_CloseAudio();
	SDL_Quit();
}



//PRIVATE
void Sounds::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << "Error initializing SDL" << std::endl;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Error initializing SDL mixer" << std::endl;
	}
}

void Sounds::loadFiles() {
	//music = Mix_LoadMUS("...wav");

	footsteps = Mix_LoadWAV("sounds/footsteps.wav");
	jump	  = Mix_LoadWAV("sounds/jump.wav");

	if (footsteps == NULL ||
		jump == NULL) {
		std::cout << "Error loading sound effects" << std::endl;
	}
}


//PUBLIC
void Sounds::playFootsteps() {
	if (!Mix_Playing(0)) {
		Mix_PlayChannel(0, footsteps, 0);
	}
}

void Sounds::playJump() {
	Mix_HaltChannel(0);
	Mix_PlayChannel(1, jump, 0);
}
