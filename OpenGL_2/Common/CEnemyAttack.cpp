#include "CEnemyAttack.h"

CEnemyAttack::CEnemyAttack(int type) {
	_Points = new vec4[ATTACK_NUM];
	for (int i = 0; i < 400; i++) {
		_Points[i].x = RADIUS * cosf(M_PI * 2.0f * i / 400);
		_Points[i].y = RADIUS * sinf(M_PI * 2.0f * i / 400);
		_Points[i].z = 0.0f;
		_Points[i].w = 1.0f;
	}
	_Colors = new vec4[ATTACK_NUM];
	_Colors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	_Colors[1] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	_Colors[2] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[3] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_Colors[4] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[5] = vec4(0.0f, 0.0f, 1.0f, 1.0f);


	// Create and initialize a buffer object 
	CreateBufferObject(ATTACK_NUM);
	_bUpdateProj = false;
}
void CEnemyAttack::draw()
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
	glDrawArrays(GL_POLYGON, 0, ATTACK_NUM);
}

void CEnemyAttack::drawW()
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
	glDrawArrays(GL_TRIANGLES, 0, ATTACK_NUM);
}
void CEnemyAttack::StartAttack(CEnemyAttack* Ea,int type, vec3 Pos, float delta) {
	mat4 mxT;
	vColor = vec4(0, 0, 1, 1);
	Ea->setColor(vColor);
	mxT = Translate(Pos.x, Pos.y, Pos.z);
	Ea->setTRSMatrix(mxT);
	//Ea->setShader();
}