// The parent-child relationship 
// 水平移動紅色方塊繞著綠色方塊進行逆時針方向旋轉
// 上下移動藍色方塊繞著紅色方塊進行逆時針方向旋轉

#include "header/Angel.h"
#include "Common/CFlighter.h"
#include "Common/CBackGround.h"
#include "Common/Enemy.h"
#include <iostream>
#include <random>
#include <iomanip>

using std::cout;
using std::endl;
using std::setprecision;

constexpr int FLOAT_MIN = 10;
constexpr int FLOAT_MAX = 100;

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE (SCREEN_SIZE/2) 
#define VP_HALFWIDTH  12.0f
#define FLIGHTER_SIZE 4 
#define PFLIGHTER_SIZE 2 
#define GDISTANCE 5
#define BDISTANCE 3

//my code 
#define FBASE 6
//for Flighter
CFlighter* g_Player[FLIGHTER_SIZE];
float g_fPlayer[FLIGHTER_SIZE][3];
float _fChangeClock = 0;
mat4 g_initmxS[4];
mat4 g_initmxT[4];
mat4 g_Flightercenter = Translate(-0.8f, -1.4f, 0.0f);
bool _bAttacked = false;
//for five star to protect flighter
CFlighter* g_FiveStar;
float g_fFiveStar[3];
mat4 g_finitmxT;
GLfloat g_fFAngle = 0;
GLfloat g_fFDir = 1;

//for missile
CFlighter* g_Missile[20];
float g_fMissile[20][3];
float g_fShootDir[20];
mat4 g_minitmxT;
mat4 g_Shootercenter = Translate(-0.8f, 0.0f, 0.0f);
float g_shootMousePos[20][3];
int _iFree = 20;
int _iOut = 0;
int _iNext = 0;
float _fShootTime = 0;
float _fShootSpeed = 5;
float _fShootDur = 0.5f;
bool _bMissileSus[20] = { false };
//for cloud
CBackGround* g_Cloud[4];
float g_fCloud[4][3];
int g_CloudType[4];
int g_fSpeedCloud[4];

//for 2-1 2-2
//First Enemy
Enemy* g_Enemy[10];
int g_EnemyType[10];
int _iGenCount;
int _iInverse[10] = { 1 };
int _iDieCount[10];
float g_fEnemy[10][3];
float g_fEnemyDir[10][3];
float g_fEnemyCount[10];
float EnemyTime = 0.0f;
bool _bEnemyGen[10] = { false };
bool _bEnemyDel[10] = { false };
//Second Enemy
Enemy* g_Enemy_S[5];
int g_EnemyType_S[5];
int _iGenCount_S;
int _iInverse_S[5] = { 1 };
int _iDieCount_S[5];
float g_fEnemy_S[5][3];
float g_fEnemyDir_S[5][3];
float g_fEnemyCount_S[5];
float EnemyTime_S = 0.0f;
float g_fEnemy_S_Angle[5] = { 0.0f };
float g_fTraceDir[5][2] = { 0.0f };
bool _bEnemyGen_S[5] = { false };
bool _bEnemyDel_S[5] = { false };
//Third Enemy
Enemy* g_Enemy_T[5];
int g_EnemyType_T[5];
int _iGenCount_T;
int _iInverse_T[5] = { 1 };
int _iDieCount_T[5];
float g_fEnemy_T[6][3];
float g_fEnemyDir_T[5][3];
float g_fEnemyCount_T[5];
float EnemyTime_T = 0.0f;
bool _bEnemyGen_T[5] = { false };
bool _bEnemyDel_T[5] = { false };
bool _bSetTrace[5][5] = { false };
//Boss
Enemy* g_Boss[9];
int _iBossType = 1;
int _iBeAttackCount;
float g_fBoss[9][3];
float g_fGenBossClock;
float g_fScaleClock;
float g_fFirstDiedPos[2];
float g_fSecondDiedPos[2];
bool _bBossGen = false;
bool _bBossOpen = false;
bool _bFirstDied = false;
bool _bSecondDied = false;
//for 3-2
CFlighter* g_UpgradeOne[2];
CFlighter* g_UpgradeTwo;
CFlighter* g_UpgradeThree;
float g_fUpgradeOne[2][3];
float g_fUpgradeTwo[3];
float g_fUpgradeThree[3];
float _fGearClock = 0;
float _fColdClock = 0;
float _fGearAngle = 0;
float _fTraceClock = 0;
float _fTraceCold = 0;
float _fTraceDir[2];
float _fTraceCount;
bool _bUpgrade[3] = { false };
bool _bUseGear = false;
bool _bGearCold = false;
bool _bUseTrace = false;
bool _bTraceCold = false;
//for 3-2 eat something change
CFlighter* g_ChangeEat[3];
float g_fChangeEat[3][3];
float CountTime;
float g_fEatDir[3][3];
bool g_bGenerate[3] = { false };
bool G_bGenDel[3] = { false };

//for mouse move
mat4 mxGT;

// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxIdentity(1.0f);
mat4 g_mxProjection;



// Mouse motion
GLfloat g_fTx = 0, g_fTy = 0;

//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();
void CreateQuadRelationship(); //1-1
void ProtectRotation(float); //1-2
void TraceAttack(float);
void Shoot(float); //1-3 1-4
void CloudMove(float); //1-5
void EatChange_Generate(int);//3-2
void EatChangeMove(int , float);//4-1
void EnemyGen(int,int);
void EnemyMove(int,int,float);
void Attack(float,int,Enemy*,int);
float getRandomf(float _max, float _min); //get romdon float
int getRandomi(int _max, int _min);
void BeAttack(float);
void BossMove(float,int);
void init(void)
{
	//  產生 projection 矩陣，此處為產生正投影矩陣
	g_mxProjection = Ortho(-VP_HALFWIDTH, VP_HALFWIDTH, -VP_HALFWIDTH, VP_HALFWIDTH, -1.0f, 1.0f);
	// 必須在 glewInit(); 執行完後,在執行物件實體的取得
	CreateQuadRelationship();
	glClearColor(0.2, 0.75f, 0.85f, 1.0); // black background


}

//----------------------------------------------------------------------------

