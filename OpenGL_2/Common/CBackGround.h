#pragma once

#include "../header/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define PI 3.1415926f
#define RADIUS 0.8
#define CLOUD_NUM 404  // 2 faces, 2 triangles/face 
#define CLOUD_V2_NUM 202 
class CBackGround
{
private:
	vec4 _CloudPoints[CLOUD_NUM];
	vec4 _CloudColors[CLOUD_NUM];
	// VAO
	GLuint _uiVao;
	// VBO
	GLuint _uiBuffer;
	//  for Shader
	GLuint _uiProgram;
	// Vertex Position Attribute
	GLuint _uiModelView, _uiProjection;
	// Matrix 
	mat4 _mxView, _mxProjection;
	mat4 _mxMVFinal, _mxTRS;

	// 紀錄是否有矩陣的更新
	bool  _bUpdateMV;
	bool  _bUpdateProj;

	void CreateBufferObject();
public:
	CBackGround(int type);

	void setShader(mat4& mxModelView, mat4& mxProjection, GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	GLuint getShaderHandle() { return _uiProgram; }
	void setViewMatrix(mat4& mat);
	void setProjectionMatrix(mat4& mat);
	void setTRSMatrix(mat4& mat);
	void setColor(GLfloat vColor[4],int type); // Single color
	void setVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[]); // four Vertices' Color

	void draw(int type);
	void drawW();
};



