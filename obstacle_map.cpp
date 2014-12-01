#include "obstacle_map.hpp"



ObstacleMap::ObstacleMap() {}

ObstacleMap::~ObstacleMap() {}

void ObstacleMap::setRoot(SceneNode* root) {
	mRoot = root;
	mRoot->setBoundaries();
}

void ObstacleMap::draw() {
	mRoot->walk_gl();
}

void ObstacleMap::moveObjects() {
	mRoot->moveObjects();
}