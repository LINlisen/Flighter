#include "CShape.h"


CShape::CShape()
{
	_Points = nullptr; // 儲存頂點的指標變數
	_Colors = nullptr; // 儲存頂點顏色的指標變數
	_nVtx = 0; // 這個物件有幾個 vertex
	_pVXshader = NULL;
	_pFSshader = NULL; //vertex 與 pixel shader 的字元指標
	_fClock = 0;

	_uiVao = 0;
	_uiBuffer = 0;
	_bUpdateMV = false;
	_bUpdateProj = false;
}

CShape::~CShape()
{
	if (_Points != nullptr) delete[] _Points;
	if (_Colors != nullptr) delete[] _Colors;

	if (_pVXshader != NULL) delete[] _pVXshader;
	if (_pFSshader != NULL) delete[] _pFSshader;
}

void CShape::CreateBufferObject(int nVtx) {
	_nVtx = nVtx;
	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);

	// Create and initialize a buffer object

	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, _nVtx * sizeof(vec4) * 2, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, _nVtx * sizeof(vec4), _Points);
	glBufferSubData(GL_ARRAY_BUFFER, _nVtx * sizeof(vec4), _nVtx * sizeof(vec4), _Colors);
}

void CShape::setShader(mat4& mxView, mat4& mxProjection, GLuint uiShaderHandle) {
	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(_nVtx * sizeof(vec4)));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
};

void CShape::setShaderName(const char vxShader[], const char fsShader[]) {
	int len;
	len = strlen(vxShader);
	_pVXshader = new char[len+1];
	memcpy(_pVXshader, vxShader, len);

	len = strlen(fsShader);
	_pFSshader = new char[len+1];
	memcpy(_pFSshader, fsShader, len);
}

void CShape::setViewMatrix(mat4& mat) {
	_mxView = mat;
	_bUpdateMV = true;
}
void CShape::setProjectionMatrix(mat4& mat) {
	_mxProjection = mat;
	_bUpdateProj = true;
}
void CShape::setTRSMatrix(mat4& mat) {
	_mxTRS = mat;
	_bUpdateMV = true;
}
void CShape::setColor(GLfloat vColor[4]) {
	for (int i = 0; i < 6; i++) {
		_Colors[i].x = vColor[0];
		_Colors[i].y = vColor[1];
		_Colors[i].z = vColor[2];
		_Colors[i].w = vColor[3];
	}
	for (int i = 0; i < 4; i++) {
		_fOriginColor[i] = vColor[i];
	}

	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, _nVtx * sizeof(vec4), _nVtx * sizeof(vec4), _Colors);
}// Single color
void CShape::setOriginColor() {
	for (int i = 0; i < 6; i++) {
		_Colors[i].x = _fOriginColor[0];
		_Colors[i].y = _fOriginColor[1];
		_Colors[i].z = _fOriginColor[2];
		_Colors[i].w = _fOriginColor[3];
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, _nVtx * sizeof(vec4), _nVtx * sizeof(vec4), _Colors);
}
void CShape::draw() {
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
	glDrawArrays(GL_TRIANGLES, 0, _nVtx);
}

void CShape::drawW() {
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
	glDrawArrays(GL_TRIANGLES, 0, _nVtx);
}
void CShape::setPos(vec3 Pos) {
	_mxPos = Pos;
}

vec3 CShape::getPos() {
	return _mxPos;
}

bool  CShape::CheckCollider(float x, float y, float r) {
	if (r * r > (x - _mxPos.x) * (x - _mxPos.x) + (y - _mxPos.y) * (y - _mxPos.y)) return true;
	else return false;
}

float CShape::getVectorAngle(vec2 posA, vec2 posB) {
	float angle = acosf(dot(posA, posB) / (length(posA) * length(posB)));
	printf("%f\n",angle);
	return angle*180/3.14;
}