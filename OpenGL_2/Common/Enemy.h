#pragma once
#pragma once

#include "../header/Angel.h"
#include "CShape.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define PI 3.1415926f
#define RADIUS 0.8
#define SARTRADIUS 1.2
#define CLOUD_NUM 500  // 2 faces, 2 triangles/face 
#define ENEMY_ONE 205
#define VP_HALFWIDTH  12.0f

class Enemy : public CShape
{
private:

public:

	Enemy(int type);


	void draw();
	void drawW();
};



