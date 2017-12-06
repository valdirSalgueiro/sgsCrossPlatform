#include "pch.h"
#include "Player.h"
#include "gmUtil.h"
#include "Bullet_.h"

#ifdef WIN32
//#include "../sound.h"
#endif

#include "../bulletML/object.h"
#include "bulletmlparser.h"
#include "bulletmlparser-tinyxml.h"

float playerX=400.0f;
float playerY=240.0f;

int explosion=0;

extern int direction;

int legsAnimationLeft=0;
int legsAnimationRight=0;

int shootAnimation=0;
int bulletAnimation=0;

int framesLeft=0;
int framesRight=0;

bool isShooting;
	
extern bool touch;

#ifdef WIN32
//sound* tiro;
#endif

int jetAnimation=2;

int powerUp;

glImage sprBullet[4];
glImage* shot;

extern void addPlayerObj(Object* object);

const unsigned int sprite_texcoords[] = {
	0,0,277,322, // 0        4810004.png
	0,324,277,322, // 1      4810003.png
	0,648,277,322, // 2      4810002.png
	279,0,277,322, // 3      4810001.png
	558,0,80,236, // 4       500.png
	640,0,136,225, // 5      487.png
	778,0,132,224, // 6      484.png
	279,324,127,222, // 7    485.png
	408,324,138,221, // 8    482.png
	279,548,125,220, // 9    486.png
	912,0,95,215, // 10      498.png
	548,324,157,208, // 11   488.png
	707,324,176,199, // 12   4920003.png
	408,547,176,199, // 13   4920002.png
	408,748,176,199, // 14   4920001.png
	586,547,158,196, // 15   495.png
	746,547,205,166, // 16   493.png
	279,770,102,149, // 17   499.png
	586,745,206,102, // 18   506.png
	586,849,206,102, // 19   504.png
	794,745,194,102, // 20   505.png
	794,849,146,85, // 21    502.png
};

const unsigned int bullet_texcoords[] = {
0,0,232,138, // 0        516.png
0,140,232,138, // 1      515.png
0,280,232,138, // 2      514.png
0,420,90,15, // 3        1.png
};

Player::Player(){
	std::string bulletPattern1("<bulletml><action label=\"top\"><fire><direction type=\"absolute\">90</direction><bullet><speed>3</speed></bullet></fire></action></bulletml>");
	std::string bulletPattern2("<bulletml><action label=\"top\"><fire><direction type=\"relative\">90</direction><bullet><speed>6</speed></bullet></fire><fire><direction type=\"relative\">90</direction><bulletRef label=\"curveLeft\" /></fire><fire><direction type=\"relative\">90</direction><bulletRef label=\"curveRight\" /></fire></action><bullet label=\"curveLeft\"><speed>6</speed><action><changeDirection><direction type=\"relative\">45</direction><term>5</term></changeDirection></action></bullet><bullet label=\"curveRight\"><speed>6</speed><action><changeDirection><direction type=\"relative\">-45</direction><term>5</term></changeDirection></action></bullet></bulletml>");
	std::string bulletPattern3("<bulletml><action label=\"top\"><fire><direction type=\"relative\">135</direction><bulletRef label=\"curveRight\" /></fire><fire><direction type=\"absolute\">90</direction><bullet><speed>4</speed></bullet></fire><fire><direction type=\"relative\">45</direction><bulletRef label=\"curveLeft\" /></fire></action><bullet label=\"curveLeft\"><speed>4</speed><action><wait>8</wait><changeDirection><direction type=\"relative\">90</direction><term>15</term></changeDirection></action></bullet><bullet label=\"curveRight\"><speed>4</speed><action><wait>8</wait><changeDirection><direction type=\"relative\">-90</direction><term>15</term></changeDirection></action></bullet></bulletml>");
	

	powerUp=0;
	isShooting=false;

	glLoadSpriteset("mecha.png",spr,1024,1024,22,sprite_texcoords,0);
	glLoadSpriteset("1.png",sprBullet,256,512,4,bullet_texcoords,0);

#ifdef WIN32
	//tiro=new sound("../res/tiro.wav",25,100,50);
#endif
	shot=&sprBullet[3];


	bps[0] = new BulletMLParserTinyXML(bulletPattern1);
	bps[1] = new BulletMLParserTinyXML(bulletPattern2);
	bps[2] = new BulletMLParserTinyXML(bulletPattern3);
    bps[0]->build();
	bps[1]->build();
	bps[2]->build();
}

Player::~Player(){
	delete bps[0];
	delete bps[1];
	delete bps[2];
}

void Player::init(){
}

