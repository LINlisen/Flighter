#pragma once
#include "CShape.h"
#include "../header/Angel.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define PI 3.1415926f
#define RADIUS 0.8
#define SARTRADIUS 1.2
#define ATTACK_NUM 400
#define VP_HALFWIDTH  12.0f
class CEnemyAttack :CShape
{
private:
	vec4 vColor;
public:

	CEnemyAttack(int type);

	void StartAttack(CEnemyAttack* Ea,int type,vec3 Pos,float delta);
	void DeleteAttack(int type, int index);
	void draw();
	void drawW();
};