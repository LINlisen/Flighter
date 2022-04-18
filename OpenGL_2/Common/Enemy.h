#pragma once
#pragma once

#include "../header/Angel.h"
#include "CShape.h"
#include "CFlighter.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define PI 3.1415926f
#define RADIUS 0.4
#define SARTRADIUS 1.2
#define CLOUD_NUM 500  // 2 faces, 2 triangles/face 
#define ENEMY_ONE 205
#define ENEMY_TWO 210
#define ENEMY_TWO_RADIUS 1.6
#define VP_HALFWIDTH  12.0f
#define ATTACK_NUM 5
class Enemy : public CShape
{
private:

public:
	Enemy(int type);
	CFlighter* g_Attack[ATTACK_NUM];
	float g_fAttack[ATTACK_NUM][3] = { 0 };
	float g_fAttackDir[ATTACK_NUM] = { 0 };
	float g_AttackInitPos[ATTACK_NUM][3] = { 0 };

	int _iFree = ATTACK_NUM;
	int _iOut = 0;
	int _iNext = 0;
	bool _bEnemyDel = false;
	bool _bAttackOut = false;
	bool _bAttackSus[ATTACK_NUM] = { false };
	float _fAttackTime = 0;
	float _fAttackSpeed = 5;
	float _fAttackDur = 2.0f;

	void draw();
	void drawW();
};



