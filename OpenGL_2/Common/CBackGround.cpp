#include "CBackGround.h"

CBackGround::CBackGround(int type)
{
	switch (type)
	{
	case 1:
		for (int i = 0; i <= 100; i++) {
			if (i <= 50) {
				_CloudPoints[i].x = cos(i * PI / 50);
				_CloudPoints[i].y = sin(i * PI / 50);
			}
			else {
				_CloudPoints[i].x = cos(i * PI / 50) + 1.8f;
				_CloudPoints[i].y = -sin(i * PI / 50) * 0.8;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		for (int i = 101; i <= 201; i++) {
			if (i <= 151) {
				_CloudPoints[i].x = cos((i - 101) * PI / 50);
				_CloudPoints[i].y = -sin((i - 101) * PI / 50) * 0.5;
			}
			else {
				_CloudPoints[i].x = cos((i - 101) * PI / 50) + 1.8f;
				_CloudPoints[i].y = sin((i - 101) * PI / 50) * 0.6;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		for (int i = 202; i <= 302; i++) {
			if (i <= 252) {
				_CloudPoints[i].x = cos((i - 202) * PI / 50);
				_CloudPoints[i].y = sin((i - 202) * PI / 50);
			}
			else {
				_CloudPoints[i].x = cos((i - 202) * PI / 50) + 2.8f;
				_CloudPoints[i].y = -sin((i - 202) * PI / 50) * 0.8;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		for (int i = 303; i <= 403; i++) {
			if (i <= 353) {
				_CloudPoints[i].x = cos((i - 303) * PI / 50);
				_CloudPoints[i].y = -sin((i - 303) * PI / 50) * 0.5;
			}
			else {
				_CloudPoints[i].x = cos((i - 303) * PI / 50) + 2.8f;
				_CloudPoints[i].y = sin((i - 303) * PI / 50) * 0.6;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		break;
	case 2:
		for (int i = 0; i <= 100; i++) {
			if (i <= 50) {
				_CloudPoints[i].x = cos(i * PI / 50);
				_CloudPoints[i].y = sin(i * PI / 50);
			}
			else {
				_CloudPoints[i].x = cos(i * PI / 50) + 1.8f;
				_CloudPoints[i].y = -sin(i * PI / 50) * 0.8;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		for (int i = 101; i <= 201; i++) {
			if (i <= 151) {
				_CloudPoints[i].x = cos((i - 101) * PI / 50);
				_CloudPoints[i].y = sin((i - 101) * PI / 50) * 0.5;
			}
			else {
				_CloudPoints[i].x = cos((i - 101) * PI / 50) + 1.8f;
				_CloudPoints[i].y = -sin((i - 101) * PI / 50) * 0.6;
			}
			_CloudPoints[i].z = -1.0f;
			_CloudPoints[i].w = 1;
		}
		break;
	}
	CreateBufferObject();
	_bUpdateProj = false;

	_CloudColors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	_CloudColors[1] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	_CloudColors[2] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_CloudColors[3] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_CloudColors[4] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_CloudColors[5] = vec4(0.0f, 0.0f, 1.0f, 1.0f);


}


void CBackGround::CreateBufferObject()
{
	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);

	// Create and initialize a buffer object

	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_CloudPoints) + sizeof(_CloudColors), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_CloudPoints), _CloudPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(_CloudPoints), sizeof(_CloudColors), _CloudColors);
}

void CBackGround::setShader(mat4& mxView, mat4& mxProjection, GLuint uiShaderHandle)
{
	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_CloudPoints)));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CBackGround::setViewMatrix(mat4& mat)
{
	_mxView = mat;
	_bUpdateMV = true;
}

void CBackGround::setProjectionMatrix(mat4& mat)
{
	_mxProjection = mat;
	_bUpdateProj = true;
}

void CBackGround::setTRSMatrix(mat4& mat)
{
	_mxTRS = mat;
	_bUpdateMV = true;
}

void CBackGround::setColor(GLfloat vColor[4],int type)
{
	if (type == 1) {
		for (int i = 0; i < CLOUD_NUM; i++) {
			_CloudColors[i].x = vColor[0];
			_CloudColors[i].y = vColor[1];
			_CloudColors[i].z = vColor[2];
			_CloudColors[i].w = vColor[3];
		}
	}
	else {
		for (int i = 0; i < CLOUD_V2_NUM; i++) {
			_CloudColors[i].x = vColor[0];
			_CloudColors[i].y = vColor[1];
			_CloudColors[i].z = vColor[2];
			_CloudColors[i].w = vColor[3];
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(_CloudPoints), sizeof(_CloudColors), _CloudColors);
}

void CBackGround::setVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[])
{
	_CloudColors[0].x = vLFColor[0];
	_CloudColors[0].y = vLFColor[1];
	_CloudColors[0].z = vLFColor[2];
	_CloudColors[0].w = vLFColor[3];
	_CloudColors[3] = _CloudColors[0];

	_CloudColors[1].x = vLRColor[0];
	_CloudColors[1].y = vLRColor[1];
	_CloudColors[1].z = vLRColor[2];
	_CloudColors[1].w = vLRColor[3];

	_CloudColors[2].x = vTRColor[0];
	_CloudColors[2].y = vTRColor[1];
	_CloudColors[2].z = vTRColor[2];
	_CloudColors[2].w = vTRColor[3];
	_CloudColors[4] = _CloudColors[2];

	_CloudColors[5].x = vTLColor[0];
	_CloudColors[5].y = vTLColor[1];
	_CloudColors[5].z = vTLColor[2];
	_CloudColors[5].w = vTLColor[3];

	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(_CloudPoints), sizeof(_CloudColors), _CloudColors);
}

void CBackGround::draw(int type)
{
	glUseProgram(_uiProgram);
	glBindVertexArray(_uiVao);
	if (_bUpdateMV) {
		_mxMVFinal = _mxView * _mxTRS;
		_bUpdateMV = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_bUpdateProj) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_bUpdateProj = false;
	}
	if(type == 1) glDrawArrays(GL_POLYGON, 0, CLOUD_NUM);
	else glDrawArrays(GL_POLYGON, 0, CLOUD_V2_NUM);

}

void CBackGround::drawW()
{
	glBindVertexArray(_uiVao);

	if (_bUpdateMV) {
		_mxMVFinal = _mxView * _mxTRS;
		_bUpdateMV = false;
	}

	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	if (_bUpdateProj) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_bUpdateProj = false;
	}
	glDrawArrays(GL_TRIANGLES, 0, CLOUD_NUM);
}