void GL_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the window
	for (int i = 0; i < 4; i++) {
		g_Cloud[i]->draw(g_CloudType[i]);
	}
	for (int i = 0; i < _iOut; i++) {
		g_Missile[i]->draw(5);
	}
	for (int i = 0; i < 3; i++) {
		if (g_bGenerate[i] && !G_bGenDel[i]) {
			g_ChangeEat[i]->draw(3);
		}
	}
	for (int i = 0; i < FLIGHTER_SIZE; i++) {
		if(_fChangeClock == 0)
			g_Player[i]->draw(1);
	}
	for (int i = 0; i < 3; i++) {
		if (_bUpgrade[i]) {
			switch (i)
			{
			case 0:
				if (_fChangeClock == 0) {
					g_UpgradeOne[0]->draw(6);
					g_UpgradeOne[1]->draw(6);
				}
				break;
			case 1:
				if (_fChangeClock == 0) {
					g_UpgradeTwo->draw(8);
				}
				break;
			case 2:
				if (_fChangeClock == 0) {
					g_UpgradeThree->draw(4);
				}
				break;
			}
		}
	}
	for (int i = 0; i < _iGenCount; i++) {
		if (_bEnemyGen[i] && !(_bEnemyDel[i])) {
			g_Enemy[i]->draw();
		}
		for (int j = 0; j < g_Enemy[i]->_iOut; j++) {
			if (!g_Enemy[i]->_bAttackOut) {
				if(g_Enemy[i]!=nullptr && !g_Enemy[i]->_bAttackSus[j])
					g_Enemy[i]->g_Attack[j]->draw(7);
				//printf("%d:%d draw\n", i, j);
			}
		}
	}
	for (int i = 0; i < _iGenCount_S; i++) {
		if (_bEnemyGen_S[i] && !(_bEnemyDel_S[i])) {
			g_Enemy_S[i]->draw();
			//printf("%d:(%f,%f)\n", i, g_Enemy_S[i]->getPos().x, g_Enemy_S[i]->getPos().y);
		}
		for (int j = 0; j < g_Enemy_S[i]->_iOut; j++) {
			if (!g_Enemy_S[i]->_bAttackOut) {
				if (g_Enemy_S[i] != nullptr && g_Enemy_S[i]->g_Attack[j] != nullptr && !g_Enemy_S[i]->_bAttackSus[j])
					g_Enemy_S[i]->g_Attack[j]->draw(7);
				//printf("%d:%d draw\n", i, j);
			}
		}
	}
	for (int i = 0; i < _iGenCount_T; i++) {
		if (_bEnemyGen_T[i] && !(_bEnemyDel_T[i])) {
			g_Enemy_T[i]->draw();
			//printf("%d:(%f,%f)\n", i, g_Enemy_S[i]->getPos().x, g_Enemy_S[i]->getPos().y);
		}
		for (int j = 0; j < g_Enemy_T[i]->_iOut; j++) {
			if (!g_Enemy_T[i]->_bAttackOut) {
				if (g_Enemy_T[i] != nullptr && g_Enemy_T[i]->g_Attack[j] != nullptr && !g_Enemy_T[i]->_bAttackSus[j])
					g_Enemy_T[i]->g_Attack[j]->draw(7);
				//printf("%d:%d draw\n", i, j);
			}
		}
	}
	if (_bBossGen) {
		g_Boss[0]->draw();
		if (_iBossType == 1) {
			for (int i = 5; i < 9; i++) {
				g_Boss[i]->draw();
				if (!_bFirstDied) {
					for (int j = 0; j < g_Boss[i]->_iOut; j++) {
						g_Boss[i]->g_Attack[j]->draw(7);
					}
				}
			}
		}
		if (_iBossType == 1 || _iBossType == 2) {
			for (int i = 1; i < 5; i++) {
				g_Boss[i]->draw();
				if (!_bSecondDied) {
					for (int j = 0; j < g_Boss[i]->_iOut; j++) {
						g_Boss[i]->g_Attack[j]->draw(7);
					}
				}
			}
		}

	}
	g_FiveStar->draw(4);
	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	if (!_bUseTrace) {
		ProtectRotation(delta);
	}
	else {
		TraceAttack(delta);
	}
	Shoot(delta);
	CloudMove(delta);
	//for 3-2 eat change
	CountTime += delta;
	EnemyTime += delta;
	EnemyTime_S += delta;
	EnemyTime_T += delta;
	g_fGenBossClock += delta;
	//Player be Attacked 
	if (_bAttacked) {
		BeAttack(delta);
	}
	if (CountTime > 2.0f && g_bGenerate[0] == false) {
		EatChange_Generate(0);
	}
	if (CountTime > 15.0f && g_bGenerate[1] == false) {
		EatChange_Generate(1);
	}
	if (CountTime > 30.0f && g_bGenerate[2] == false) {
		EatChange_Generate(2);
	}
	for (int i = 0; i < 3; i++) {
		if (g_ChangeEat[i] == nullptr) break;
		if (g_bGenerate[i] && G_bGenDel[i] == false) {
			EatChangeMove(i, delta);
			switch (i)
			{
			case 0:
				if (g_ChangeEat[i]->CheckCollider(g_Player[0]->getPos().x, g_Player[0]->getPos().y, 0.4f)) {
					G_bGenDel[i] = true;
					delete g_ChangeEat[i];
					_bUpgrade[i] = true;
					_fShootSpeed = 15;
					_fShootDur = 0.4f;
				}
			break;
			case 1:
				if (g_ChangeEat[i]->CheckCollider(g_Player[0]->getPos().x, g_Player[0]->getPos().y, 0.4f)) {
					G_bGenDel[i] = true;
					delete g_ChangeEat[i];
					_bUpgrade[i] = true;
				}
			break;
			case 2:
				if (g_ChangeEat[i]->CheckCollider(g_Player[0]->getPos().x, g_Player[0]->getPos().y, 0.4f)) {
					G_bGenDel[i] = true;
					delete g_ChangeEat[i];
					_bUpgrade[i] = true;
				}
				break;
			}
			
		}
	}
	//use gear
	if (_bUseGear && !_bGearCold) {
		mat4 mxR , mxT;
		_fGearClock += delta;
		if (_fGearClock < 5.0f) {
			_fGearAngle = _fGearClock * 1800;
			if (_fGearAngle == 360.0f) _fGearAngle -= 360.0f;
			else if (_fGearAngle == -360.0f)  _fGearAngle += 360.0f;
			mxR = RotateZ(_fGearAngle);
			mxT = Translate(g_fUpgradeTwo[0], g_fUpgradeTwo[1], g_fUpgradeTwo[2]);
			g_UpgradeTwo->setTRSMatrix(mxT * mxGT * mxR);
			vec4 vColor = vec4(_fGearClock / 5.0f, 0, 0, 1);
			g_UpgradeTwo->setColor(vColor, 8);
			g_UpgradeTwo->setPos(vec3(g_fUpgradeTwo[0] + g_fTx, g_fUpgradeTwo[1] + g_fTy, g_fUpgradeTwo[2]));
		}
		else {
			_fGearClock = 0;
			_bUseGear = false;
			_bGearCold = true;
		}
	}
	if (_bGearCold) {
		_fColdClock += delta;
		vec4 vColor = vec4((1 - _fColdClock / 10.0f), 0, 0, 1);
		g_UpgradeTwo->setColor(vColor, 8);
		if (_fColdClock > 10.0f) {
			_fColdClock = 0.0f;
			_bGearCold = false;
		}
	}
	//First Enemy Generate
	if (EnemyTime > 2.0f && !_bEnemyGen[_iGenCount] && _iGenCount<10) {

		EnemyGen(_iGenCount, 1);
		_iGenCount++;
		EnemyTime -= 2.0f;
	}
	for (int i = 0; i < _iGenCount; i++) {
		if (g_Enemy[i] == nullptr) break;
		if (_bEnemyGen[i] && _bEnemyDel[i] == false) {
			g_Enemy[i]->_fAttackDur = getRandomf(5.0f,1.0f);
			EnemyMove(1,i,delta);
			for (int j = 0; j < _iOut; j++) {
				bool check = g_Enemy[i]->CheckCollider(g_Missile[j]->getPos().x, g_Missile[j]->getPos().y, 1.0f);
				bool checkGear = g_Enemy[i]->CheckCollider(g_UpgradeTwo->getPos().x, g_UpgradeTwo->getPos().y, 2.0f);
				if (check) {
					_iDieCount[i]++;
				}
				if (_iDieCount[i] == 2 || (checkGear && _bUseGear)) {
					_bEnemyDel[i] = true;
					g_Enemy[i]->_bEnemyDel = true;
					if(g_Enemy[i]->_bAttackOut) delete g_Enemy[i];
					_iDieCount[i] = 0;
				}
			}
			//Attack(delta);
		}
		if (!g_Enemy[i]->_bAttackOut) {
			Attack(delta,i, g_Enemy[i],1);
		}
	}
	//Second Enemy Generate
	if (EnemyTime_S > getRandomf(10.0f,5.0f) && !_bEnemyGen_S[_iGenCount_S] && _iGenCount_S < 5) {

		EnemyGen(_iGenCount_S, 2);
		_iGenCount_S++;
		EnemyTime_S = 0.0f;
	}
	for (int i = 0; i < _iGenCount_S; i++) {
		if (g_Enemy_S[i] != nullptr) {
			if (_bEnemyGen_S[i] && _bEnemyDel_S[i] == false) {
				g_Enemy_S[i]->_fAttackDur = getRandomf(3.0f, 1.0f);
				EnemyMove(2, i, delta);
				for (int j = 0; j < _iOut; j++) {
					bool check = g_Enemy_S[i]->CheckCollider(g_Missile[j]->getPos().x, g_Missile[j]->getPos().y, 1.0f);
					bool checkGear = g_Enemy_S[i]->CheckCollider(g_UpgradeTwo->getPos().x, g_UpgradeTwo->getPos().y, 2.0f);
					if (check) {
						_iDieCount_S[i]++;
					}
					if (_iDieCount_S[i] == 2 || (checkGear && _bUseGear)) {
						_bEnemyDel_S[i] = true;
						g_Enemy_S[i]->_bEnemyDel = true;
						if (g_Enemy_S[i]->_bAttackOut) delete g_Enemy_S[i];
						_iDieCount_S[i] = 0;
					}
				}
			}
			if (!g_Enemy_S[i]->_bAttackOut && g_Enemy_S[i]!= nullptr) {
				Attack(delta, i, g_Enemy_S[i],2);
			}
		}
	}
	//Third Enemy Gen and Attack
	if (EnemyTime_T > getRandomf(20.0f, 10.0f) && !_bEnemyGen_T[_iGenCount_T] && _iGenCount_T < 5) {

		EnemyGen(_iGenCount_T, 3);
		_iGenCount_T++;
		EnemyTime_T = 0.0f;
		printf("Gen 3\n");
	}
	for (int i = 0; i < _iGenCount_T; i++) {
		if (g_Enemy_T[i] != nullptr) {
			if (_bEnemyGen_T[i] && _bEnemyDel_T[i] == false) {
				g_Enemy_T[i]->_fAttackDur = getRandomf(3.0f, 1.0f);
				//EnemyMove(2, i, delta);
				for (int j = 0; j < _iOut; j++) {
					bool check = g_Enemy_T[i]->CheckCollider(g_Missile[j]->getPos().x, g_Missile[j]->getPos().y, 1.0f);
					bool checkGear = g_Enemy_T[i]->CheckCollider(g_UpgradeTwo->getPos().x, g_UpgradeTwo->getPos().y, 2.0f);
					if (check) {
						_iDieCount_T[i]++;
					}
					if (_iDieCount_T[i] == 2 || (checkGear && _bUseGear)) {
						_bEnemyDel_T[i] = true;
						g_Enemy_T[i]->_bEnemyDel = true;
						if (g_Enemy_T[i]->_bAttackOut) delete g_Enemy_T[i];
						_iDieCount_T[i] = 0;
					}
				}
				//Attack(delta);
			}
			if (!g_Enemy_T[i]->_bAttackOut && g_Enemy_T[i] != nullptr) {
				Attack(delta, i, g_Enemy_T[i], 3);
			}
		}

	}
	//boss
	if (g_fGenBossClock>20.0f && !_bBossGen) {
		_bBossGen = true;
		g_Boss[0] = new Enemy(3);
		g_fBoss[0][0] = 0.0f; g_fBoss[0][1] = 5.0f; g_fBoss[0][2] = 0.0f;
		vec4 vColor = vec4(1, 0, 0, 1);
		g_Boss[0]->setColor(vColor);
		mat4 mxT = Translate(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]);
		g_Boss[0]->setTRSMatrix(mxT);
		g_Boss[0]->setShader(g_mxModelView, g_mxProjection);
		for (int i = 1; i < 5; i++) {
			g_Boss[i] = new Enemy(1);
			switch (i)
			{
			case 1:
				g_fBoss[i][0] = 3.0f; g_fBoss[i][1] = 0; g_fBoss[i][2] = 0.0f;
				break;
			case 2:
				g_fBoss[i][0] = 0; g_fBoss[i][1] = - 3.0f; g_fBoss[i][2] = 0.0f;
				break;
			case 3:
				g_fBoss[i][0] = -3.0f; g_fBoss[i][1] =0; g_fBoss[i][2] = 0.0f;
				break;
			case 4:
				g_fBoss[i][0] = 0; g_fBoss[i][1] =  3.0f; g_fBoss[i][2] = 0.0f;
				break;
			}
			vec4 vColor = vec4(i*0.2, i*0.25, i*0.2, 1);
			g_Boss[i]->setColor(vColor);
			mat4 mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			mat4 mxCT = Translate(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]);
			g_Boss[i]->setTRSMatrix(mxCT*mxT);
			g_Boss[i]->setShader(g_mxModelView, g_mxProjection);
		}
		for (int i = 5; i < 9; i++) {
			g_Boss[i] = new Enemy(2);
			switch (i)
			{
			case 5:
				g_fBoss[i][0] = 4.5f; g_fBoss[i][1] = 4.5f; g_fBoss[i][2] = 0.0f;
				break;
			case 6:
				g_fBoss[i][0] = 4.5f; g_fBoss[i][1] =  - 4.5f; g_fBoss[i][2] = 0.0f;
				break;
			case 7:
				g_fBoss[i][0] = - 4.5f; g_fBoss[i][1] = -4.5f; g_fBoss[i][2] = 0.0f;
				break;
			case 8:
				g_fBoss[i][0] =-4.5f; g_fBoss[i][1] = 4.5f; g_fBoss[i][2] = 0.0f;
				break;
			}
			vec4 vColor = vec4(i * 0.1, i * 0.05, i * 0.0, 1);
			g_Boss[i]->setColor(vColor);
			mat4 mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			mat4 mxCT = Translate(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]);
			g_Boss[i]->setTRSMatrix(mxCT*mxT);
			g_Boss[i]->setShader(g_mxModelView, g_mxProjection);
		}
		printf("Boss Gen");
	}
	if (_bBossGen) {
		BossMove(delta,1);
		for (int j = 0; j < _iOut; j++) {
			bool attackAdd = false;
			bool attack = g_Boss[0]->CheckCollider(g_Missile[j]->getPos().x, g_Missile[j]->getPos().y, 2.0f);
			if (attack && !_bMissileSus[j] && _bBossOpen) {
				_iBeAttackCount++;
				_bMissileSus[j] = true;
				printf("Score:%d\n", _iBeAttackCount);
			}
		}
		/*first change*/
		if (_iBeAttackCount == 10 && !_bFirstDied) {
			_bFirstDied = true;
			g_fFirstDiedPos[0] = g_Boss[0]->getPos().x;
			g_fFirstDiedPos[1] = g_Boss[0]->getPos().y;
		}
		/*second change*/
		if (_iBeAttackCount == 20 && !_bSecondDied) {
			_bSecondDied = true;
			g_fSecondDiedPos[0] = g_Boss[0]->getPos().x;
			g_fSecondDiedPos[1] = g_Boss[0]->getPos().y;
		}
		if (_iBossType == 1 && !_bFirstDied) {
			for (int i = 5; i < 9; i++) {
				Attack(delta, i, g_Boss[i], 2);
			}
		}
		if ((_iBossType == 1 || _iBossType == 2) && !_bSecondDied) {
			for (int i = 1; i < 5; i++) {
				Attack(delta, i, g_Boss[i], 1);
			}
		}
	}
	if (_fShootDur >= 3.5f) {
		_fShootDur = 0.5f;
	}
	
	GL_Display();
}

