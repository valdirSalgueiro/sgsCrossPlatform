#include "pch.h"
#include "Boss2.h"
#include "gmUtil.h"
#include "sound.h"

int numframes;

const unsigned int boss2_texcoords[] = {
0,0,656,350, // 0        boss020024.png
658,0,656,349, // 1      boss020023.png
1316,0,653,350, // 2     boss020021.png
0,352,656,348, // 3      boss020001.png
0,702,654,349, // 4      boss020022.png
0,1053,656,347, // 5     boss020002.png
0,1402,655,347, // 6     boss020003.png
658,352,650,349, // 7    boss020012.png
1310,352,649,349, // 8   boss020013.png
658,703,649,348, // 9    boss020011.png
1309,703,649,348, // 10          boss020010.png
658,1053,645,350, // 11          boss020020.png
1305,1053,648,348, // 12         boss020014.png
658,1405,644,349, // 13          boss020019.png
1305,1403,643,349, // 14         boss020009.png
//2.png
};
const unsigned int boss2_sequence[] ={3,5,6,21,18,23,22,17,14,10,9,7,8,12,20,16,15,19,13,11,2,4,1,0};

const unsigned int boss2_texcoords_2[] = {
0,0,632,350, // 15       boss020017.png
634,0,631,350, // 16     boss020016.png
1267,0,643,349, // 17    boss020008.png
0,352,636,349, // 18     boss020005.png
0,703,633,349, // 19     boss020018.png
0,1054,632,349, // 20    boss020015.png
638,352,638,348, // 21   boss020004.png
0,1405,633,348, // 22    boss020007.png
1278,352,633,348, // 23          boss020006.png
};

extern std::vector<Object*> objs;

glImage sprBoss2[24];

int sign=1;

extern int powerUp;
extern int shield;
extern int score;
extern int bossMode;
extern int frames;

int olDenergyBoss2=0;
extern int powerUpTime;
extern sound* alienTiro1;


Boss2::Boss2(){
	std::string bulletPattern1("<?xml version=\"1.0\" ?><!DOCTYPE bulletml SYSTEM \"../bulletml.dtd\"><bulletml><action label=\"top\"><repeat><times>1</times><action><actionRef label=\"3way\"><param>0.3 + 1.1 * $rank</param></actionRef></action></repeat><wait>30</wait></action><action label=\"3way\"><fire><direction type=\"aim\">-45</direction><speed>$1</speed><bullet/></fire><fire><direction type=\"aim\">0</direction><speed>$1</speed><bullet/></fire><fire><direction type=\"aim\">45</direction><speed>$1</speed><bullet/></fire></action></bulletml>");
	std::string bulletPattern2("<?xml version=\"1.0\" ?><!DOCTYPE bulletml SYSTEM \"../bulletml.dtd\"><bulletml><action label=\"top\"><repeat><times>1 + 3 * $rank</times><action><fire><direction type=\"aim\">-20 + 40 * $rand</direction><bulletRef label=\"t-beam\"></bulletRef></fire><wait>60 - 20 * $rank</wait></action></repeat><wait>200</wait></action><bullet label=\"t-beam\"><speed>1.8</speed><action><repeat><times>999</times><action><fire><direction type=\"relative\">0</direction><bulletRef label=\"bit\"/></fire><wait>15 - 10 * $rank</wait></action></repeat></action></bullet><bullet label=\"bit\"><speed>0.0001</speed><action><wait>80</wait><fire><direction type=\"relative\">90</direction><bulletRef label=\"accel\"/></fire><fire><direction type=\"relative\">-90</direction><bulletRef label=\"accel\"/></fire><vanish/></action></bullet><bullet label=\"accel\"><speed>0.1 + 0.9 * $rank</speed><action><changeSpeed><speed>4.0</speed><term>300</term></changeSpeed></action></bullet></bulletml>");

	isAlive=false;

	numframes=sizeof(boss2_sequence)/sizeof(const unsigned int);

	glLoadSpriteset("boss2_1.png",sprBoss2,2048,2048,15,boss2_texcoords,0);
	glLoadSpriteset("boss2_2.png",sprBoss2,2048,2048,9,boss2_texcoords_2,0,15);

	path.push_back(Vector2D<float>(1600+sprBoss2[0].width,500));
	path.push_back(Vector2D<float>(1600,500));
	path.push_back(Vector2D<float>(1200,500));
	path.push_back(Vector2D<float>(1000,500));
	path.push_back(Vector2D<float>(800,500));

	

	bps[0] = new BulletMLParserTinyXML(bulletPattern1);
	bps[1] = new BulletMLParserTinyXML(bulletPattern2);
    bps[0]->build();
	bps[1]->build();    

	radius=sprBoss2[0].height/2;	
	
	Animation* animation=new Animation();
	animation->currentFrame=0;
	animations.push_back(animation);	

}

