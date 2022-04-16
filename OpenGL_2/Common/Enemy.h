#pragma once
#pragma once

#include "../header/Angel.h"
#include "CShape.h"
#include "CFlighter.h"
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
	CFlighter* g_Attack[10];
	float g_fAttack[10][3] = { 0 };
	float g_fAttackDir[10] = { 0 };
	float g_AttackInitPos[10][3] = { 0 };

	int _iFree = 10;
	int _iOut = 0;
	int _iNext = 0;
	bool _bEnemyDel = false;
	bool _bAttackOut = false;
	float _fAttackTime = 0;
	float _fAttackSpeed = 2;
	float _fAttackDur = 5.0f;

	void draw();
	void drawW();
};