//----------------------------------------------------------------------------
//create flighter and protect 1-1 1-2
void CreateQuadRelationship()
{
	GLuint uiShaderHandle;
	vec4 vColor = vec4(1, 0, 0, 1);
	vec4 vT;
	mat4 mxT;
	mat4 mxS;
	int idx = 0;
	// Flighter Create
	for (int i = 0; i < 4; i++) {
		if (i %3 ==0) {
			if (i < 2) {
				g_Player[i] = new  CFlighter(1);
				g_Player[i]->setColor(vColor, 1);
				vColor = vec4(1, 1, 0, 1);
				g_Player[i]->setColor(vColor, 2);
				g_fPlayer[i][0] = g_fPlayer[i][1] = g_fPlayer[i][2] = 0;
				g_initmxS[i] = Scale(1.0, 1.0, 0);
				g_initmxT[i] = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
			}
			else {
				g_Player[i] = new  CFlighter(2);
				vColor = vec4(1, 0, 0, 1);
				g_Player[i]->setColor(vColor, 2);
				vColor = vec4(1, 1, 0, 1);
				g_Player[i]->setColor(vColor, 1);
				g_fPlayer[i][0] = 1.5;
				g_fPlayer[i][1] = 0;
				g_fPlayer[i][2] = 0;
				g_initmxS[i] = Scale(1.0, 1.0, 0);
				g_initmxT[i] = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
			}
			g_Player[i]->setTRSMatrix(g_initmxT[i]);
		}
		else {
			if (i < 2) {
				g_Player[i] = new  CFlighter(1);
				vColor = vec4(1, 0, 221, 1);
				g_Player[i]->setColor(vColor, 1);
				vColor = vec4(0, 0, 1, 1);
				g_Player[i]->setColor(vColor, 2);
			}
			else {
				g_Player[i] = new  CFlighter(2);
				vColor = vec4(0, 0, 1, 1);
				g_Player[i]->setColor(vColor, 1);
				vColor = vec4(1, 0, 221, 1);
				g_Player[i]->setColor(vColor, 2);
			}
			g_fPlayer[i][0] = 0.3*int(i+1);
			g_fPlayer[i][1] = 2.2;
			g_fPlayer[i][2] = 0;
			g_initmxT[i] = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
			g_initmxS[i] = Scale(1.0, 2.0, 0);
			g_Player[i]->setTRSMatrix(g_initmxT[i] * g_initmxS[i]);
		}
		g_Player[i]->setShader(g_mxModelView, g_mxProjection,1);
	}
	//for flighter upgrade one  3-2
	mat4 mxUT;
	for (int i = 0; i < 2; i++) {
		g_UpgradeOne[i] = new  CFlighter(6);
		vColor = vec4(0, 0, 1, 1);
		g_UpgradeOne[i]->setColor(vColor, 6);
		g_fUpgradeOne[i][0] = 0+i*0.6; g_fUpgradeOne[i][1] = 0; g_fUpgradeOne[i][2] = 0;
		mxUT = Translate(g_fUpgradeOne[i][0], g_fUpgradeOne[i][1], g_fUpgradeOne[i][2]);
		g_UpgradeOne[i]->setTRSMatrix(mxUT);
		g_UpgradeOne[i]->setShader(g_mxModelView, g_mxProjection, 6);
	}
	//for flighter upgrade two 3-2
	g_UpgradeTwo = new CFlighter(8);
	vColor = vec4(0, 0, 1, 1);
	g_UpgradeTwo->setColor(vColor, 8);
	g_fUpgradeTwo[0] = -0.8; g_fUpgradeTwo[1] = 2.5; g_fUpgradeTwo[2] = 0;
	mxUT = Translate(g_fUpgradeTwo[0], g_fUpgradeTwo[1], g_fUpgradeTwo[2]);
	g_UpgradeTwo->setTRSMatrix(mxUT);
	g_UpgradeTwo->setPos(vec3(g_fUpgradeTwo[0], g_fUpgradeTwo[1], g_fUpgradeTwo[2]));
	g_UpgradeTwo->setShader(g_mxModelView, g_mxProjection, 8);
	//for flighter upgrade three 3-2
	g_UpgradeThree = new CFlighter(4);
	vColor = vec4(0.8, 0.5, 0, 1);
	g_UpgradeThree->setColor(vColor, 4);
	g_fUpgradeThree[0] = -0.8f; g_fUpgradeThree[1] = -1.0f; g_fUpgradeThree[2] = 0;
	mxUT = Translate(g_fUpgradeThree[0], g_fUpgradeThree[1], g_fUpgradeThree[2]);
	g_UpgradeThree->setTRSMatrix(mxUT);
	g_UpgradeThree->setPos(vec3(g_fUpgradeThree[0], g_fUpgradeThree[1], g_fUpgradeThree[2]));
	g_UpgradeThree->setShader(g_mxModelView, g_mxProjection, 4);
	//for fivestar create
	g_FiveStar = new CFlighter(4);
	vColor = vec4(0.11,0.25, 0.51, 1);
	g_FiveStar->setColor(vColor,4);
	g_fFiveStar[0] = -3.0f; g_fFiveStar[1] = -1.25f; g_fFiveStar[2] = 0;
	g_finitmxT = Translate(g_fFiveStar[0], g_fFiveStar[1], g_fFiveStar[2]);
	g_FiveStar->setShader(g_mxModelView, g_mxProjection, 4);
	g_FiveStar->setTRSMatrix(g_finitmxT);

	//for background 1-4
	float min = -13.0f;
	float max = 13.0f;

	int _imin = 3;
	int _imax = 10;

	/* 產生 [min , max] 的整數亂數 */
	srand(time(NULL));
	for (int i = 0; i < 4; i++) {
		float x = (max - min) * rand() / (RAND_MAX + 1.0) + min;
		int _ix = rand() % (_imax - _imin + 1) + _imin;
		int type = 0;
		if (int(x) % 2 == 0) type = 1;
		else type = 2;
		g_Cloud[i] = new CBackGround(type);
		g_CloudType[i] = type;
		vColor = vec4(191, 218, 218, 0.0);
		g_Cloud[i]->setColor(vColor, 2);
		g_fCloud[i][0] = x; g_fCloud[i][1] = 13.0f; g_fCloud[i][2] = 0.0f;
		mat4 mxCT = Translate(g_fCloud[i][0], g_fCloud[i][1], g_fCloud[i][2]);
		g_fSpeedCloud[i] = _ix;
		g_Cloud[i]->setShader(g_mxModelView, g_mxProjection);
		g_Cloud[i]->setTRSMatrix(mxCT);
	}
	
}
//1-2
void ProtectRotation(float delta) {
	mat4 mxR;
	g_fFAngle += delta * 90.0f;
	if (g_fFAngle > 360.0) g_fFAngle -= 360;
	else if (g_fFAngle < 0.0) g_fFAngle += 360.0;
	mxR = RotateZ(g_fFAngle);
	float x = g_Player[0]->getPos().x-(mxGT * g_Flightercenter * g_initmxS[0] * mxR * g_finitmxT)._m->x ;
	float y = (mxGT * g_Flightercenter * g_initmxS[0] * mxR * g_finitmxT)._m->y + g_Player[0]->getPos().y;
	g_FiveStar->setPos(vec3(x,y,0));
	
	g_FiveStar->setTRSMatrix( mxGT  * g_Flightercenter * g_initmxS[0] * mxR * g_finitmxT);
}
void TraceAttack(float delta) {
	mat4 mxT;
	_fTraceCount += delta;
	_fTraceDir[0] = g_Player[0]->getPos().x - g_FiveStar->getPos().x;
	_fTraceDir[1] = g_Player[0]->getPos().y - g_FiveStar->getPos().y;
	mxT = Translate(g_FiveStar->getPos().x + _fTraceDir[0] * _fTraceCount, g_FiveStar->getPos().y + _fTraceDir[1] * _fTraceCount, 0);
	g_FiveStar->setTRSMatrix(mxT);
}
//---------------------------------------------------------------------------------------------------------------------
//1-3 1-4
void Shoot(float delta) {
	mat4 mxT;
	vec4 vColor = vec4(1, 0, 0, 1);
	_fShootTime += delta;
	if (_fShootTime > _fShootDur) {
		g_Missile[_iOut] = new CFlighter(5);
		vColor = vec4(1, 0, 0, 1);
		g_Missile[_iOut]->setColor(vColor, 5);
		//g_fMissile[_iOut][0] = -3.0f; g_fMissile[_iOut][1] = -1.25f; g_fMissile[_iOut][2] = 0;
		//g_minitmxT = Translate(g_fMissile[_iOut][0], g_fMissile[_iOut][1], g_fMissile[_iOut][2]);
		g_Missile[_iOut]->setShader(g_mxModelView, g_mxProjection, 5);
		g_Missile[_iOut]->setTRSMatrix(g_Shootercenter);
		g_shootMousePos[_iOut][0] = g_fTx; g_shootMousePos[_iOut][1] = g_fTy; g_shootMousePos[_iOut][3] = 0;
		_iOut++;
		_iFree--;
		_iNext = 20 - _iFree;
		_fShootTime = 0;
		
	}
	for (int i = 0; i < 20-_iFree; i++) {
		g_fShootDir[i] += delta;
		mxT = Translate(g_shootMousePos[i][0], g_shootMousePos[i][1] + g_fShootDir[i]* _fShootSpeed, 0);
		g_Missile[i]->setTRSMatrix(g_Shootercenter * mxT);
		g_Missile[i]->setPos(vec3(g_shootMousePos[i][0], g_shootMousePos[i][1] + g_fShootDir[i] * _fShootSpeed, 0));
		//out of windowns reset
		
	}
	for (int i = 0; i < _iOut; i++) {
		if (g_Missile[i]->getPos().y > 13.8f) {
			_iFree++;
			
			for (int j = 0; j < _iOut-1; j++) {
				g_fShootDir[j] = g_fShootDir[j + 1];
				g_shootMousePos[j][0] = g_shootMousePos[j + 1][0];
				g_shootMousePos[j][1] = g_shootMousePos[j + 1][1];
				g_Missile[j] = g_Missile[j + 1];
				g_Missile[j]->setPos(vec3(g_fTx, g_fTy + g_fShootDir[j] * _fShootSpeed, 0));
				_bMissileSus[j] = _bMissileSus[j + 1];
			}
			g_fShootDir[_iOut-1] = 0;
			_bMissileSus[_iOut - 1] = false;
			_iOut--;
		}
	}
}

