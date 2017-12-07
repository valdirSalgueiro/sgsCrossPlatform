#include "pch.h"
#include "Enemy.h"
#include "gmUtil.h"

extern float elapsedTime;

bool Enemy::update(float time) {


	pos+=dir; 
	loops++; 

	if (wasHurt && (elapsedTime - hurtTimer > 0.05f)) {
		wasHurt = false;
	}

	//Aabb.Init( pos.x - spr->width/2, pos.y - spr->height/2,   spr->width, spr->height  );
	return true;
}


void Enemy::hurt() {
	wasHurt = true;
	hurtTimer = elapsedTime;
	energy--;
}


Enemy::~Enemy(){

}


void Enemy::init(){
	wasHurt = false;
}

void Enemy::cleanUp(){
}

bool Enemy::collides(Vector2D<float> pos, Vector2D<float> size){
	return false;
}