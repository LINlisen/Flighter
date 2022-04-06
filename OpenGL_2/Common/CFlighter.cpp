#include "CFlighter.h"


CFlighter::CFlighter(int type)
{
	switch (type)
	{
	case 1:
		_Points[0] = vec4(-2.0f, -1.5f, 0.0f, 1.0f);
		_Points[1] = vec4(-2.0f, -2.0f, 0.0f, 1.0f);
		_Points[2] = vec4(-1.7f, -2.0f, 0.0f, 1.0f);
		_Points[3] = vec4(-2.0f, -1.5f, 0.0f, 1.0f);
		_Points[4] = vec4(-1.7f, -2.0f, 0.0f, 1.0f);
		_Points[5] = vec4(-1.7f, -1.5f, 0.0f, 1.0f);

		_Points[6] = vec4(-1.7f, -1.2f, 0.0f, 1.0f);
		_Points[7] = vec4(-1.7f, -1.9f, 0.0f, 1.0f);
		_Points[8] = vec4(-1.4f, -1.9f, 0.0f, 1.0f);
		_Points[9] = vec4(-1.7f, -1.2f, 0.0f, 1.0f);
		_Points[10] = vec4(-1.4f, -1.9f, 0.0f, 1.0f);
		_Points[11] = vec4(-1.4f, -1.2f, 0.0f, 1.0f);
		break;
	case 2:

		_Points[0] = vec4(-1.7f, -1.2f, 0.0f, 1.0f);
		_Points[1] = vec4(-1.7f, -1.9f, 0.0f, 1.0f);
		_Points[2] = vec4(-1.4f, -1.9f, 0.0f, 1.0f);
		_Points[3] = vec4(-1.7f, -1.2f, 0.0f, 1.0f);
		_Points[4] = vec4(-1.4f, -1.9f, 0.0f, 1.0f);
		_Points[5] = vec4(-1.4f, -1.2f, 0.0f, 1.0f);

		_Points[6] = vec4(-1.4f, -1.5f, 0.0f, 1.0f);
		_Points[7] = vec4(-1.4f, -2.0f, 0.0f, 1.0f);
		_Points[8] = vec4(-1.1f, -2.0f, 0.0f, 1.0f);
		_Points[9] = vec4(-1.4f, -1.5f, 0.0f, 1.0f);
		_Points[10] = vec4(-1.1f, -2.0f, 0.0f, 1.0f);
		_Points[11] = vec4(-1.1f, -1.5f, 0.0f, 1.0f);
		break;
	//for circle
	case 3 :
		for (int i = 0; i < CIRCLE_NUM; i++) {
			_CPoints[i].x = RADIUS * cosf(M_PI * 2.0f * i / CIRCLE_NUM);
			_CPoints[i].y = RADIUS * sinf(M_PI * 2.0f * i / CIRCLE_NUM);
			_CPoints[i].z = 0.0f;
			_CPoints[i].w = 1.0f;
		}
		break;
	//for five start
	case 4 : 
		_SPoints[0].x = RADIUS * cos(90 * 2 * PI / 360);
		_SPoints[0].y = RADIUS * sin(90 * 2 * PI / 360);
		_SPoints[0].z = 0; _SPoints[0].w = 1.0f;
		_SPoints[1].x = RADIUS * cos(306 * 2 * PI / 360);
		_SPoints[1].y = RADIUS * sin(306 * 2 * PI / 360);
		_SPoints[1].z = 0; _SPoints[1].w = 1.0f;
		_SPoints[2].x = RADIUS * cos(162 * 2 * PI / 360);
		_SPoints[2].y = RADIUS * sin(162* 2 * PI / 360);
		_SPoints[2].z = 0; _SPoints[2].w = 1.0f;
		_SPoints[3].x = RADIUS * cos(18* 2 * PI / 360);
		_SPoints[3].y = RADIUS * sin(18* 2 * PI / 360);
		_SPoints[3].z = 0; _SPoints[3].w = 1.0f;
		_SPoints[4].x = RADIUS * cos(234* 2 * PI / 360);
		_SPoints[4].y = RADIUS * sin(234* 2 * PI / 360);
		_SPoints[4].z = 0; _SPoints[4].w = 1.0f;
		break;
	}
	

	_Colors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	_Colors[1] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	_Colors[2] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[3] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_Colors[4] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[5] = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	
	// Create and initialize a buffer object 
	CreateBufferObject(type);
	_bUpdateProj = false;
}