// for 1-5
void CloudMove(float delta) {
	mat4 mxT;
	for (int i = 0; i < 4; i++) {
		g_fCloud[i][1] -= delta * g_fSpeedCloud[i];
		if (g_fCloud[i][1] < -13.0f) {
			g_fCloud[i][0] = getRandomf(13.0f,-13.0f); g_fCloud[i][1] = 13.0f;
			g_fSpeedCloud[i] = getRandomi(10,1);
		}
		mxT = Translate(g_fCloud[i][0], g_fCloud[i][1], g_fCloud[i][2]);
		g_Cloud[i]->setTRSMatrix(mxT);
	}
}

//for 2-4


//for 3-2 eat change
void EatChange_Generate(int type) {
	g_bGenerate[type] = true;
	vec4 vColor = vec4(1, 0, 0, 1);
	mat4 mxT;
	g_ChangeEat[type] = new CFlighter(3);
	vColor = vec4(type*0.2, type * 0.2, 0, 1);
	g_ChangeEat[type]->setColor(vColor, 3);
	g_fChangeEat[type][0] = 0.0f; g_fChangeEat[type][1] = 13.0f; g_fChangeEat[type][2] = 0;
	mxT = Translate(g_fChangeEat[type][0], g_fChangeEat[type][1], g_fChangeEat[type][2]);
	g_ChangeEat[type]->setShader(g_mxModelView, g_mxProjection, 3);
	g_ChangeEat[type]->setTRSMatrix(mxT);
}
float count;
void EatChangeMove(int type, float delta) {
	mat4 newmxT;
	count += delta;
	g_fEatDir[type][0] += sin(count)/100;
	g_fEatDir[type][1] += delta * 2;
	newmxT = Translate(g_fChangeEat[type][0] + g_fEatDir[type][0], g_fChangeEat[type][1] - g_fEatDir[type][1], g_fChangeEat[type][2]);
	g_ChangeEat[type]->setPos(vec3(g_fChangeEat[type][0] + g_fEatDir[type][0], g_fChangeEat[type][1] - g_fEatDir[type][1], g_fChangeEat[type][2]));
	g_ChangeEat[type]->setTRSMatrix(newmxT);
	//printf("(%f,%f)\n", g_ChangeEat[type]->getPos().x, g_ChangeEat[type]->getPos().y);
	if (g_ChangeEat[type]->getPos().y < -13.0f) {
		G_bGenDel[type] = true;
		delete g_ChangeEat[type];
	}
}
//for 2-1-> 2-3
void EnemyGen(int index,int type) {
	vec4 vColor = vec4(1, 0, 0, 1);
	mat4 mxT,mxR;
	float _cr, _cg, _cb;
	switch (type)
	{
	case 1:
		_bEnemyGen[index] = true;
		g_Enemy[index] = new Enemy(1);
		g_EnemyType[index] = 1;
		_cr = getRandomf(1.0f, 0.0f);
		_cg = getRandomf(1.0f, 0.0f);
		_cb = getRandomf(1.0f, 0.0f);
		vColor = vec4(_cr, _cg, _cb, 1);
		g_Enemy[index]->setColor(vColor);
		g_Enemy[index]->_fAttackDur = getRandomf(10.0f, 3.0f);
		g_fEnemy[index][0] = -12.0f; g_fEnemy[index][1] = 11; g_fEnemy[index][2] = 0;
		mxT = Translate(g_fEnemy[index][0], g_fEnemy[index][1], g_fEnemy[index][2]);
		g_Enemy[index]->setPos(vec3(g_fEnemy[index][0], g_fEnemy[index][1], g_fEnemy[index][2] = 0));
		g_Enemy[index]->setTRSMatrix(mxT);
		g_Enemy[index]->setShader(g_mxModelView, g_mxProjection);
		break;
	case 2:
		_bEnemyGen_S[index] = true;
		g_Enemy_S[index] = new Enemy(2);
		g_EnemyType_S[index] = 1;
		_iInverse_S[index] =  1 ;
		_cr = getRandomf(1.0f, 0.0f);
		_cg = getRandomf(1.0f, 0.0f);
		_cb = getRandomf(1.0f, 0.0f);
		vColor = vec4(_cr, _cg, _cb, 1);
		g_Enemy_S[index]->setColor(vColor);
		g_Enemy_S[index]->_fAttackDur = getRandomf(10.0f, 3.0f);
		g_fEnemy_S[index][0] = getRandomf(10.0f, -10.0f); g_fEnemy_S[index][1] = 8; g_fEnemy_S[index][2] = 0;
		mxT = Translate(g_fEnemy_S[index][0], g_fEnemy_S[index][1], g_fEnemy_S[index][2]);
		g_Enemy_S[index]->setPos(vec3(g_fEnemy_S[index][0], g_fEnemy_S[index][1], g_fEnemy_S[index][2] = 0));
		g_Enemy_S[index]->setTRSMatrix(mxT);
		g_Enemy_S[index]->setShader(g_mxModelView, g_mxProjection);
		break;
	case 3:
		_bEnemyGen_T[index] = true;
		g_Enemy_T[index] = new Enemy(3);
		g_EnemyType_T[index] = 1;
		_iInverse_T[index] = 1;
		_cr = getRandomf(1.0f, 0.0f);
		_cg = getRandomf(1.0f, 0.0f);
		_cb = getRandomf(1.0f, 0.0f);
		vColor = (_cr, _cg, _cb, 1);
		g_Enemy_T[index]->setColor(vColor);
		g_Enemy_T[index]->_fAttackDur = getRandomf(12.0f, 5.0f);
		g_fEnemy_T[index][0] = getRandomf(9.0f,-9.0f); g_fEnemy_T[index][1] = getRandomf(9.0f, 4.0f); g_fEnemy_T[index][2] = 0;
		mxT = Translate(g_fEnemy_T[index][0], g_fEnemy_T[index][1], g_fEnemy_T[index][2]);
		g_Enemy_T[index]->setPos(vec3(g_fEnemy_T[index][0], g_fEnemy_T[index][1], g_fEnemy_T[index][2] = 0));
		g_Enemy_T[index]->setTRSMatrix(mxT);
		g_Enemy_T[index]->setShader(g_mxModelView, g_mxProjection);
		break;
	}
	
}

