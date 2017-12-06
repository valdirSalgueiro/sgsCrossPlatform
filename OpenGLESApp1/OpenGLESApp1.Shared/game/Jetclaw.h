#ifndef JETCLAW_H
#define JETCLAW_H

#include "Enemy.h"
#include "Vector2D.h"
#include "gl2d.h"
#include "Aabb.h"

#include <vector>

#include "../bulletML/object.h"
#include "bulletmlparser.h"
#include "bulletmlparser-tinyxml.h"

class Jetclaw : public Enemy{
public:

	Jetclaw();
	~Jetclaw();

	bool update(float time);
	bool render(float time);
	void init();
	void cleanUp();
	bool collides(Vector2D<float> pos,Vector2D<float> size);

	std::vector< Vector2D<float> > path;
	Vector2D<float> oldPosition;

	int pathIter;
	float x;
	int color;

	BulletMLParser* bp;
};

#endif