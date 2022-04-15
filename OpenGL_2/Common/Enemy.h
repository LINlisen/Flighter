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
	Enemy* g_Attack[20];
	float g_fAttack[20][3];
	float g_fAttackDir[20];
	float g_AttackInitPos[20][3];

	int _iFree = 20;
	int _iOut = 0;
	int _iNext = 0;
	bool _bEnemyDel;
	float _fAttackTime = 0;
	float _fAttackSpeed = 5;
	float _fAttackDur = 0.5f;
	Enemy(int type);


	void Attack(float delta);

	void draw();
	void drawW();
};



