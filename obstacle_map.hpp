#ifndef OBSTACLE_MAP_HPP
#define OBSTACLE_MAP_HPP

#include <iostream>
#include "scene.hpp"

class ObstacleMap {
public:
	ObstacleMap();
	virtual ~ObstacleMap();

private:
	// hierarchical tree to store the obstacle blocks
	SceneNode *root;
};

#endif