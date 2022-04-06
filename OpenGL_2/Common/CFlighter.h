#pragma once

#define CCFlighter_H
#include "../header/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define QUAD_NUM 12  // 2 faces, 2 triangles/face 
#define CIRCLE_NUM 500 
#define FIVESTART_NUM 5
#define RADIUS 0.4f
#define PI 3.1415926f
class CFlighter
{
private:
	vec4 _Points[QUAD_NUM];
	vec4 _Colors[QUAD_NUM];
	//for circle
	vec4 _CPoints[CIRCLE_NUM];
	vec4 _CColors[CIRCLE_NUM];
	//for five star
	vec4 _SPoints[FIVESTART_NUM];
	vec4 _SColors[FIVESTART_NUM];
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
	mat4 _mxMVFinal, _mxTRS, _mxT;

	// 紀錄是否有矩陣的更新
	bool  _bUpdateMV;
	bool  _bUpdateMT;
	bool  _bUpdateProj;

	void CreateBufferObject(int type);
public:
	CFlighter(int type);

	void setShader(mat4& mxModelView, mat4& mxProjection, int type, GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	GLuint getShaderHandle() { return _uiProgram; }
	void setViewMatrix(mat4& mat);
	void setProjectionMatrix(mat4& mat);
	void setTRSMatrix(mat4& mat);
	void setTMatrix(mat4& mat);
	void setColor(GLfloat vColor[4],int index); // Single color
	void setVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[]); // four Vertices' Color

	void draw(int type);
	void drawW();
};