void EnemyMove(int type, int index,float delta) {
	mat4 mxT;

	switch (type)
	{
	case 1:
		if (g_Enemy[index]->getPos().x >= 12.0f)  _iInverse[index] = -1;
		if (g_Enemy[index]->getPos().x <= -12.0f)  _iInverse[index] = 1;
		g_fEnemyCount[index] = delta * 2 * _iInverse[index] + g_fEnemyCount[index];
		g_fEnemyDir[index][0] = g_fEnemyCount[index];
		g_fEnemyDir[index][1] = sin(g_fEnemyCount[index]*0.5*PI);
		mxT = Translate(g_fEnemy[index][0] + g_fEnemyDir[index][0], g_fEnemy[index][1] + g_fEnemyDir[index][1], g_fEnemy[index][2]);
		g_Enemy[index]->setPos(vec3(g_fEnemy[index][0] + g_fEnemyDir[index][0], g_fEnemy[index][1] + g_fEnemyDir[index][1], g_fEnemy[index][2]));
		g_Enemy[index]->setTRSMatrix(mxT);
		//printf("(%f,%f,%d,%d)\n", g_Enemy[index]->getPos().x, g_fEnemyCount[index], _iInverse[index],index);
		break;
	case 2:
		if (g_Enemy_S[index]->getPos().y >= 12.0f)  _iInverse_S[index] = -1;
		if (g_Enemy_S[index]->getPos().y <= -12.0f)  _iInverse_S[index] = 1;
		g_fEnemyCount_S[index] = delta * 2 * _iInverse_S[index] + g_fEnemyCount_S[index];
		g_fEnemyDir_S[index][0] = sin(g_fEnemyCount_S[index] * 0.5 * PI *2);
		g_fEnemyDir_S[index][1] = g_fEnemyCount_S[index]*5;
		mat4 mxR;
		g_fEnemy_S_Angle[index] += delta * 90.0f;
		if (g_fEnemy_S_Angle[index] > 360.0) g_fEnemy_S_Angle[index] -= 360;
		else if (g_fEnemy_S_Angle[index] < 0.0) g_fEnemy_S_Angle[index] += 360.0;
		mxR = RotateZ(g_fEnemy_S_Angle[index]);
		mxT = Translate(g_fEnemy_S[index][0] + g_fEnemyDir_S[index][0], g_fEnemy_S[index][1] + g_fEnemyDir_S[index][1], g_fEnemy_S[index][2]);
		g_Enemy_S[index]->setPos(vec3(g_fEnemy_S[index][0] + g_fEnemyDir_S[index][0], g_fEnemy_S[index][1] + g_fEnemyDir_S[index][1], g_fEnemy_S[index][2]));
		g_Enemy_S[index]->setTRSMatrix(mxT* mxR);
		//printf("(%f,%f,%d,%d)\n", g_Enemy[index]->getPos().x, g_fEnemyCount[index], _iInverse[index],index);
		break;

	}
}
//for 2-4
void Attack(float delta,int i,Enemy* Enemy,int type) {
	mat4 mxT;
	mat4 mxR;
	vec4 vColor = vec4(1, 0, 0, 1);
	float px, py, pz = 0.0f;
	Enemy->_fAttackTime += delta;
	if (Enemy->_fAttackTime > Enemy->_fAttackDur && Enemy->_bEnemyDel == false) {
		Enemy->g_Attack[Enemy->_iOut] = new CFlighter(7);
		if(type == 1) vColor = vec4(1, 0, 0, 1);
		else if(type ==2) vColor = vec4(0, 0, 1, 1);
		else if (type == 3) vColor = vec4(0, 1, 0, 1);
		Enemy->g_Attack[Enemy->_iOut]->setColor(vColor, 7);
		Enemy->g_Attack[Enemy->_iOut]->setShader(g_mxModelView, g_mxProjection, 7);
		Enemy->g_AttackInitPos[Enemy->_iOut][0] = Enemy->getPos().x;
		Enemy->g_AttackInitPos[Enemy->_iOut][1] = Enemy->getPos().y;
		mxT = Translate(Enemy->g_AttackInitPos[Enemy->_iOut][0], Enemy->g_AttackInitPos[Enemy->_iOut][1], pz);
		Enemy->g_Attack[Enemy->_iOut]->setTRSMatrix(mxT);
		Enemy->_iOut +=1;
		Enemy->_iFree -=1;
		Enemy->_fAttackTime = 0;
	}
	for (int k = 0; k < Enemy->_iOut; k++) {
		//printf("%d,%d:(%f)\n", i, k, g_Enemy[i]->g_Attack[k]->getPos().y);
		if (Enemy->g_Attack[k] != nullptr) {
			switch (type)
			{
			case 1:
				if (Enemy->g_Attack[k]->getPos().y < -13.8f ) {
					Enemy->_iFree++;
					for (int l = 0; l < Enemy->_iOut - 1; l++) {
						Enemy->g_fAttackDir[l] = Enemy->g_fAttackDir[l + 1];
						Enemy->g_AttackInitPos[l][0] = Enemy->g_AttackInitPos[l + 1][0];
						Enemy->g_AttackInitPos[l][1] = Enemy->g_AttackInitPos[l + 1][1];
						Enemy->g_Attack[l] = Enemy->g_Attack[l + 1];
						Enemy->_bAttackSus[l] = Enemy->_bAttackSus[l + 1];
						Enemy->g_Attack[l]->setPos(vec3(Enemy->g_AttackInitPos[l][0] + g_fTraceDir[l][0] * -Enemy->g_fAttackDir[l], Enemy->g_AttackInitPos[l][1] + g_fTraceDir[l][1] * -Enemy->g_fAttackDir[l], 0));
					}
					Enemy->g_fAttackDir[Enemy->_iOut - 1] = 0;
					Enemy->_bAttackSus[Enemy->_iOut - 1] = false;
					Enemy->_iOut--;
				}
				break;
			case 2:
				if (Enemy->g_Attack[k]->getPos().x < -13.8f || Enemy->g_Attack[k]->getPos().x > 13.8f) {
					Enemy->_iFree++;
					for (int l = 0; l < Enemy->_iOut - 1; l++) {
						Enemy->g_fAttackDir[l] = Enemy->g_fAttackDir[l + 1];
						Enemy->g_AttackInitPos[l][0] = Enemy->g_AttackInitPos[l + 1][0];
						Enemy->g_AttackInitPos[l][1] = Enemy->g_AttackInitPos[l + 1][1];
						Enemy->g_Attack[l] = Enemy->g_Attack[l + 1];
						Enemy->_bAttackSus[l] = Enemy->_bAttackSus[l + 1];
						Enemy->g_Attack[l]->setPos(vec3(Enemy->g_AttackInitPos[l][0], Enemy->g_AttackInitPos[l][1] + Enemy->g_fAttackDir[l] * Enemy->_fAttackSpeed, 0));
					}
					Enemy->g_fAttackDir[Enemy->_iOut - 1] = 0;
					Enemy->_bAttackSus[Enemy->_iOut - 1] = false;
					Enemy->_iOut--;
				}
				break;
			case 3:
				if (Enemy->g_Attack[k]->getPos().x < -13.8f || Enemy->g_Attack[k]->getPos().x > 13.8f || Enemy->g_Attack[k]->getPos().y < -13.8f || Enemy->g_Attack[k]->getPos().y > 13.8f) {
					Enemy->_iFree++;
					for (int l = k; l < Enemy->_iOut - 1; l++) {
						Enemy->g_fAttackDir[l] = Enemy->g_fAttackDir[l + 1];
						Enemy->g_AttackInitPos[l][0] = Enemy->g_AttackInitPos[l + 1][0];
						Enemy->g_AttackInitPos[l][1] = Enemy->g_AttackInitPos[l + 1][1];
						Enemy->g_Attack[l] = Enemy->g_Attack[l + 1];
						Enemy->_bAttackSus[l] = Enemy->_bAttackSus[l + 1];
						g_fTraceDir[l][0] = g_fTraceDir[l + 1][0];
						g_fTraceDir[l][1] = g_fTraceDir[l + 1][1];
						_bSetTrace[i][l] = _bSetTrace[i][l + 1];
						Enemy->g_Attack[l]->setPos(vec3(Enemy->g_AttackInitPos[l][0], Enemy->g_AttackInitPos[l][1] + Enemy->g_fAttackDir[l] * Enemy->_fAttackSpeed, 0));
					}
					Enemy->g_fAttackDir[Enemy->_iOut - 1] = 0;
					Enemy->_bAttackSus[Enemy->_iOut - 1] = false;
					_bSetTrace[i][Enemy->_iOut - 1] = false;
					Enemy->_iOut--;
				}
				break;
			}


		}
	}
	for (int j = 0; j <ATTACK_NUM - Enemy->_iFree; j++) {
		bool _bAttack = false;
		bool _bDefend = false;
		if (Enemy->g_Attack[j] != nullptr) {
			Enemy->g_fAttackDir[j] -= delta;
			switch (type)
			{
			case 1:
				mxT = Translate(Enemy->g_AttackInitPos[j][0], Enemy->g_AttackInitPos[j][1] + Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed, 0);
				Enemy->g_Attack[j]->setPos(vec3(Enemy->g_AttackInitPos[j][0], Enemy->g_AttackInitPos[j][1] + Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed, 0));
				Enemy->g_Attack[j]->setTRSMatrix(mxT);
				//out of windowns reset
				_bAttack = g_Player[0]->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1.5);
				//protecy player
				// 
				_bDefend = g_FiveStar->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1.0f);
				//printf("(%f,%f),(%f,%f)\n", g_FiveStar->getPos().x, g_FiveStar->getPos().y, g_Player[0]->getPos().x, g_Player[0]->getPos().y);
				if (_bDefend) {
					Enemy->_bAttackSus[j] = true;
					//printf("1Defend\n");
				}
				if (_bAttack && !_bAttacked && !Enemy->_bAttackSus[j]) {
					//printf("Attack\n");
					Enemy->_bAttackSus[j] = true;
					_bAttacked = true;
					_fShootDur += 1.0f;
				}
				break;
			case 2:
				if (Enemy->g_AttackInitPos[j][0] < 0.0f) {
					mxT = Translate(Enemy->g_AttackInitPos[j][0] - (Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed), Enemy->g_AttackInitPos[j][1], 0);
					Enemy->g_Attack[j]->setPos(vec3(Enemy->g_AttackInitPos[j][0] - (Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed), Enemy->g_AttackInitPos[j][1], 0));
				}
				else {
					mxT = Translate(Enemy->g_AttackInitPos[j][0] + (Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed), Enemy->g_AttackInitPos[j][1], 0);
					Enemy->g_Attack[j]->setPos(vec3(Enemy->g_AttackInitPos[j][0] + (Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed), Enemy->g_AttackInitPos[j][1], 0));
				}
				Enemy->g_Attack[j]->setTRSMatrix(mxT);
				//out of windowns reset
				_bAttack = g_Player[0]->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1.5);
				//protecy player
				// 
				_bDefend = g_FiveStar->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y , 1.0f);
				//printf("(%f,%f),(%f,%f)\n", g_FiveStar->getPos().x, g_FiveStar->getPos().y, g_Player[0]->getPos().x, g_Player[0]->getPos().y);
				if (_bDefend) {
					Enemy->_bAttackSus[j] = true;
					//printf("2Defend\n");
				}
				if (_bAttack && !_bAttacked && !Enemy->_bAttackSus[j]) {
					//printf("Attack\n");
					Enemy->_bAttackSus[j] = true;
					_bAttacked = true;
					_fShootDur += 1.0f;
				}
				break;
			case 3:
				mat4 mxTrace;
				if (!_bSetTrace[i][j]) {
					_bSetTrace[i][j] = true;
					g_fTraceDir[j][0] = g_Player[0]->getPos().x - Enemy->g_AttackInitPos[j][0];
					g_fTraceDir[j][1] = g_Player[0]->getPos().y - Enemy->g_AttackInitPos[j][1];
				}
				mxT = Translate(Enemy->g_AttackInitPos[j][0]+ g_fTraceDir[j][0] * -Enemy->g_fAttackDir[j], Enemy->g_AttackInitPos[j][1] + g_fTraceDir[j][1] * -Enemy->g_fAttackDir[j], 0);
				mxTrace = Translate(Enemy->getPos().x - Enemy->g_AttackInitPos[j][0], Enemy->getPos().y - Enemy->g_AttackInitPos[j][1], 0);
				Enemy->g_Attack[j]->setPos(vec3(Enemy->g_AttackInitPos[j][0] + g_fTraceDir[j][0] * -Enemy->g_fAttackDir[j], Enemy->g_AttackInitPos[j][1] + g_fTraceDir[j][1] * -Enemy->g_fAttackDir[j], 0));
				Enemy->g_Attack[j]->setTRSMatrix(mxT); 
				//out of windowns reset
				_bAttack = g_Player[0]->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1.5);
				//protecy player
				// 
				_bDefend = g_FiveStar->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y , 1.0f);
				//printf("(%f,%f),(%f,%f)\n", g_FiveStar->getPos().x, g_FiveStar->getPos().y, g_Player[0]->getPos().x, g_Player[0]->getPos().y);
				if (_bDefend) {
					Enemy->_bAttackSus[j] = true;
					//printf("3Defend\n");
				}
				if (_bAttack && !_bAttacked && !Enemy->_bAttackSus[j]) {
					//printf("Attack\n");
					Enemy->_bAttackSus[j] = true;
					_bAttacked = true;
					_fShootDur += 1.0f;
				}
				break;
			}
			
			
		}
	}
	if (Enemy->_iOut == 0 && Enemy->_bEnemyDel) {
		Enemy->_bAttackOut = true;
	}
}
float getRandomf(float _max, float _min) {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> distr(_min, _max);
	float fx = distr(eng);
	return fx;
}
int getRandomi(int _max, int _min) {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<> distr(_min, _max);
	int fx = distr(eng);
	return fx;
}

