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
		for (int i = 0; i < 400; i++) {
			_CPoints[i].x = RADIUS * cosf(M_PI * 2.0f * i / 400);
			_CPoints[i].y = RADIUS * sinf(M_PI * 2.0f * i / 400);
			_CPoints[i].z = 0.0f;
			_CPoints[i].w = 1.0f;
		}
		for (int i = 400; i < CIRCLE_NUM; i++) {
			_CPoints[i].x = cosf(M_PI * 2.0f * i / 400);
			_CPoints[i].y = sinf(M_PI * 2.0f * i / 400);
			_CPoints[i].z = 0.0f;
			_CPoints[i].w = 1.0f;
		}
		break;
	//for five start
	case 4 : 
		_SPoints[0].x = RADIUS * cos(90 * 2 * PI / 360);
		_SPoints[0].y = RADIUS * sin(90 * 2 * PI / 360);
		_SPoints[0].z = -1.0f; _SPoints[0].w = 1.0f;
		_SPoints[1].x = RADIUS * cos(306 * 2 * PI / 360);
		_SPoints[1].y = RADIUS * sin(306 * 2 * PI / 360);
		_SPoints[1].z = -1.0f; _SPoints[1].w = 1.0f;
		_SPoints[2].x = RADIUS * cos(162 * 2 * PI / 360);
		_SPoints[2].y = RADIUS * sin(162* 2 * PI / 360);
		_SPoints[2].z = -1.0f; _SPoints[2].w = 1.0f;
		_SPoints[3].x = RADIUS * cos(18* 2 * PI / 360);
		_SPoints[3].y = RADIUS * sin(18* 2 * PI / 360);
		_SPoints[3].z = -1.0f; _SPoints[3].w = 1.0f;
		_SPoints[4].x = RADIUS * cos(234* 2 * PI / 360);
		_SPoints[4].y = RADIUS * sin(234* 2 * PI / 360);
		_SPoints[4].z = -1.0f; _SPoints[4].w = 1.0f;
		break;
	//for MISSILE_NUM
	case 5:
		for (int i = 0; i < MISSILE_NUM; i++) {
			_MPoints[i].x = RADIUS * sin(i)*cos(i);
			_MPoints[i].y = RADIUS * cos(i);
			_MPoints[i].z = 0.0f;
			_MPoints[i].w = 1.0f;
		}
		break;
	//for Flighter Upgrade 1
	case 6:
		for (int i = 0; i < UPGRADE_ONE; i++) {
			//-0.8f, -1.4f, 0.0f
			_UOPoints[0] = vec4(-1.2f, 1.0f, 0.0f, 1.0f);
			_UOPoints[1] = vec4(-1.2f, -1.4f, 0.0f, 1.0f);
			_UOPoints[2] = vec4(-1.0f, -1.4f, 0.0f, 1.0f);
			_UOPoints[3] = vec4(-1.2f, 1.0f, 0.0f, 1.0f);
			_UOPoints[4] = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
			_UOPoints[5] = vec4(-1.0f, -1.4f, 0.0f, 1.0f);
		}
	case 7:
		for (int i = 0; i < 200; i++) {
			_APoints[i].x = RADIUS * cosf(M_PI * 2.0f * i / 200);
			_APoints[i].y = RADIUS * sinf(M_PI * 2.0f * i / 200);
			_APoints[i].z = 0.0f;
			_APoints[i].w = 1.0f;
		}
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
	if (type == 3 ) {
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
	else if (type == 5) {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_MPoints) + sizeof(_MColors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_MPoints), _MPoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_MPoints), sizeof(_MColors), _MColors);
	}
	else if (type == 6) {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_UOPoints) + sizeof(_UOColors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_UOPoints), _UOPoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_UOPoints), sizeof(_UOColors), _UOColors);
	}
	else if (type == 7) {
		glGenBuffers(1, &_uiBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_APoints) + sizeof(_AColors), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_APoints), _APoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_APoints), sizeof(_AColors), _AColors);
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
	else if (type == 5) {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_MPoints)));
	}
	else if (type == 6) {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_UOPoints)));
	}
	else if (type == 7) {
		GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_APoints)));
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
			_CColors[i].x = vColor[0]+ i * 0.0008f + 0.15f;
			_CColors[i].y = vColor[1]+ i * 0.0006f + 0.3f;
			_CColors[i].z = vColor[2]+ 0.3f;
			_CColors[i].w = vColor[3];
		}
		break;
	case 5:
		for (int i = 0; i < MISSILE_NUM; i++) {
			_MColors[i].x = vColor[0];
			_MColors[i].y = vColor[1];
			_MColors[i].z = vColor[2];
			_MColors[i].w = vColor[3];
		}
		break;
	case 4:
		for (int i = 0; i < FIVESTART_NUM; i++) {
			_SColors[i].x = vColor[0];
			_SColors[i].y = vColor[1];
			_SColors[i].z = vColor[2];
			_SColors[i].w = vColor[3];
		}
		break;
	case 6:
		for (int i = 0; i < UPGRADE_ONE; i++) {
			_UOColors[i].x = vColor[0];
			_UOColors[i].y = vColor[1];
			_UOColors[i].z = vColor[2];
			_UOColors[i].w = vColor[3];
		}
		break;
	case 7:
		for (int i = 0; i < 200; i++) {
			_AColors[i].x = vColor[0];
			_AColors[i].y = vColor[1];
			_AColors[i].z = vColor[2];
			_AColors[i].w = vColor[3];
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
	else if (index == 5) {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_MPoints), sizeof(_MColors), _MColors);
	}
	else if (index == 6) {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_UOPoints), sizeof(_UOColors), _UOColors);
	}
	else if (index == 7) {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_APoints), sizeof(_AColors), _AColors);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_Points), sizeof(_Colors), _Colors);
	}
	
}

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
	else if(type ==5 ) glDrawArrays(GL_POLYGON, 0, MISSILE_NUM);
	else if  (type ==6) glDrawArrays(GL_POLYGON, 0, UPGRADE_ONE);
	else if (type ==7)  glDrawArrays(GL_POLYGON, 0, 200);
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

void CFlighter::setPos(vec3 Pos) {
	_mxPos = Pos;
}

vec3 CFlighter::getPos() {
	return _mxPos;
}

bool  CFlighter::CheckCollider(float x, float y, float r) {
	if (r * r > (x - _mxPos.x) * (x - _mxPos.x) + (y - _mxPos.y) * (y - _mxPos.y)) return true;
	else return false;
}
