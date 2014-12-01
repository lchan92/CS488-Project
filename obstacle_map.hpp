#ifndef OBSTACLE_MAP_HPP
#define OBSTACLE_MAP_HPP

#include <iostream>
#include "scene.hpp"

class ObstacleMap {
public:
	ObstacleMap();
	virtual ~ObstacleMap();

	void setRoot(SceneNode* root);

	void draw();
	void moveObjects();

private:
	// hierarchical tree to store the obstacle blocks
	SceneNode *mRoot;
};

#endif