void BeAttack(float delta) {
	_fChangeClock += delta;
	if (_fChangeClock > 0.1) {
		_fChangeClock=0;
		_bAttacked = false;
	}
}
float RotateCount;
bool inverse[4] = { true };
void BossMove(float delta,int type) {
	RotateCount += delta;
	g_fScaleClock += delta;
	if (RotateCount > 180.0f) RotateCount == -180.0f;
	if (RotateCount < -270.0f) RotateCount == -RotateCount;
	float s;
	mat4 mxR,mxT,mxCT,mxS;
	mxR = RotateZ(RotateCount*90);
	s = (1 - sin((g_fScaleClock * 0.25 * 180.0f) * 3.1415926 / 180.0f)) * 0.8f ;//縮放
	if (s < 0.01f) {
		g_fBoss[0][0] = getRandomf(5.0f, -5.0f);
		g_fBoss[0][1] = getRandomf(10.0f, 5.0f);
	}
	if (s > 1.0f) {
		s = 1.0f;
		_bBossOpen = true;
	}
	else {
		_bBossOpen = false;
	}
	mxS = Scale(s, s, 1);
	mxCT = Translate(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]);
	g_Boss[0]->setTRSMatrix(mxCT*mxS);
	g_Boss[0]->setPos(vec3(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]));
	if ((type == 1 || type == 2) && !_bSecondDied) {
		for (int i = 1; i < 5; i++) {
			mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			mxCT = Translate(g_fBoss[0][0], g_fBoss[0][1], g_fBoss[0][2]);
			g_Boss[i]->setTRSMatrix(mxCT * mxR * mxS * mxT * mxR);
			float fx, fy;
			fx = g_fBoss[i][0] + g_fBoss[0][0] + mxR._m->x+ mxS._m->x + mxR._m->x;
			fy = g_fBoss[i][1] + g_fBoss[0][1]+ mxR._m->y + mxS._m->y + mxR._m->y;
			//printf(" % d:(% f, % f)", i, fx, fy);
			g_Boss[i]->setPos(vec3(fx, fy, 0));
		}
	}
	else {
		for (int i = 1; i < 5; i++) {
			g_fBoss[i][1] -= delta * 4;
			mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			mxCT = Translate(g_fSecondDiedPos[0], g_fSecondDiedPos[1],0);
			g_Boss[i]->setTRSMatrix(mxCT  * mxT * mxR);
			float fx, fy;
			fx = g_fBoss[i][0] + g_fSecondDiedPos[0] + mxR._m->x;
			fy = g_fBoss[i][1] + g_fSecondDiedPos[1] + mxR._m->y;
			//printf(" % d:(% f, % f)", i, fx, fy);
			g_Boss[i]->setPos(vec3(fx, fy, 0));
		}
	}
	if (type == 1 && !_bFirstDied) {
		for (int i = 5; i < 9; i++) {
			switch (i) {
			case 5:
				if (g_fBoss[i][0] >= 4.0f) {
					inverse[0] = true;
				}
				else if (g_fBoss[i][0] < 1.5f) {
					inverse[0] = false;
				}
				if (inverse[0]) {
					g_fBoss[i][0] -= delta * 2;
					g_fBoss[i][1] -= delta * 2;
				}
				else {
					g_fBoss[i][0] += delta * 2;
					g_fBoss[i][1] += delta * 2;
				}
				break;
			case 6:
				if (g_fBoss[i][0] >= 4.0f) {
					inverse[1] = true;
				}
				else if (g_fBoss[i][0] < 1.5f) {
					inverse[1] = false;
				}
				if (inverse[1]) {
					g_fBoss[i][0] -= delta * 2;
					g_fBoss[i][1] += delta * 2;
				}
				else {
					g_fBoss[i][0] += delta * 2;
					g_fBoss[i][1] -= delta * 2;
				}
				break;
			case 7:
				if (g_fBoss[i][0] > -1.5f) {
					inverse[2] = true;
				}
				else if (g_fBoss[i][0] <= -4.0f) {
					inverse[2] = false;
				}
				if (inverse[2]) {
					g_fBoss[i][0] -= delta * 2;
					g_fBoss[i][1] -= delta * 2;
				}
				else {
					g_fBoss[i][0] += delta * 2;
					g_fBoss[i][1] += delta * 2;
				}
				break;
			case 8:
				if (g_fBoss[i][0] > -1.5f) {
					inverse[3] = true;
				}
				else if (g_fBoss[i][0] <= -4.0f) {
					inverse[3] = false;
				}
				if (inverse[3]) {
					g_fBoss[i][0] -= delta * 2;
					g_fBoss[i][1] += delta * 2;
				}
				else {
					g_fBoss[i][0] += delta * 2;
					g_fBoss[i][1] -= delta * 2;
				}
				break;
			}

			mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			g_Boss[i]->setTRSMatrix(mxCT * mxT * mxS);
			float fx, fy;
			fx = g_fBoss[i][0] + g_fBoss[0][0];
			fy = g_fBoss[i][1] + g_fBoss[0][1];
			g_Boss[i]->setPos(vec3(fx, fy, 0));
		}

	}
	else {
		for (int i = 5; i < 9; i++) {
			switch (i) {
			case 5:
				g_fBoss[i][1] -= delta * 2;
			case 6:
				g_fBoss[i][1] -= delta * 2;
				break;
			case 7:
				g_fBoss[i][1] -= delta * 2;
				break;
			case 8:
				g_fBoss[i][1] -= delta * 2;
				break;
			}
			mxCT = Translate(g_fFirstDiedPos[0], g_fFirstDiedPos[1], 0);
			mxT = Translate(g_fBoss[i][0], g_fBoss[i][1], g_fBoss[i][2]);
			g_Boss[i]->setTRSMatrix(mxCT* mxT);
			float fx, fy;
			fx = g_fBoss[i][0] + g_fFirstDiedPos[0];
			fy = g_fBoss[i][1] + g_fFirstDiedPos[1];
			g_Boss[i]->setPos(vec3(fx, fy, 0));
		}

	}
	if (g_fBoss[8][1] < -14.0f) {
		_iBossType == 2;
	}
	if (g_fBoss[1][1] < -14.5f) {
		_iBossType == 3;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
void UpdateMatrix()
{
	
}
//----------------------------------------------------------------------------
void Win_Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case  SPACE_KEY:

		break;
	case 68: // D key
	case 100: // d key

		break;
	case 82: // R key
	case 114: // r key
		break;
	case 033:
		glutIdleFunc(NULL);
		for (int i = 0; i < FLIGHTER_SIZE; i++) {
			delete g_Player[i];
		}
		for (int i = 0; i < _iOut; i++) {
			delete g_Missile[i];
		}
		for (int i = 0; i < 4; i++) {
			delete g_Cloud[i];
		}
		delete g_FiveStar;
		for (int i = 0; i < _iGenCount; i++) {
			if (g_Enemy[i] != NULL && !_bEnemyDel[i])
				delete g_Enemy[i];
		}
		for (int i = 0; i < _iGenCount_S; i++) {
			if (g_Enemy_S[i] != nullptr && !_bEnemyDel_S[i])
				delete g_Enemy_S[i];
		}
		for (int i = 0; i < _iGenCount_T; i++) {
			if (g_Enemy_T[i] != nullptr && !_bEnemyDel_T[i])
				delete g_Enemy_T[i];
		}
		for (int i = 0; i < 8; i++) {
			delete g_Boss[i];
		}
		exit(EXIT_SUCCESS);
		break;
	}
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
		if (state == GLUT_DOWN && _bUpgrade[1] && !_bUseGear) {
			if(_bUpgrade[1] && !_bUseGear && !_bGearCold)
			_bUseGear = true;
		}
		break;
	case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 
		if (state == GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
		if (state == GLUT_DOWN) {
			if (_bUpgrade[2] && !_bTraceCold) {
				_bUseTrace = true;

			}
		}
		
		break;
	default:
		break;
	}
}
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
//1-1
void Win_PassiveMotion(int x, int y) {
	mat4  mxT;

	g_fTx = 12.0f * (x - HALF_SIZE) / (HALF_SIZE);
	g_fTy = -12.0f * (y - HALF_SIZE) / (HALF_SIZE);
	mxGT = Translate(g_fTx, g_fTy, 0);
	for (int i = 0; i < FLIGHTER_SIZE; i++) {
		mxT = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
		g_Player[i]->setTRSMatrix(mxGT * g_initmxT[i]*g_initmxS[i]);
	}
	for (int i = 0; i < 2; i++) {
		mxT = Translate(g_fUpgradeOne[i][0], g_fUpgradeOne[i][1], g_fUpgradeOne[i][2]);
		g_UpgradeOne[i]->setTRSMatrix(mxT * mxGT);
	}
	if (!_bUseGear || _fColdClock == 0.0f) {
		mxT = Translate(g_fUpgradeTwo[0], g_fUpgradeTwo[1], g_fUpgradeTwo[2]);
		g_UpgradeTwo->setTRSMatrix(mxT * mxGT);
		g_UpgradeTwo->setPos(vec3(g_fUpgradeTwo[0] + g_fTx, g_fUpgradeTwo[1] + g_fTy, g_fUpgradeTwo[2]));
	}
	mxT = Translate(g_fUpgradeThree[0], g_fUpgradeThree[1], g_fUpgradeThree[2]);
	g_UpgradeThree->setTRSMatrix(mxT * mxGT);
	g_UpgradeThree->setPos(vec3(g_fUpgradeThree[0] + g_fTx, g_fUpgradeThree[1] + g_fTy, g_fUpgradeThree[2]));
	g_Player[0]->setPos(vec3(-0.8f + g_fTx, -1.4f + g_fTy, 0.0f));

}
// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	mat4 mxGT;

}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	mat4 rx, ry, rz;
	switch (key) {
	case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
		
		break;
	case GLUT_KEY_RIGHT:		// 目前按下的是向右方向鍵
		
		break;
	case GLUT_KEY_UP:		// 目前按下的是向右方向鍵
		
		break;
	case GLUT_KEY_DOWN:		// 目前按下的是向右方向鍵
		
		break;
	default:
		break;
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("FighterGame");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// 處理 ASCII 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc(Win_SpecialKeyboard);	// 處理 NON-ASCII 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;
}
