#ifndef CQuadEX2B_H
#define CQuadEX2B_H
#include "../header/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define QUAD_NUM 6  // 2 faces, 2 triangles/face 

class CQuadEX2B
{
private:
	vec4 _Points[QUAD_NUM];
	vec4 _Colors[QUAD_NUM];
	// VAO
	GLuint _uiVao;
	// VBO
	GLuint _uiBuffer;
	//  for Shader
	GLuint _uiProgram ;
	// Vertex Position Attribute
	GLuint _uiModelView, _uiProjection;
	// Matrix 
	mat4 _mxView, _mxProjection;
	mat4 _mxMVFinal, _mxTRS;

	// Angle and Translation
	GLuint _uiAngleLoc, _uiTranLoc;
	float _fAngle;
	float _fTranslate[3];

	// 紀錄是否有矩陣的更新
	bool  _bUpdateMV;
	bool  _bUpdateProj;

	void CreateBufferObject();
public:
	CQuadEX2B();

	void setShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle=MAX_UNSIGNED_INT);
	GLuint GetShaderHandle() { return _uiProgram;}
	void setViewMatrix(mat4 &mat);
	void setProjectionMatrix(mat4 &mat);
	void setTRSMatrix(mat4 &mat);
	void setColor(GLfloat vColor[4]); // Single color
	void setVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[]); // four Vertices' Color

	// Angle and Translation
	void setAngle(float fAngle);
	void setTranslate(float vT[]);

	void Draw();
	void DrawW();
};




#endif