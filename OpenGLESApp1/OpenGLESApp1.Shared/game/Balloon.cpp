#include "pch.h"
#include "Balloon.h"
#include "gmUtil.h"
//#include "../sound.h"

const unsigned int balloon_texcoords[] = {
0,0,162,448, // 0        balloon0006.png
164,0,143,445, // 1      balloon0005.png
309,0,140,444, // 2      balloon0019.png
451,0,143,442, // 3      balloon0004.png
596,0,140,441, // 4      balloon0003.png
738,0,136,441, // 5      balloon0020.png
876,0,140,439, // 6      balloon0002.png
0,450,136,437, // 7      balloon0001.png
138,450,162,435, // 8    balloon0007.png
302,450,180,422, // 9    balloon0009.png
484,450,162,422, // 10   balloon0008.png
648,450,181,420, // 11   balloon0012.png
831,450,175,420, // 12   balloon0013.png
0,889,180,417, // 13     balloon0011.png
0,1308,180,417, // 14    balloon0010.png
182,889,171,414, // 15   balloon0018.png
355,889,171,414, // 16   balloon0017.png
528,889,209,379, // 17   balloon0016.png
739,889,209,379, // 18   balloon0015.png
182,1305,294,337, // 19          balloon0014.png
};

extern std::vector<Object*> objs;

glImage sprBalloon[20];
int BalloonAnimation=0;
int signBaloon=1;
int olDenergy=0;

extern int powerUpTime;
extern int frames;
extern int powerUp;
extern int shield;
extern int score;
extern int bossMode;

//extern sound* alienTiro1;



Balloon::Balloon(){
	std::string bulletPattern1("<?xml version=\"1.0\" ?><!DOCTYPE bulletml SYSTEM \"../bulletml.dtd\"><bulletml><action label=\"top\"><fire><direction type=\"aim\">20</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"2way-1\"><param>1</param></bulletRef></fire><fire><direction type=\"aim\">-20</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"2way-1\"><param>-1</param></bulletRef></fire><wait>120</wait></action><bullet label=\"2way-1\"><action><wait>30 - 15 * $rank</wait><fire><direction type=\"aim\">0</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"2way-2\"><param>$1</param></bulletRef></fire><fire><direction type=\"aim\">30 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"2way-2\"><param>$1</param></bulletRef></fire><vanish/></action></bullet><bullet label=\"2way-2\"><action><wait>30 - 15 * $rank</wait><fire><direction type=\"aim\">-10 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"3way\"><param>$1</param></bulletRef></fire><fire><direction type=\"aim\">10 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"3way\"><param>$1</param></bulletRef></fire><fire><direction type=\"aim\">30 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bulletRef label=\"3way\"><param>$1</param></bulletRef></fire><vanish/></action></bullet><bullet label=\"3way\"><action><wait>30 - 15 * $rank</wait><fire><direction type=\"aim\">0</direction><speed>0.5 + 1.1 * $rank</speed><bullet/></fire><fire><direction type=\"relative\">70 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bullet/></fire><repeat><times>4</times><action><fire><direction type=\"sequence\">-30 * $1</direction><speed>0.5 + 1.1 * $rank</speed><bullet/></fire></action></repeat><vanish/></action></bullet></bulletml>");
	std::string bulletPattern2("<?xml version=\"1.0\" ?><!DOCTYPE bulletml SYSTEM \"../bulletml.dtd\"><bulletml><action label=\"top\"><fire><bulletRef label=\"bit\"><param>3</param><param>1.1</param></bulletRef></fire><fire><bulletRef label=\"bit\"><param>-3</param><param>1.1</param></bulletRef></fire><wait>10</wait><fire><bulletRef label=\"bit\"><param>3.5</param><param>1.15</param></bulletRef></fire><fire><bulletRef label=\"bit\"><param>-3.5</param><param>1.15</param></bulletRef></fire><wait>350 - 100 * $rank</wait></action><bullet label=\"bit\"><direction type=\"absolute\">0</direction><speed>0.0</speed><action><fire><direction type=\"absolute\">0</direction><speed>$2 * (0.5 + 0.5 * $rank)</speed><bullet/></fire><wait>2 * (3.5 - 2.5 * $rank)</wait><repeat><times>100 / (3.5 - 2.5 * $rank)</times><action><fire><direction type=\"sequence\">$1 * (3.5 - 2.5 * $rank)</direction><speed>$2 * (0.5 + 0.5 * $rank)</speed><bullet/></fire><wait>2 * (3.5 - 2.5 * $rank)</wait></action></repeat><vanish/></action></bullet></bulletml>");

	isAlive=false;

	glLoadSpriteset("balloon.png",sprBalloon,1024,2048,20,balloon_texcoords,0);

	path.push_back(Vector2D<float>(1600+sprBalloon[0].width,500));
	path.push_back(Vector2D<float>(1600,500));
	path.push_back(Vector2D<float>(1200,500));
	path.push_back(Vector2D<float>(1000,500));
	path.push_back(Vector2D<float>(800,500));

	bps[0] = new BulletMLParserTinyXML(bulletPattern1);
	bps[1] = new BulletMLParserTinyXML(bulletPattern2);
    bps[0]->build();
	bps[1]->build();

	radius=sprBalloon[0].height/2;	

}