Boss2::~Boss2(){
	std::vector<Animation*>::iterator iterAnim = animations.begin();
	while (iterAnim != animations.end())
	{
		delete * iterAnim;  
		iterAnim = animations.erase(iterAnim);
	}
	delete bps[0];
	delete bps[1];
}


bool Boss2::update(float time) {
	Enemy::update(time);
	pos+=dir; 
	loops++; 

	if(x>=1.0f){
		x=0.0f;
		oldPosition=pos;
		pathIter+=pathIter*sign;

		if(pathIter>=path.size()){
			pathIter=path.size()-1;
			sign=-sign;
		}else if(pathIter<=1){
			pathIter=1;
			sign=-sign;
		}
			
	}else{
		pos=catmullRomSpline(x,oldPosition,oldPosition,path.at(pathIter),path.at(pathIter));		 
	}


	if(loops%90==0){
		alienTiro1->play();
		objs.push_back(new Bullet(bps[1], pos.x, pos.y, 0, 0,BOSS));
	}else if(loops%30==0){
		alienTiro1->play();
		objs.push_back(new Bullet(bps[0], pos.x, pos.y, 0, 0,BOSS));
	}

	if(energy%200==0){
		if(energy!=olDenergyBoss2 && powerUp<2){
			powerUpTime=frames;
			powerUp++;
			shield=100;
			olDenergyBoss2=energy;
		}
	}

	x+=0.01f;
	life--;

	std::vector<Animation*>::iterator iter = animations.begin();
	while (iter != animations.end())
	{
		if(loops%4==0){
			(*iter)->currentFrame++;
		}

		if((*iter)->currentFrame>numframes){
			(*iter)->currentFrame=0;
		}
		Aabb.Init( pos.x/2 - sprBoss2[boss2_sequence[(*iter)->currentFrame]].width/4, pos.y/2 - sprBoss2[boss2_sequence[(*iter)->currentFrame]].height/2,   sprBoss2[boss2_sequence[(*iter)->currentFrame]].width/2, sprBoss2[boss2_sequence[(*iter)->currentFrame]].height  );
		iter++;
	}

	return true;
}


bool Boss2::render(float time){
	std::vector<Animation*>::iterator iter = animations.begin();
	while (iter != animations.end())
	{
		if (wasHurt)
			glSprite(pos.x, pos.y, GL2D_CENTER, &sprBoss2[boss2_sequence[(*iter)->currentFrame]], false, 0, 2, 2, 100, 100, 100);
		else
			glSprite(pos.x,pos.y,GL2D_CENTER,&sprBoss2[boss2_sequence[(*iter)->currentFrame]],0,0,2,2);
		iter++;
	}
	return true;
}


void Boss2::init(){
	loops=0;
	isAlive=true;
	pos=path.at(0);
	oldPosition=pos;
	pathIter=1;
	x=0;

	life=1000;
	energy=999;

	Aabb.Init( pos.x/2 - sprBoss2[0].width/2, pos.y/2 - sprBoss2[0].height/2,   sprBoss2[0].width/2, sprBoss2[0].height  );
}

void Boss2::cleanUp(){
	shield=100;
	powerUp=0;
	bossMode=0;
}

bool Boss2::collides(Vector2D<float> pos,Vector2D<float> size){
	return false;
}