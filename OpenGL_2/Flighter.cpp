// The parent-child relationship 
// 水平移動紅色方塊繞著綠色方塊進行逆時針方向旋轉
// 上下移動藍色方塊繞著紅色方塊進行逆時針方向旋轉

#include "header/Angel.h"
#include "Common/CFlighter.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE (SCREEN_SIZE/2) 
#define VP_HALFWIDTH  12.0f
#define FLIGHTER_SIZE 4 
#define GDISTANCE 5
#define BDISTANCE 3

//my code 
#define FBASE 6
//for Flighter
CFlighter* g_Player[FLIGHTER_SIZE];
float g_fPlayer[FLIGHTER_SIZE][3];
mat4 g_initmxS[4];


// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxIdentity(1.0f);
mat4 g_mxProjection;

// Mouse motion
GLfloat g_fTx = 0, g_fTy = 0;

//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();
void CreateQuadRelationship();

void init(void)
{
	//  產生 projection 矩陣，此處為產生正投影矩陣
	g_mxProjection = Ortho(-VP_HALFWIDTH, VP_HALFWIDTH, -VP_HALFWIDTH, VP_HALFWIDTH, -1.0f, 1.0f);

	// 必須在 glewInit(); 執行完後,在執行物件實體的取得
	CreateQuadRelationship();

	glClearColor(0.0, 0.0, 0.0, 1.0); // black background
}

//----------------------------------------------------------------------------

void GL_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the window
	for (int i = 0; i < FLIGHTER_SIZE; i++) {
		g_Player[i]->draw();
	}


	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	GL_Display();
}

//----------------------------------------------------------------------------
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
				mxT = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
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
				mxT = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
			}
			g_Player[i]->setTRSMatrix(mxT);
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
			mxT = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
			g_initmxS[i] = Scale(1.0, 2.0, 0);
			g_Player[i]->setTRSMatrix(mxT * g_initmxS[i]);
		}
		g_Player[i]->setShader(g_mxModelView, g_mxProjection);
	}
}
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
		delete g_pRQuad_Lo;
		delete g_pRQuad_Lt;
		delete g_pRQuad_Rt;
		delete g_pRQuad_Rt;
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
void Win_PassiveMotion(int x, int y) {
	mat4 mxGT, mxT;

	g_fTx = 12.0f * (x - HALF_SIZE) / (HALF_SIZE);
	g_fTy = -12.0f * (y - HALF_SIZE) / (HALF_SIZE);
	mxGT = Translate(g_fTx, g_fTy, 0);
	for (int i = 0; i < FLIGHTER_SIZE; i++) {
		mxT = Translate(g_fPlayer[i][0], g_fPlayer[i][1], g_fPlayer[i][2]);
		g_Player[i]->setTRSMatrix(mxGT * mxT*g_initmxS[i]);
	}
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