void CFlighter::CreateBufferObject(int type)
{
	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);

	// Create and initialize a buffer object
	if (type == 3) {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_CPoints) + sizeof(_CColors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_CPoints), _CPoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_CPoints), sizeof(_CColors), _CColors);
	}
	else if (type == 4) {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_SPoints) + sizeof(_SColors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_SPoints), _SPoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_SPoints), sizeof(_SColors), _SColors);
	}
	else {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_Points) + sizeof(_Colors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_Points), _Points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_Points), sizeof(_Colors), _Colors);
	}
	
}

void CFlighter::setShader(mat4& mxView, mat4& mxProjection, int type, GLuint uiShaderHandle)
{
	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	if (type == 3) {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_CPoints)));
	}
	else if (type == 4) {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_SPoints)));
	}
	else {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_Points)));
	}
	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CFlighter::setViewMatrix(mat4& mat)
{
	_mxView = mat;
	_bUpdateMV = true;
}

void CFlighter::setProjectionMatrix(mat4& mat)
{
	_mxProjection = mat;
	_bUpdateProj = true;
}

void CFlighter::setTRSMatrix(mat4& mat)
{
	_mxTRS = mat;
	_bUpdateMV = true;
}
void CFlighter::setTMatrix(mat4& mat)
{
	_mxT = mat;
	_bUpdateMT = true;
}
void CFlighter::setColor(GLfloat vColor[4], int index)
{
	switch (index)
	{
	case 1:
		for (int i = 0; i < 6; i++) {
			_Colors[i].x = vColor[0];
			_Colors[i].y = vColor[1];
			_Colors[i].z = vColor[2];
			_Colors[i].w = vColor[3];
		}
		break;
	case 2:
		for (int i = 6; i < 12; i++) {
			_Colors[i].x = vColor[0];
			_Colors[i].y = vColor[1];
			_Colors[i].z = vColor[2];
			_Colors[i].w = vColor[3];
		}
		break;
	case 3:
		for (int i = 0; i < CIRCLE_NUM; i++) {
			_CColors[i].x = vColor[0];
			_CColors[i].y = vColor[1];
			_CColors[i].z = vColor[2];
			_CColors[i].w = vColor[3];
		}
	case 4:
		for (int i = 0; i < FIVESTART_NUM; i++) {
			_SColors[i].x = vColor[0];
			_SColors[i].y = vColor[1];
			_SColors[i].z = vColor[2];
			_SColors[i].w = vColor[3];
		}
	}
	if (index == 3) {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_CPoints), sizeof(_CColors), _CColors);
	}
	else if (index == 4) {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_SPoints), sizeof(_SColors), _SColors);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_Points), sizeof(_Colors), _Colors);
	}
	
}

//void CFlighter::setVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[])
//{
//	_Colors[0].x = vLFColor[0];
//	_Colors[0].y = vLFColor[1];
//	_Colors[0].z = vLFColor[2];
//	_Colors[0].w = vLFColor[3];
//	_Colors[3] = _Colors[0];
//
//	_Colors[1].x = vLRColor[0];
//	_Colors[1].y = vLRColor[1];
//	_Colors[1].z = vLRColor[2];
//	_Colors[1].w = vLRColor[3];
//
//	_Colors[2].x = vTRColor[0];
//	_Colors[2].y = vTRColor[1];
//	_Colors[2].z = vTRColor[2];
//	_Colors[2].w = vTRColor[3];
//	_Colors[4] = _Colors[2];
//
//	_Colors[5].x = vTLColor[0];
//	_Colors[5].y = vTLColor[1];
//	_Colors[5].z = vTLColor[2];
//	_Colors[5].w = vTLColor[3];
//
//	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(_Points), sizeof(_Colors), _Colors);
//}

void CFlighter::draw(int type)
{
	glUseProgram(_uiProgram);
	glBindVertexArray(_uiVao);
	if (_bUpdateMV) {
		_mxMVFinal = _mxView * _mxTRS;
		_bUpdateMV = false;
	}
	if (_bUpdateMT) {
		_mxMVFinal = _mxMVFinal * _mxT;
		_bUpdateMT = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_bUpdateProj) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_bUpdateProj = false;
	}
	if (type == 3) glDrawArrays(GL_POLYGON, 0, CIRCLE_NUM);	
	else if (type == 4) glDrawArrays(GL_POLYGON, 0, FIVESTART_NUM);
	else glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}

void CFlighter::drawW()
{
	glBindVertexArray(_uiVao);

	if (_bUpdateMV) {
		_mxMVFinal = _mxView * _mxTRS;
		_bUpdateMV = false;
	}
	if (_bUpdateMT) {
		_mxMVFinal = _mxMVFinal * _mxT;
		_bUpdateMT = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	if (_bUpdateProj) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_bUpdateProj = false;
	}
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}
