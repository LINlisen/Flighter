#include "Enemy.h"


Enemy::Enemy(int type)
{
	switch (type)
	{
	case 1:
		_Points = new vec4[ENEMY_ONE];
		for (int i = 0; i < ENEMY_ONE; i++) {
			if (i < 5) {
				_Points[0].x = SARTRADIUS * cos(90 * 2 * PI / 360);
				_Points[0].y = -SARTRADIUS * sin(90 * 2 * PI / 360);
				_Points[0].z = -1.0f; _Points[0].w = 1.0f;
				_Points[1].x = SARTRADIUS * cos(306 * 2 * PI / 360);
				_Points[1].y = -SARTRADIUS * sin(306 * 2 * PI / 360);
				_Points[1].z = -1.0f; _Points[1].w = 1.0f;
				_Points[2].x = SARTRADIUS * cos(162 * 2 * PI / 360);
				_Points[2].y = -SARTRADIUS * sin(162 * 2 * PI / 360);
				_Points[2].z = -1.0f; _Points[2].w = 1.0f;
				_Points[3].x = SARTRADIUS * cos(18 * 2 * PI / 360);
				_Points[3].y = -SARTRADIUS * sin(18 * 2 * PI / 360);
				_Points[3].z = -1.0f; _Points[3].w = 1.0f;
				_Points[4].x = SARTRADIUS * cos(234 * 2 * PI / 360);
				_Points[4].y = -SARTRADIUS * sin(234 * 2 * PI / 360);
				_Points[4].z = -1.0f; _Points[4].w = 1.0f;
			}
			else {
				_Points[i].x = RADIUS * cosf(M_PI * 2.0f * i / 200);
				_Points[i].y = -RADIUS * sinf(M_PI * 2.0f * i / 200);
				_Points[i].z = 0.0f;
				_Points[i].w = 1.0f;
			}

		}
		CreateBufferObject(ENEMY_ONE);
		break;
	case 2:
		_Points = new vec4[200];
		for (int i = 0; i < 200; i++) {
			_Points[i].x = 200 * cosf(M_PI * 2.0f * i / 200);
			_Points[i].y = -200 * sinf(M_PI * 2.0f * i / 200);
			_Points[i].z = 0.0f;
			_Points[i].w = 1.0f;
		}
		CreateBufferObject(200);
		break;
	}
	_Colors = new vec4[ENEMY_ONE];
	_Colors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	_Colors[1] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	_Colors[2] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[3] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_Colors[4] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	_Colors[5] = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	// Create and initialize a buffer object 
	
	_bUpdateProj = false;
}

void Enemy::draw()
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
	glDrawArrays(GL_POLYGON, 0, ENEMY_ONE);
}

void Enemy::drawW()
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
	glDrawArrays(GL_TRIANGLES, 0, ENEMY_ONE);
}
mat4 _mxModelView(1.0f);
mat4 _mxIdentity(1.0f);
mat4 _mxProjection;
void Enemy::Attack(float delta) {
	mat4 mxT;
	vec4 vColor = vec4(1, 0, 0, 1);
	_fAttackTime += delta;
	if (_fAttackTime > _fAttackDur) {
		g_Attack[_iOut] = new Enemy(2);
		vColor = vec4(1, 0, 0, 1);
		g_Attack[_iOut]->setColor(vColor);
		g_Attack[_iOut]->setShader(_mxModelView, _mxProjection);
		mxT = Translate(g_AttackInitPos[_iOut][0], g_AttackInitPos[_iOut][1], g_AttackInitPos[_iOut][2]);
		g_Attack[_iOut]->setTRSMatrix(mxT);
		_iOut++;
		_iFree--;
		_fAttackTime -= _fAttackDur;

	}
	for (int i = 0; i < 20 - _iFree; i++) {
		g_fAttackDir[i] += delta;
		mxT = Translate(g_AttackInitPos[i][0], g_AttackInitPos[i][1] + g_fAttackDir[i] * _fAttackSpeed, 0);
		g_Attack[i]->setTRSMatrix(mxT);
		g_Attack[i]->setPos(vec3(g_AttackInitPos[i][0], g_AttackInitPos[i][1] + g_fAttackDir[i] * _fAttackSpeed, 0));
		//out of windowns reset

	}
	for (int i = 0; i < _iOut; i++) {
		if (g_Attack[i]->getPos().y > 13.8f) {
			_iFree++;

			for (int j = 0; j < _iOut - 1; j++) {
				g_fAttackDir[j] = g_fAttackDir[j + 1];
				g_AttackInitPos[j][0] = g_AttackInitPos[j + 1][0];
				g_AttackInitPos[j][1] = g_AttackInitPos[j + 1][1];
				g_Attack[j] = g_Attack[j + 1];
				g_Attack[j]->setPos(vec3(g_AttackInitPos[i][0], g_AttackInitPos[i][1] + g_fAttackDir[i] * _fAttackSpeed, 0));
			}
			g_fAttackDir[_iOut - 1] = 0;
			_iOut--;
		}
	}
}