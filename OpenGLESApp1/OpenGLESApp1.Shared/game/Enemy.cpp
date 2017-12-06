#include "pch.h"
#include "Enemy.h"
#include "gmUtil.h"


bool Enemy::update(float time) {


	pos+=dir; 
	loops++; 

	//Aabb.Init( pos.x - spr->width/2, pos.y - spr->height/2,   spr->width, spr->height  );
	return true;
}


Enemy::~Enemy(){

}


void Enemy::init(){
}

void Enemy::cleanUp(){
}

bool Enemy::collides(Vector2D<float> pos, Vector2D<float> size){
	return false;
}