Balloon::~Balloon(){
	delete bps[0];
	delete bps[1];
}



bool Balloon::update(float time) {
	pos+=dir; 
	loops++; 

	if(loops%5==0)
		BalloonAnimation++;

	if(BalloonAnimation>19)
		BalloonAnimation=0;

	if(x>=1.0f){
		x=0.0f;
		oldPosition=pos;
		pathIter+=pathIter*signBaloon;

		if(pathIter>=path.size()){
			pathIter=path.size()-1;
			signBaloon=-signBaloon;
		}else if(pathIter<=1){
			pathIter=1;
			signBaloon=-signBaloon;
		}
			
	}else{
		pos=catmullRomSpline(x,oldPosition,oldPosition,path.at(pathIter),path.at(pathIter));		 
	}
	
	if(loops%500==0){
#ifdef WIN32
		//alienTiro1->play();
#endif
		objs.push_back(new Bullet(bps[1], pos.x, pos.y, 0, 0,BOSS));
	}else if(loops%180==0){
		#ifdef WIN32
		//alienTiro1->play();
#endif
		objs.push_back(new Bullet(bps[0], pos.x, pos.y, 0, 0,BOSS));
	}


	x+=0.01f;
	life--;

	if(energy%150==0){
		if(energy!=olDenergy && powerUp<2){
			powerUpTime=frames;
			powerUp++;
			shield=100;
			olDenergy=energy;
		}
	}



	Aabb.Init( pos.x/2 - sprBalloon[BalloonAnimation].width/2, pos.y/2 - sprBalloon[BalloonAnimation].height/2,   sprBalloon[BalloonAnimation].width, sprBalloon[BalloonAnimation].height  );
	return true;
}


bool Balloon::render(float time){
	glSprite(pos.x,pos.y,GL2D_CENTER,&sprBalloon[BalloonAnimation],false,0,2,2);
	return true;
}


void Balloon::init(){
	loops=0;
	isAlive=true;
	pos=path.at(0);
	oldPosition=pos;
	pathIter=1;
	x=0;

	life=1000;
	energy=399;

	Aabb.Init( pos.x/2 - sprBalloon[BalloonAnimation].width/2, pos.y/2 - sprBalloon[BalloonAnimation].height/2,   sprBalloon[BalloonAnimation].width, sprBalloon[BalloonAnimation].height  );
}

void Balloon::cleanUp(){
	shield=100;
	powerUp=0;
	bossMode=0;
}

bool Balloon::collides(Vector2D<float> pos,Vector2D<float> size){
	return false;
}