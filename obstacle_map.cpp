#include "obstacle_map.hpp"



ObstacleMap::ObstacleMap() {}

ObstacleMap::~ObstacleMap() {}

void ObstacleMap::setRoot(SceneNode* root) {
	mRoot = root;
}

void ObstacleMap::draw() {
	mRoot->walk_gl();
}