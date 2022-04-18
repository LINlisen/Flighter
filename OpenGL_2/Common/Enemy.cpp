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
		_Colors = new vec4[ENEMY_ONE];
		CreateBufferObject(ENEMY_ONE);
		break;
	case 2:
		_Points = new vec4[ENEMY_ONE];
		for (int i = 0; i < ENEMY_ONE; i++) {
			if (i < 5) {
				_Points[0].x = SARTRADIUS * cos(90 * 2 * PI / 360)* sin(i);
				_Points[0].y = SARTRADIUS * sin(90 * 2 * PI / 360) * sin(i);
				_Points[0].z = 1.0f; _Points[0].w = 1.0f;
				_Points[1].x = SARTRADIUS * cos(306 * 2 * PI / 360) * sin(i);
				_Points[1].y = SARTRADIUS * sin(306 * 2 * PI / 360) * sin(i);
				_Points[1].z = -1.0f; _Points[1].w = 1.0f;
				_Points[2].x = SARTRADIUS * cos(162 * 2 * PI / 360) * sin(i);
				_Points[2].y = SARTRADIUS * sin(162 * 2 * PI / 360) * sin(i);
				_Points[2].z = 1.0f; _Points[2].w = 1.0f;
				_Points[3].x = SARTRADIUS * cos(18 * 2 * PI / 360) * sin(i);
				_Points[3].y = SARTRADIUS * sin(18 * 2 * PI / 360) * sin(i);
				_Points[3].z = 1.0f; _Points[3].w = 1.0f;
				_Points[4].x = SARTRADIUS * cos(234 * 2 * PI / 360) * sin(i);
				_Points[4].y = SARTRADIUS * sin(234 * 2 * PI / 360) * sin(i);
				_Points[4].z = 1.0f; _Points[4].w = 1.0f;
			}
			else {
				_Points[i].x = RADIUS * cosf(M_PI * 2.0f * i / 200);
				_Points[i].y = -RADIUS * sinf(M_PI * 2.0f * i / 200);
				_Points[i].z = 0.0f;
				_Points[i].w = 1.0f;
			}

		}
		_Colors = new vec4[ENEMY_ONE];
		CreateBufferObject(ENEMY_ONE);
		break;
	}
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