bool Player::update(float time) {
	loops++;

	if(touch && !isShooting){
#ifdef WIN32
		//tiro->play();
#endif		
		isShooting=true;
		bulletAnimation=0;
		if(powerUp==0){
			addPlayerObj(new Bullet(bps[0], playerX, playerY, 0, 0,PLAYER));
		}
		else if(powerUp==1){
			addPlayerObj(new Bullet(bps[1], playerX, playerY, 0, 0,PLAYER));
		}
		else if(powerUp==2){
			addPlayerObj(new Bullet(bps[2], playerX, playerY, 0, 0,PLAYER));
		}
	}

	if(isShooting){
		if(powerUp==0){
			if(loops%2==0)
				shootAnimation++;
		}
		else if(powerUp==1){
			if(loops%3==0)
				shootAnimation++;
		}
		else if(powerUp==2){
			if(loops%5==0)
				shootAnimation++;
		}

		if(shootAnimation>2){
			shootAnimation=0;
			isShooting=false;
		}

		if(loops%2==0)
			bulletAnimation++;

		if(bulletAnimation>2){
			bulletAnimation=0;
		}
	}

	if(loops%2==0)
		explosion++;
	if(explosion>3)
		explosion=0;

	if(direction & RIGHT){
		if(framesLeft%4==0 && legsAnimationLeft<1){
			legsAnimationLeft++;
		}
		framesLeft++;
	}
	else if(direction & LEFT){
		if(framesRight%2==0 && legsAnimationRight<6){
			legsAnimationRight++;
		}
		framesRight++;
	}

	if(direction & RIGHT){
		switch(legsAnimationLeft){
		case 0:
			break;
		case 1:
			break;
		}
	}
	else if(direction & LEFT){
		switch(legsAnimationRight){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			if(loops%2==0)
				jetAnimation--;
			if(jetAnimation<0)
				jetAnimation=2;
			break;
		}
	}

	return true;
}


bool Player::render(float time){
	glSprite(playerX-132,playerY,GL2D_CENTER,&spr[3-explosion],false,0,0.75,0.75);

	if(direction & RIGHT){
		glSprite(playerX-77,playerY+0,GL2D_CENTER,&spr[17]);//costas 499
	}
	else if(direction & LEFT){
		glSprite(playerX-77,playerY+0,GL2D_CENTER,&spr[4]);//costas 500
	}else{
		glSprite(playerX-77,playerY+0,GL2D_CENTER,&spr[10]);//costas 498
	}

	if(direction & RIGHT){
		switch(legsAnimationLeft){
		case 0:
			glSprite(playerX-1,playerY+131,GL2D_CENTER,&spr[8]);//pernas 482
			break;
		case 1:
			glSprite(playerX-15,playerY+120,GL2D_CENTER,&spr[15]);//pernas 495
			break;
		}
	}
	else if(direction & LEFT){
		switch(legsAnimationRight){
		case 0:
			glSprite(playerX-1,playerY+131,GL2D_CENTER,&spr[8]);//pernas 482 [root]
			break;
		case 1:
			glSprite(playerX-1+2,playerY+131+1,GL2D_CENTER,&spr[6]);//pernas 484
			break;
		case 2:
			glSprite(playerX-1+3,playerY+131+0,GL2D_CENTER,&spr[7]);//pernas 485
			break;
		case 3:
			glSprite(playerX-1+2,playerY+131+0,GL2D_CENTER,&spr[9]);//pernas 486
			break;
		case 4:
			glSprite(playerX-1+27,playerY+131-10,GL2D_CENTER,&spr[5]);//pernas 487
			break;
		case 5:
			glSprite(playerX-1+45,playerY+131-7,GL2D_CENTER,&spr[11]);//pernas 488
			break;
		case 6:
			glSprite(playerX-1+130,playerY+131+80,GL2D_CENTER,&spr[jetAnimation+12]); //sprite 4920001

			glSprite(playerX-1+68,playerY+131-28,GL2D_CENTER,&spr[16]);//pernas 493				
			break;
		}
	}
	else{
		glSprite(playerX-1,playerY+131,GL2D_CENTER,&spr[8]);//pernas
	}

	glSprite(playerX-6,playerY-24,GL2D_CENTER,&spr[21]);//torso 502


	switch(shootAnimation){
	case 0:
		glSprite(playerX+34,playerY+20,GL2D_CENTER,&spr[19]);//arma 504
		break;
	case 1:
		glSprite(playerX+34,playerY+20,GL2D_CENTER,&spr[20]);//arma 505
		break;
	case 2:
		glSprite(playerX+34,playerY+20,GL2D_CENTER,&spr[18]);//arma 506
		break;
	}
	

	if(isShooting){
		if(powerUp==0){
			switch(bulletAnimation){
			case 0:				
				glSprite(playerX+114,playerY-38,0,&sprBullet[2]);
				break;
			case 1:				
				glSprite(playerX+114,playerY-38,0,&sprBullet[1]);
				break;
			case 2:				
				glSprite(playerX+114,playerY-28,0,&sprBullet[0]);
				break;
			}
		}
		else if(powerUp==1){
			switch(bulletAnimation){
			case 0:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[2],false,0,1,1,100,100,100,100,0,-127.0f/255.0f,-179.0f/255.0f);
				break;
			case 1:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[1],false,0,1,1,100,100,100,100,0,-127.0f/255.0f,-179.0f/255.0f);
				break;
			case 2:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[0],false,0,1,1,100,100,100,100,0,-127.0f/255.0f,-179.0f/255.0f);
				break;
			}
		}
		else if(powerUp==2){
			switch(bulletAnimation){
			case 0:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[2],false,0,1,1,100,100,100,100,-36.0f/255.0f,-189.0f/255.0f,-179.0f/255.0f);
				break;
			case 1:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[1],false,0,1,1,100,100,100,100,-36.0f/255.0f,-189.0f/255.0f,-179.0f/255.0f);
				break;
			case 2:
				glSprite(playerX+114,playerY-38,GL2D_COLOR_ADVANCED,&sprBullet[0],false,0,1,1,100,100,100,100,-36.0f/255.0f,-189.0f/255.0f,-179.0f/255.0f);
				break;
			}
		}
	}

	return true;
}

void Player::cleanUp(){
}

bool Player::collides(Vector2D<float> pos){
	return false;
}