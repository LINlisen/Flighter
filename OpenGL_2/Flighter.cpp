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
mat4 g_initmxS[4];
mat4 g_initmxT[4];
mat4 g_Flightercenter = Translate(-0.8f, -1.4f, 0.0f);

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
Enemy* g_Enemy_S[10];
int g_EnemyType_S[10];
int _iGenCount_S;
int _iInverse_S[10] = { 1 };
int _iDieCount_S[10];
float g_fEnemy_S[10][3];
float g_fEnemyDir_S[10][3];
float g_fEnemyCount_S[10];
float EnemyTime_S = 0.0f;
bool _bEnemyGen_S[10] = { false };
bool _bEnemyDel_S[10] = { false };
//for 3-2
CFlighter* g_UpgradeOne[2];
float g_fUpgradeOne[2][3];
bool _bUpgrade[3] = { false };
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
void Shoot(float); //1-3 1-4
void CloudMove(float); //1-5
void EatChange_Generate(int);//3-2
void EatChangeMove(int , float);//4-1
void EnemyGen(int,int);
void EnemyMove(int,int,float);
void Attack(float,int,Enemy*);
float getRandomf(float _max, float _min); //get romdon float
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
		g_Player[i]->draw(1);
	}
	for (int i = 0; i < 3; i++) {
		if (_bUpgrade[i]) {
			switch (i)
			{
			case 0:
				g_UpgradeOne[0]->draw(6);
				g_UpgradeOne[1]->draw(6);
				break;
			case 1:
				break;
			case 2:
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
				if(g_Enemy[i]!=nullptr)
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
	}
	g_FiveStar->draw(4);
	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	ProtectRotation(delta);
	Shoot(delta);
	CloudMove(delta);
	//for 3-2 eat change
	CountTime += delta;
	EnemyTime += delta;
	EnemyTime_S += delta;
	if (CountTime > 2.0f && g_bGenerate[0] == false) {
		EatChange_Generate(0);
	}
	for (int i = 0; i < 3; i++) {
		if (g_ChangeEat[i] == nullptr) break;
		if (g_bGenerate[i] && G_bGenDel[i] == false) {
			EatChangeMove(i, delta);
			if (g_ChangeEat[i]->CheckCollider(g_Player[0]->getPos().x, g_Player[0]->getPos().y, 0.4f)) {
				G_bGenDel[i] = true;
				delete g_ChangeEat[i];
				_bUpgrade[i] = true;
				_fShootSpeed = 15;
				_fShootDur = 0.4f;
			}
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
				if (check) {
					_iDieCount[i]++;
				}
				if (_iDieCount[i] == 2) {
					_bEnemyDel[i] = true;
					g_Enemy[i]->_bEnemyDel = true;
					if(g_Enemy[i]->_bAttackOut) delete g_Enemy[i];
					_iDieCount[i] = 0;
				}
			}
			//Attack(delta);
		}
		if (!g_Enemy[i]->_bAttackOut) {
			Attack(delta,i, g_Enemy[i]);
		}
	}
	//Second Enemy Generate
	if (EnemyTime_S > getRandomf(6.0f,1.0f) && !_bEnemyGen_S[_iGenCount_S] && _iGenCount_S < 10) {

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
					if (check) {
						_iDieCount_S[i]++;
					}
					if (_iDieCount_S[i] == 2) {
						_bEnemyDel_S[i] = true;
						g_Enemy_S[i]->_bEnemyDel = true;
						if (g_Enemy_S[i]->_bAttackOut) delete g_Enemy_S[i];
						_iDieCount_S[i] = 0;
					}
				}
				//Attack(delta);
			}
			if (!g_Enemy_S[i]->_bAttackOut) {
				Attack(delta, i, g_Enemy_S[i]);
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
	g_FiveStar->setPos(vec3((g_fFiveStar[0])* mxR._m->x+ g_Player[0]->getPos().x, -(g_fFiveStar[1] * mxR._m->y + g_Player[0]->getPos().y),0));
	g_FiveStar->setTRSMatrix( mxGT  * g_Flightercenter * g_initmxS[0] * mxR * g_finitmxT);
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
			}
			g_fShootDir[_iOut-1] = 0;
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
			g_fSpeedCloud[i] = rand() % (_imax - _imin + 1) + _imin;
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
	int _imin = 1;
	int _imax = 1;
	vec4 vColor = vec4(1, 0, 0, 1);
	mat4 mxT;
	srand(time(NULL));
	int _ix = rand() % (_imax - _imin + 1) + _imin;
	switch (type)
	{
	case 1:
		_bEnemyGen[index] = true;
		g_Enemy[index] = new Enemy(1);
		g_EnemyType[index] = 1;
		g_Enemy[index]->setColor(vColor);
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
		vColor = vec4(0, 0, 1, 1);
		g_Enemy_S[index]->setColor(vColor);
		g_fEnemy_S[index][0] = getRandomf(10.0f, -10.0f); g_fEnemy_S[index][1] = 8; g_fEnemy_S[index][2] = 0;
		mxT = Translate(g_fEnemy_S[index][0], g_fEnemy_S[index][1], g_fEnemy_S[index][2]);
		g_Enemy_S[index]->setPos(vec3(g_fEnemy_S[index][0], g_fEnemy_S[index][1], g_fEnemy_S[index][2] = 0));
		g_Enemy_S[index]->setTRSMatrix(mxT);
		g_Enemy_S[index]->setShader(g_mxModelView, g_mxProjection);
		break;
	case 3:
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
		mxT = Translate(g_fEnemy_S[index][0] + g_fEnemyDir_S[index][0], g_fEnemy_S[index][1] + g_fEnemyDir_S[index][1], g_fEnemy_S[index][2]);
		g_Enemy_S[index]->setPos(vec3(g_fEnemy_S[index][0] + g_fEnemyDir_S[index][0], g_fEnemy_S[index][1] + g_fEnemyDir_S[index][1], g_fEnemy_S[index][2]));
		g_Enemy_S[index]->setTRSMatrix(mxT);
		//printf("(%f,%f,%d,%d)\n", g_Enemy[index]->getPos().x, g_fEnemyCount[index], _iInverse[index],index);
		break;
	}
}

