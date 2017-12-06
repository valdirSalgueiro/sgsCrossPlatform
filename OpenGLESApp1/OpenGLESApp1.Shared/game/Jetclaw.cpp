#include "pch.h"
#include "Jetclaw.h"
#include "gmUtil.h"
#include "sound.h"

const unsigned int jetclaw_texcoords[] = {
0,0,493,239, // 0        jetclaw0007.png
495,0,493,239, // 1      jetclaw0005.png
495,0,493,239, // 2      jetclaw0006.png
0,241,493,239, // 3      jetclaw0004.png
495,241,493,239, // 4    jetclaw0003.png
0,482,493,239, // 5      jetclaw0002.png
0,723,493,239, // 6      jetclaw0001.png
};

extern std::vector<Object*> objs;

glImage sprJetclaw[7];
int jetclawAnimation=0;
extern sound* alienTiro1;


Jetclaw::Jetclaw(){
	std::string bulletPattern("<bulletml type=\"vertical\" xmlns=\"http://www.asahi-net.or.jp/~cs8k-cyu/bulletml\"><action label=\"top\"><fire><direction type=\"absolute\">0</direction><bulletRef label=\"curved\" /></fire><fire><direction type=\"absolute\">180</direction><bulletRef label=\"curved\" /></fire></action><bullet label=\"curved\"><action><changeDirection><direction type=\"absolute\">-90</direction><term>30</term></changeDirection><wait>30</wait><fire><bullet /></fire></action></bullet></bulletml>");

	isAlive=false;
	color=rand()%4;

	glLoadSpriteset("jetclaw.png",sprJetclaw,1024,1024,7,jetclaw_texcoords,0);	

	path.push_back(Vector2D<float>(1600+sprJetclaw[0].width,rand()%960));
	path.push_back(Vector2D<float>(800,rand()%960));
	path.push_back(Vector2D<float>(400,rand()%960));
	path.push_back(Vector2D<float>(200,-sprJetclaw[0].height));	
	
	bp = new BulletMLParserTinyXML(bulletPattern);
    bp->build();

}

Jetclaw::~Jetclaw(){
	delete bp;
}

bool Jetclaw::update(float time) {
	pos+=dir; 
	loops++; 


	if(loops%4==0)
		jetclawAnimation++;

	if(jetclawAnimation>6)
		jetclawAnimation=0;

	if(x>=1.0f){
		x=0.0f;
		oldPosition=pos;
		pathIter++;

		if(pathIter>=path.size()){
			if(life>0){
				pos=path.at(0);
				oldPosition=pos;
				pathIter=1;
			}else{
				isAlive=false;
			}
		}
			
	}else{
		if(color<2)
			pos=catmullRomSpline(x,oldPosition,oldPosition,path.at(pathIter),path.at(pathIter));		 
		else
			pos=catmullRomSpline(x,oldPosition,oldPosition,Vector2D<float>(path.at(pathIter).x,960-path.at(pathIter).y),Vector2D<float>(path.at(pathIter).x,960-path.at(pathIter).y));		 
	}


	if(loops%180==0){
		alienTiro1->play();
		objs.push_back(new Bullet(bp, pos.x, pos.y, 0, 0,COMMON));
	}

	x+=0.01f;
	life--;



	Aabb.Init( pos.x/2 - sprJetclaw[jetclawAnimation].width/4, pos.y/2 - sprJetclaw[jetclawAnimation].height/4,   sprJetclaw[jetclawAnimation].width/2, sprJetclaw[jetclawAnimation].height/2  );
	return true;
}


bool Jetclaw::render(float time){
	if(color==0){
		glSprite(pos.x,pos.y,GL2D_CENTER|GL2D_COLOR_ADVANCED,&sprJetclaw[jetclawAnimation],false,0,1,1,83,83,83,100,43.0f/255.0f);

	}
	else if(color==1){
		glSprite(pos.x,pos.y,GL2D_CENTER|GL2D_COLOR_ADVANCED,&sprJetclaw[jetclawAnimation],false,0,1,1,83,83,83,100,0,43.0f/255.0f);
	}
	else if(color==2){
		glSprite(pos.x,pos.y,GL2D_CENTER|GL2D_COLOR_ADVANCED,&sprJetclaw[jetclawAnimation],false,0,1,1,83,83,83,100,0,0,43.0f/255.0f);
	}
	else{
		glSprite(pos.x,pos.y,GL2D_CENTER,&sprJetclaw[jetclawAnimation]);
	}
	return true;
}


void Jetclaw::init(){
	loops=0;
	isAlive=true;
	pos=path.at(0);
	oldPosition=pos;
	pathIter=1;
	x=0;
	color=rand()%4;

	life=1000;
	energy=5;

	Aabb.Init( pos.x/2 - sprJetclaw[0].width/4, pos.y/2 - sprJetclaw[0].height/4,   sprJetclaw[0].width/2, sprJetclaw[0].height/2  );
}

void Jetclaw::cleanUp(){
}

bool Jetclaw::collides(Vector2D<float> pos,Vector2D<float> size){
	return false;
}