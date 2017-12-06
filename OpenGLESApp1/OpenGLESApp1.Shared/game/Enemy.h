#ifndef ENEMY_H
#define ENEMY_H

#include "Vector2D.h"
#include "gl2d.h"
#include "Aabb.h"

class Enemy{
public:
	Enemy(){
	}

	virtual ~Enemy();

	enum EnemyType{
		NPC_TYPE,
		BIG_RING,
		TENTACLE,
		RING_ORB,
		RING_ORBS,
		BUG_BOMB
	};

	virtual bool update(float time);
	virtual bool render(float time)=0;
	virtual void init();
	virtual void cleanUp();
	virtual bool collides(Vector2D<float> pos,Vector2D<float> size);

	Vector2D<float> pos;
	Vector2D<float> dir;
	float angle;

	int life;

	int loops;

	AABB	Aabb;

	bool isAlive;

	float radius;

	EnemyType type;	

	int energy;



};

#endif