void Attack(float delta,int i,Enemy* Enemy) {
	mat4 mxT;
	vec4 vColor = vec4(1, 0, 0, 1);
	float px, py, pz = 0.0f;
	Enemy->_fAttackTime += delta;
	//printf("%d:%f\n", i, g_Enemy[i]->_fAttackTime);
	if (Enemy->_fAttackTime > Enemy->_fAttackDur && Enemy->_bEnemyDel == false) {
		Enemy->g_Attack[Enemy->_iOut] = new CFlighter(7);
		vColor = vec4(1, 0, 0, 1);
		Enemy->g_Attack[Enemy->_iOut]->setColor(vColor, 7);
		Enemy->g_Attack[Enemy->_iOut]->setShader(g_mxModelView, g_mxProjection, 7);
		Enemy->g_AttackInitPos[Enemy->_iOut][0] = Enemy->getPos().x;
		Enemy->g_AttackInitPos[Enemy->_iOut][1] = Enemy->getPos().y;
		mxT = Translate(Enemy->g_AttackInitPos[Enemy->_iOut][0], Enemy->g_AttackInitPos[Enemy->_iOut][1], pz);
		Enemy->g_Attack[Enemy->_iOut]->setTRSMatrix(mxT);
		Enemy->_iOut++;
		Enemy->_iFree--;
		Enemy->_fAttackTime = 0;
	}
	for (int j = 0; j < 20 - Enemy->_iFree; j++) {
		if (Enemy->g_Attack[j] != nullptr) {
			Enemy->g_fAttackDir[j] -= delta;
			mxT = Translate(Enemy->g_AttackInitPos[j][0], Enemy->g_AttackInitPos[j][1] + Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed, 0);
			Enemy->g_Attack[j]->setPos(vec3(Enemy->g_AttackInitPos[j][0], Enemy->g_AttackInitPos[j][1] + Enemy->g_fAttackDir[j] * Enemy->_fAttackSpeed, 0));
			Enemy->g_Attack[j]->setTRSMatrix(mxT);
			//out of windowns reset
			bool _bAttack = g_Player[0]->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1.5);
			//protecy player
			// 
			//bool _bDefend = g_FiveStar->CheckCollider(Enemy->g_Attack[j]->getPos().x, Enemy->g_Attack[j]->getPos().y, 1);
			//printf("(%f,%f),(%f,%f)\n", g_FiveStar->getPos().x, g_FiveStar->getPos().y, g_Player[0]->getPos().x, g_Player[0]->getPos().y);
			/*if (_bDefend) {
				Enemy->_bAttackSus[j] = true;
			}*/
			if (_bAttack) {
				printf("Attack");
				Enemy->_bAttackSus[j] = true;
				_fShootDur += 1.0f;
			}
		}
	}
	for (int k = 0; k < Enemy->_iOut; k++) {
		//printf("%d,%d:(%f)\n", i, k, g_Enemy[i]->g_Attack[k]->getPos().y);
		if (Enemy->g_Attack[k] != nullptr) {
			if (Enemy->g_Attack[k]->getPos().y < -13.8f || Enemy->_bAttackSus[k]) {
				Enemy->_iFree++;
				for (int l = 0; l < Enemy->_iOut - 1; l++) {
					Enemy->g_fAttackDir[l] = Enemy->g_fAttackDir[l + 1];
					Enemy->g_AttackInitPos[l][0] = Enemy->g_AttackInitPos[l + 1][0];
					Enemy->g_AttackInitPos[l][1] = Enemy->g_AttackInitPos[l + 1][1];
					Enemy->g_Attack[l] = Enemy->g_Attack[l + 1];
					Enemy->g_Attack[l]->setPos(vec3(Enemy->g_AttackInitPos[l][0], Enemy->g_AttackInitPos[l][1] + Enemy->g_fAttackDir[l] * Enemy->_fAttackSpeed, 0));
				}
				Enemy->g_fAttackDir[Enemy->_iOut - 1] = 0;
				Enemy->_iOut--;
			}
			if (Enemy->_iOut == 0) {
				Enemy->_bAttackOut = true;
			}
		}
	}
}
float getRandomf(float _max, float _min) {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> distr(_min, _max);
	float fx = distr(eng);
	return fx;
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
			if(g_Enemy[i]!= NULL)
				delete g_Enemy[i];
		}
		for (int i = 0; i < _iGenCount_S; i++) {
			if (g_Enemy_S[i] != nullptr)
				delete g_Enemy_S[i];
		}
		exit(EXIT_SUCCESS);
		break;
	}
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
		if (state == GLUT_DOWN);
		break;
	case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 
		if (state == GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
		if (state == GLUT_DOWN);
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
	case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵，移動 Red 往左
		
		break;
	case GLUT_KEY_RIGHT:		// 目前按下的是向右方向鍵，移動 Red 往右
		
		break;
	case GLUT_KEY_UP:		// 目前按下的是向右方向鍵，移動 Red 往右
		
		break;
	case GLUT_KEY_DOWN:		// 目前按下的是向右方向鍵，移動 Red 往右
		
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
