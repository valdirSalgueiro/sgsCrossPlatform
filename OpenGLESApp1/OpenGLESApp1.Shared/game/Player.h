#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2D.h"
#include "gl2d.h"
#include "Aabb.h"

#include <vector>

class BulletMLParser;

class Player{
public:
	Player();
	~Player();

	bool update(float time);
	bool render(float time);
	void init();
	void cleanUp();
	bool collides(Vector2D<float> pos);

private:
	glImage spr[22];
	int loops;

	BulletMLParser* bps[5];


};

#endif