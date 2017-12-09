#include "pch.h"
#include "Engine.h"
#include "gl2d.h"
#include "gmUtil.h"
#include "Aabb.h"

#include <time.h>
//#include "VideoGLSurfaceView.h"


#include "sound.h"

#ifdef USEFONT
#include "font/FTBitmapChar.h"
#endif


#include "Jetclaw.h"
#include "Hellburguer.h"
#include "Balloon.h"
#include "Boss2.h"

#include "Bullet_.h"

#include <vector>
#include "Enemy.h"
#include "Animation.h"

#include "Max.h"
#include "Tammy.h"

#include "Intro.h"

#include "Tween.h"
#include "Vector2D.h"


const char* szLetters = " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_аимясз";

extern glImage sprBullet[4];

bool touch;

glImage sprTiro[3];
glImage sprExplosao[14];
glImage sprJetclawExplosao[9];


glImage* fundo;
glImage* lua;
glImage* nuvem1;
glImage* nuvem2;
glImage* nuvem3;
glImage* nuvem4;
glImage* montanha;
glImage* painel;

glImage* purpurinanave;
glImage* naveColonizadora;

extern int legsAnimationLeft;
extern int legsAnimationRight;
extern int framesLeft;
extern int framesRight;

extern int powerUp;

int shield;
int score;

const unsigned int tiro_texcoords[] = {
	0,0,17,16, // 0          shape 9
	19,0,17,16, // 1         shape 10
	38,0,15,14, // 2         shape 8
};

const unsigned int explosao_texcoords[] = {
	0,0,523,426, // 0        7.png
	0,0,523,426, // 1        8.png
	525,0,523,426, // 2      5.png
	525,0,523,426, // 3      6.png
	1050,0,523,426, // 4     3.png
	1050,0,523,426, // 5     4.png
	0,428,523,426, // 6      13.png
	0,428,523,426, // 7      14.png
	0,856,523,426, // 8      11.png
	0,856,523,426, // 9      12.png
	0,1284,523,426, // 10    10.png
	0,1284,523,426, // 11    9.png
	525,428,523,426, // 12   01.png
	525,428,523,426, // 13   2.png
};

const unsigned int explosion_sequence[] = { 12,13,4,5,2,3,0,1,11,10,8,9,6,7 };

const unsigned int explosao_texcoords2[] = {
	0,0,214,239, //0        17.png
	0,241,126,147, //1      16.png
	0,390,96,92, //2        15.png
	128,241,86,70, //3      13.png
}; glImage sprExplosao2[4];
const unsigned int explosion_sequence2[] = { 3,2,1,0 };



const unsigned int jetclawExplosion_texcoords[] = {
	0,0,1287,461, // 0       384.png
	0,463,1169,452, // 1     383.png
	0,917,890,424, // 2      382.png
	1289,0,455,328, // 3     381.png
	1171,463,412,320, // 4   380.png
	1585,463,356,272, // 5   378.png
	1746,0,222,233, // 6     377.png
	1746,235,136,180, // 7   376.png
	1884,235,115,141, // 8   375.png
};

const unsigned int jetclawExplosion_sequence[] = { 8,7,6,5,4,3,2,1,0 };

const unsigned int level1_texcoords[] = {
	0,0,1920,216, //0       image 99
	0,218,1024,470, //1     image 191
	0,690,984,500, //2      shape 98
	1026,218,929,213, //3   shape 558
	0,1192,731,542, //4     shape 114
	1026,433,673,182, //5   painel
	986,690,601,395, //6    purpurinanave
	986,1087,473,471, //7   shape 103
	1701,433,304,81, //8    shape 108
	1026,617,138,28, //9    shape 111
	1701,516,128,128, //10  onscreencontrolbase
	1922,0,52,52, //11      onscreencontrolknob
	1976,0,4,4, //12        point
};
glImage level1[13];


extern float playerX;
extern float playerY;


int framesShoot;

int direction;

int nuvemX;
int nuvem2X;
int nuvem3X;
int nuvem4X;

Vector2D<float> nave[5];

int montanhaX;
int montanha2X;

int turn = 0;

#include "../bulletML/object.h"
std::vector<Object*> objs;
std::vector<Object*> playerObjs;

int frames;

glImage* point;

float elapsedTime = 0.0;

void addObj(Object* object) {
	objs.push_back(object);
}

void addPlayerObj(Object* object) {
	playerObjs.push_back(object);
}


std::vector<Animation*> animations;

Enemy* enemy;
Max* max_;
Tammy* tammy;
Intro* intro;

int powerUpTime;
int bossTime;

int enemieKills;
int enemieIter;
int bossMode;
Tween myTween;

#include "ahb.h"
#include "digit.h"

#ifdef USEFONT
FontAtlas* m_pFontAtlas;
#endif
int widths[10];


int jetclawSize = 0;
int hellburguerSize = 0;
int baloonSize = 0;
int boss2Size = 0;

int boss = 0;
bool naveSoundPlayed;

sound* explosionSound;
sound* aliendie;
sound* warning;
sound* naveSound;
sound* alien1;
sound* alienTiro1;
music* levelMusic;
bool warningPlayed = false;

bool shakeScreen = false;
float shakeAmount = 0.1;

void Engine::init(int width_, int height_) {
	width = width_;
	height = height_;
	startBatch(width, height);
	srand(time(NULL));

	//deinit();

#ifdef USEFONT
	m_pFontAtlas = new FontAtlas();
	m_pFontAtlas->AddFont(ahb_ttf, ahb_ttf_size, 44, szLetters);
	m_pFontAtlas->AddFont(ahb_ttf, ahb_ttf_size, 36, szLetters);
	m_pFontAtlas->AddFont(ahb_ttf, ahb_ttf_size, 20, szLetters);
	m_pFontAtlas->AddFont(digit_ttf, digit_ttf_size, 58, szLetters);
	m_pFontAtlas->CreateAtlas();

	widths[0] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("OK, MAX! YOUR FIRST MISSION IS TO BREAKTRHOUGH");
	widths[1] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("THE ENEMY FRONTLINE! THE ENEMY FORCES ARE");
	widths[2] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("COMPOSED OF BIOMECHANICHAL LIFEFORMS,");
	widths[3] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("UNKNOWN TO US!");
	widths[4] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("I'LL SEE YOU BEHIND ENEMY LINES!");
	widths[5] = m_pFontAtlas->GetFont(AHB_20)->GetWidth("GOOD LUCK!");
	widths[6] = m_pFontAtlas->GetFont(AHB_44)->GetWidth("MISSION START!");
#endif

	reset();

	glLoadSpriteset("tiro.png", sprTiro, 128, 128, 3, tiro_texcoords, 0);
	glLoadSpriteset("explosao.png", sprExplosao, 2048, 2048, 14, explosao_texcoords, 0);
	glLoadSpriteset("explosao2.png", sprExplosao2, 256, 512, 4, explosao_texcoords2, 0);
	glLoadSpriteset("jetclawExplosion.png", sprJetclawExplosao, 2048, 2048, 9, jetclawExplosion_texcoords, 0);

	controlBasePos.x = 100;
	controlBasePos.y = 380;
	controlKnobPos = controlBasePos;

	direction = STOPPED;

	wcsound_start();
	explosionSound = new sound("explosion.wav", 25, 100, 50);
	aliendie = new sound("aliendie1.wav", 25, 100, 50);
	warning = new sound("warning.wav", 100, 100, 50);
	alien1 = new sound("alien1.wav", 100, 100, 50);
	naveSound = new sound("ship.wav", 100, 100, 50);
	alienTiro1 = new sound("alienTiro1.wav", 100, 100, 50);
	levelMusic = new music("level1.mp3", 60, 50);

	glLoadSpriteset("level1.png", level1, 2048, 2048, 13, level1_texcoords, 0);

	montanha = &level1[0]; //99
	nuvem3 = &level1[1];//191
	fundo = &level1[2];//98
	nuvem1 = &level1[3];//558
	naveColonizadora = &level1[4];//114
	painel = &level1[5];//
	purpurinanave = &level1[6];//
	lua = &level1[7];//103
	nuvem2 = &level1[8];//108
	nuvem4 = &level1[9];//111
	controlBase = &level1[10];//
	controlKnob = &level1[11];//
	point = &level1[12];//

	//gameState=LOGO1;
	gameState=LOGO2;
	//gameState = PLAYING;


	enemies.push_back(new Jetclaw());
	enemies.push_back(new Jetclaw());
	enemies.push_back(new Jetclaw());
	enemies.push_back(new Jetclaw());
	jetclawSize = enemies.size();
	enemies.push_back(new Hellburguer());
	enemies.push_back(new Hellburguer());
	enemies.push_back(new Hellburguer());
	enemies.push_back(new Hellburguer());
	hellburguerSize = enemies.size();
	enemies.push_back(new Balloon());
	baloonSize = enemies.size();
	enemies.push_back(new Boss2());
	boss2Size = enemies.size();

	player = new Player();

	max_ = new Max();
	tammy = new Tammy();
	intro = new Intro();

	if (gameState == LOGO1) {
#ifdef WIN32
		//native_init("../res/SGSMovie.avi");
#else
		videoPath = "/sdcard/SGSMovie.avi";
		//native_init(videoPath);
#endif
	}
}

Engine::~Engine() {
	deinit();
}

void Engine::reset() {
	player = NULL;
	frames = 0;

	shownScore = false;
	started = false;

	//
	framesShoot = 0;
	direction = 0;
	nuvemX = 1600;
	nuvem2X = 2400;
	nuvem3X = 2800;
	nuvem4X = 3000;

	score = 0;
	shield = 100;

	nave[0].x = -3000;
	nave[1].x = -3000;
	nave[2].x = -1500;
	nave[3].x = -640;
	nave[4].x = 0;

	montanhaX = 0;
	montanha2X = 1920;

	turn = 0;

	powerUpTime = 0;
	bossTime = 0;

	enemieKills = 0;
	enemieIter = 0;
	bossMode = 0;


	touch = false;


}

void Engine::deinit() {
	//destroy_();
#ifdef USEFONT
	delete m_pFontAtlas;
#endif
	delete player;


	delete enemy;
	delete max_;
	delete tammy;

	std::vector<Enemy*>::iterator iter = enemies.begin();
	while (iter != enemies.end())
	{
		delete * iter;
		iter = enemies.erase(iter);
	}

	std::vector<Object*>::iterator iterObj = objs.begin();
	while (iterObj != objs.end())
	{
		delete* iterObj;
		iterObj = objs.erase(iterObj);
	}

	iterObj = playerObjs.begin();
	while (iterObj != playerObjs.end())
	{
		delete* iterObj;
		iterObj = playerObjs.erase(iterObj);
	}

	std::vector<Animation*>::iterator iterAnim = animations.begin();
	while (iterAnim != animations.end())
	{
		delete * iterAnim;
		iterAnim = animations.erase(iterAnim);
	}

}

void Engine::update(float time) {
	wcsound_update();
	elapsedTime += time;

	if (shakeScreen && shakeAmount > 0.01) {
		shakeAmount *= 0.9f;
	}
	else {
		shakeAmount = 0.1f;
		shakeScreen = false;
	}

	if (gameState == LOGO1) {
		if (touch) {
			gameState = LOGO2;
			//destroy_();
			touch = false;
		}
	}

	else if (gameState == LOGO2) {
		intro->update(time);
		if (touch) {
			wcsound_music_play(levelMusic);
			gameState = PLAYING;
			touch = false;
		}
	}
	else {
		if (gameState == PLAYING || gameState == GAMEOVER)
			playing(time);

		if (!started) {
			if (frames > 10 * 60)
				started = true;

			max_->update(time);
			tammy->update(time);

		}
		else {

			if (!naveSoundPlayed && frames > 1900) { //nave some em 5800
				naveSound->play();
				naveSoundPlayed = true;
			}

			if (started && frames % (120 + rand() % 300) == 0)
				shakeScreen = true;

			if (frames % 10 == 0) {
				Animation* animation = new Animation();
				animation->currentFrame = 0;
				animation->x = nave[0].x + rand() % naveColonizadora->width*0.8f;
				animation->y = 40 + rand() % naveColonizadora->height*0.8f;
				animation->type = Animation::Explosion2;
				animations.push_back(animation);

				animation = new Animation();
				animation->currentFrame = 0;
				animation->x = nave[0].x + rand() % naveColonizadora->width*0.8f;
				animation->y = 40 + rand() % naveColonizadora->height*0.8f;
				animation->type = Animation::Explosion2;
				animations.push_back(animation);
			}

			//inimigos
			for (int i = 0; i < enemies.size(); ++i) {
				if (enemies[i]->isAlive) {
					enemies[i]->update(time);
					for (size_t j = 0; j < playerObjs.size(); j++) {
						AABB aabb;
						aabb.Init(playerObjs[j]->x / 2 - sprBullet[3].width / 2, playerObjs[j]->y / 2 - sprBullet[3].height / 2, sprBullet[3].width, sprBullet[3].height);
						if (enemies[i]->Aabb.Intersects(aabb)) {
							if (enemies[i]->energy > 0) {
								score++;
								enemies[i]->hurt();
								Animation* animation = new Animation();
								animation->currentFrame = 0;
								animation->x = playerObjs[j]->x + sprBullet[3].width;
								animation->y = playerObjs[j]->y;
								animation->type = Animation::ExplosionAlien;

								explosionSound->play();

								animations.push_back(animation);
							}
							else {
								Animation* animation = new Animation();
								animation->currentFrame = 0;
								animation->x = playerObjs[j]->x + sprBullet[3].width;
								animation->y = playerObjs[j]->y;
								animation->type = Animation::JetclawExplosion;
								animations.push_back(animation);

								aliendie->play();

								enemies[i]->cleanUp();
								enemies[i]->isAlive = false;
								score += 100;
								enemieKills++;
							}
							delete playerObjs[j];
							playerObjs.erase(playerObjs.begin() + j);
							continue;
						}
					}
				}
			}
			if (enemieKills >= 200) {
				if (!bossMode) {
					bossTime = frames;
					boss++;
					bossMode = 1;
					shield = 100;
				}
				else {
					if (bossMode != 2) {
						if (frames - bossTime > 300) {
							if (boss == 1) {

								enemies[baloonSize - 1]->init();
							}
							else {

								enemies[boss2Size - 1]->init();
							}
							bossMode = 2;
							enemieKills = 0;
						}
					}
				}
			}
			else if (enemieKills > 150) {
				powerUp = 0;
			}
			else if (enemieKills > 100) {
				if (powerUp == 1)
					powerUpTime = frames;
				powerUp = 2;

			}
			else if (enemieKills > 50) {
				if (powerUp == 0)
					powerUpTime = frames;
				powerUp = 1;
			}



			int r = 0;
			if (frames % 30 == 0) {
				if (enemieKills < 400 && bossMode == 0) {
					int enemyTypeSize = 0;
					int lowerbound = 0;
					r = (rand() % 100);
					if (r < 40) {
						enemyTypeSize = jetclawSize;
					}
					else if (r < 80) {
						lowerbound = jetclawSize;
						enemyTypeSize = hellburguerSize;
					}
					else {
						lowerbound = hellburguerSize;
						//enemyTypeSize=baloonSize;
					}
					for (enemieIter = lowerbound; enemieIter < enemyTypeSize; enemieIter++) {
						if (!enemies[enemieIter]->isAlive) {
							enemies[enemieIter]->init();
							break;
						}
					}
				}
			}
		}

		player->update(time);

		//new update
		if (frames % 8 == 0) {
			montanhaX--;
			montanha2X--;
		}

		if (montanhaX < -1918) {
			montanhaX = 1920;
		}
		if (montanha2X < -1918) {
			montanha2X = 1920;
		}

		nuvemX -= 8;
		if (nuvemX < -1858 * 2) {
			nuvemX = 1600;
		}

		nuvem2X -= 4;
		if (nuvem2X < -2048 * 2) {
			nuvem2X = 1600;
		}

		nuvem3X -= 2;
		if (nuvem3X < -608 * 2) {
			nuvem3X = 1600;
		}

		nuvem4X -= 1;
		if (nuvem4X < -276 * 2) {
			nuvem4X = 1600;
		}

		nave[4].x += 0.2f;
		nave[3].x += 0.2f;
		nave[2].x += 0.2f;
		nave[1].x += 0.2f;
		nave[0].x += 0.4f;

		for (size_t i = 0; i < playerObjs.size(); i++) {
			if (!playerObjs[i]->move()) {
				delete playerObjs[i];
				playerObjs.erase(playerObjs.begin() + i);
				continue;
			}
		}

		for (size_t i = 0; i < objs.size(); i++) {
			if (!objs[i]->move()) {
				delete objs[i];
				objs.erase(objs.begin() + i);
				continue;
			}


			if (circlesColliding(objs[i]->x, objs[i]->y, sprTiro[objs[i]->anim].width / 2.0f, playerX, playerY, 150)) {
				Animation* animation = new Animation();
				animation->currentFrame = 0;
				animation->x = objs[i]->x;
				animation->y = objs[i]->y;
				shield -= 1;

				explosionSound->play();
#ifdef WIN32
				animations.push_back(animation);
#else
				if (animations.size() > 3) {
					animations.erase(animations.begin());
				}
				animations.push_back(animation);
#endif
				delete objs[i];
				objs.erase(objs.begin() + i);
			}
		}

		std::vector<Animation*>::iterator iter = animations.begin();
		while (iter != animations.end())
		{
			if ((*iter)->type == Animation::Explosion2) {
				if (frames % 10 == 0) {
					(*iter)->currentFrame++;
				}
			}
			else {
				if (frames % 2 == 0) {
					(*iter)->currentFrame++;
				}
			}

			int numframes;

			if ((*iter)->type == Animation::Explosion || ((*iter)->type == Animation::ExplosionAlien)) {
				numframes = sizeof(explosion_sequence) / sizeof(const unsigned int);
				numframes -= 1;
			}
			else if ((*iter)->type == Animation::Explosion2) {
				numframes = sizeof(explosion_sequence2) / sizeof(const unsigned int);
				numframes -= 1;
			}
			else if ((*iter)->type == Animation::JetclawExplosion) {
				numframes = sizeof(jetclawExplosion_sequence) / sizeof(const unsigned int);
				numframes -= 1;
			}

			if ((*iter)->currentFrame > numframes) {
				delete * iter;
				iter = animations.erase(iter);
				continue;
			}
			iter++;
		}
		if (direction & LEFT) {
			if (playerX > 0)
				playerX -= 10;
		}

		if (direction & RIGHT) {
			if (playerX < 1600)
				playerX += 10;
		}

		if (direction & DOWN) {
			if (playerY < 960)
				playerY += 10;
		}

		if (direction & UP) {
			if (playerY > 0)
				playerY -= 10;
		}

		frames++;
		turn++;
	}
}

void Engine::render(float time) {

	if (gameState == LOGO1) {
		spriteBatchBegin();
		//if(!native_gl_render(time)){
		//	gameState=LOGO2;
		//	destroy_();
		//}
		spriteBatchEnd();
	}
	else if (gameState == LOGO2) {
		spriteBatchBegin();
		intro->render(time);
		spriteBatchEnd();
	}
	else {
		spriteBatchBegin();

		glSprite(-50, 0, GL2D_BACKGROUND | GL2D_SCALE_TO_SCREEN, fundo);

		glSprite(1340, 200, GL2D_CENTER | GL2D_NO_SCALE, lua, 0, 0, 1, 1);

		glSprite(montanhaX, 320, GL2D_NO_SCALE, montanha, 0, 0, 1, 1, 1, 1, 1, 0.3f);
		glSprite(montanha2X, 320, GL2D_NO_SCALE, montanha, 0, 0, 1, 1, 1, 1, 1, 0.3f);

		glSprite(nave[4].x, 0, GL2D_FLIP_H | GL2D_NO_SCALE | GL2D_COLOR_ADVANCED, naveColonizadora, false, 0, 0.4, 0.4, 100.0f, 100.0f, 100.0f, 58.0f, 106.0f / 255.0f, 129.0f / 255.0f, 255.0f / 255.0f, 0);
		glSprite(nave[3].x, 40, GL2D_FLIP_H | GL2D_NO_SCALE | GL2D_COLOR_ADVANCED, naveColonizadora, false, 0, 0.8, 0.8, 100.0f, 100.0f, 100.0f, 89.0f, 106.0f / 255.0f, 129.0f / 255.0f, 255.0f / 255.0f, 0);
		glSprite(nave[2].x, 80, GL2D_FLIP_H | GL2D_NO_SCALE | GL2D_COLOR_ADVANCED, naveColonizadora, false, 0, 1.2, 1.2, 100.0f, 100.0f, 100.0f, 76.0f, 120.0f / 255.0f, 111.0f / 255.0f, 255.0f / 255.0f, 0);
		glSprite(nave[1].x, 160, GL2D_FLIP_H | GL2D_NO_SCALE | GL2D_COLOR_ADVANCED, naveColonizadora, false, 0, 1.6, 1.6, 100.0f, 100.0f, 100.0f, 100.0f, 97.0f / 255.0f, 78.0f / 255.0f, 255.0f / 255.0f, 0);
		glSprite(nave[0].x, 80, GL2D_FLIP_H | GL2D_NO_SCALE | GL2D_COLOR_ADVANCED, naveColonizadora, false, 0, 2, 2, 100.0f, 100.0f, 100.0f, 100.0f, 227.0f / 255.0f, 106.0f / 255.0f, 83.0f / 255.0f, 0);
		//glSprite(nave[0].x+25,nave[0].y+65,GL2D_FLIP_H|GL2D_NO_SCALE|GL2D_COLOR_ADVANCED,purpurinanave,false,0,1,1,100.0f,100.0f,100.0f,100.0f,227.0f/255.0f,106.0f/255.0f,83.0f/255.0f,0);	



		glSprite(nuvem4X, 480, GL2D_NO_SCALE, nuvem4, false, 0, 2, 2, 1, 1, 1, 1);

		glSprite(nuvem3X, 480, GL2D_NO_SCALE, nuvem2, false, 0, 2, 2, 1, 1, 1, 1);

		glSprite(nuvem2X, 480, GL2D_NO_SCALE, nuvem3, false, 0, 1, 1, 1, 1, 1, 1);

		//inimigos
		for (int i = 0; i < enemies.size(); ++i) {
			if (enemies[i]->isAlive)
				enemies[i]->render(time);
		}

		//printf("%d\n",objs.size());
		for (size_t i = 0; i < objs.size(); i++) {
			if (!objs[i]->draw()) {
				delete objs[i];
				objs.erase(objs.begin() + i);
				continue;
			}
		}

		for (size_t i = 0; i < playerObjs.size(); i++) {
			if (!playerObjs[i]->draw()) {
				delete playerObjs[i];
				playerObjs.erase(playerObjs.begin() + i);
				continue;
			}
		}

		player->render(time);


		std::vector<Animation*>::iterator iter = animations.begin();
		while (iter != animations.end())
		{
			if ((*iter)->type == Animation::Explosion) {
				glSprite((*iter)->x, (*iter)->y, GL2D_CENTER, &sprExplosao[explosion_sequence[(*iter)->currentFrame]]);
			}
			else if ((*iter)->type == Animation::ExplosionAlien) {
				glSprite((*iter)->x, (*iter)->y, GL2D_CENTER | GL2D_COLOR_ADVANCED, &sprExplosao[explosion_sequence[(*iter)->currentFrame]], false, 0, 1, 1, 100, 100, 100, 100, -1, 51 / 255.0f, 0, 0);
			}
			else if ((*iter)->type == Animation::JetclawExplosion) {
				glSprite((*iter)->x, (*iter)->y, GL2D_CENTER, &sprJetclawExplosao[jetclawExplosion_sequence[(*iter)->currentFrame]]);
			}
			else if ((*iter)->type == Animation::Explosion2) {
				glSprite((*iter)->x, (*iter)->y, GL2D_CENTER | GL2D_COLOR_ADVANCED | GL2D_NO_SCALE, &sprExplosao2[explosion_sequence2[(*iter)->currentFrame]], false, 0, 0.8, 0.8, 100.0f, 100.0f, 100.0f, 100.0f, 97.0f / 255.0f, 78.0f / 255.0f, 255.0f / 255.0f, 0);
				//glSprite((*iter)->x,(*iter)->y,GL2D_CENTER|GL2D_NO_SCALE,&sprExplosao2[explosion_sequence2[(*iter)->currentFrame]]);
			}
			iter++;
		}

		glSprite(nuvemX, 534, GL2D_NO_SCALE, nuvem1, 0, 0, 2.0f, 2.0f);

		//glSprite(playerX,playerY+80,GL2D_CENTER,point,0,0,75,75);

		//HUD

		std::stringstream st;
		st << score;
		std::string scoreStr = st.str();

#ifdef USEFONT
		m_pFontAtlas->GetFont(AHB_44)->DrawStringShadow(100, 25, "SCORE", 0xffffff, 0x000000);
		m_pFontAtlas->GetFont(DIGIT_X)->DrawStringShadow(220, -5, scoreStr, 0x0000ff, 0xffffff);


		st.str("");
		st << shield;
		std::string shieldStr = st.str();
		m_pFontAtlas->GetFont(AHB_44)->DrawStringShadow(470, 25, "SHIELD", 0xffffff, 0x000000);
		m_pFontAtlas->GetFont(DIGIT_X)->DrawStringShadow(600, -5, shieldStr, 0x0000ff, 0xffffff);

		if (powerUp != 0 && frames - powerUpTime < 300) {
			if (frames % 30 < 15) {

			}
			else {
				m_pFontAtlas->GetFont(AHB_44)->DrawStringShadow(640, 480, "POWER UP!", 0xffffff, 0x000000);
			}
		}


		if (boss != 0 && frames - bossTime < 300) {
			if (frames - bossTime == 200) {
				alien1->play();
			}
			if (frames % 30 < 15) {
				warningPlayed = false;
			}
			else {
				if (!warningPlayed) {
					warning->play();
					warningPlayed = true;
				}
				m_pFontAtlas->GetFont(AHB_44)->DrawStringShadow(640, 480, "WARNING!", 0xff0000, 0x000000);
			}
		}

		if (!started) {
			glSprite(870, 800, GL2D_CENTER | GL2D_NO_SCALE, painel, false, 0, 1.4f, 1.4f);

			max_->render(time);
			tammy->render(time);

			if (frames % 30 < 15) {
				warningPlayed = false;
			}
			else {
				if (!warningPlayed) {
					warning->play();
					warningPlayed = true;
				}
				m_pFontAtlas->GetFont(AHB_44)->DrawStringShadow(800 - widths[6] / 2, 480, "MISSION START!", 0xffffff, 0x000000);
			}

			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[0] / 2, 680, "OK, MAX! YOUR FIRST MISSION IS TO BREAKTRHOUGH", 0xffffff, 0x000000);
			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[1] / 2, 720, "THE ENEMY FRONTLINE! THE ENEMY FORCES ARE", 0xffffff, 0x000000);
			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[2] / 2, 760, "COMPOSED OF BIOMECHANICHAL LIFEFORMS,", 0xffffff, 0x000000);
			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[3] / 2, 800, "UNKNOWN TO US!", 0xffffff, 0x000000);
			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[4] / 2, 840, "I'LL SEE YOU BEHIND ENEMY LINES!", 0xffffff, 0x000000);
			m_pFontAtlas->GetFont(AHB_20)->DrawStringShadow(840 - widths[5] / 2, 890, "GOOD LUCK!", 0xffff00, 0x000000);
		}
#endif

#ifdef __APPLE__ || __ANDROID_API__
		glSprite(controlBasePos.x, controlBasePos.y, GL2D_NO_SCALE | GL2D_CENTER, controlBase, false, 0, 1, 1, 1.0f, 1.0f, 1.0f, 0.5f);
		glSprite(controlKnobPos.x, controlKnobPos.y, GL2D_NO_SCALE | GL2D_CENTER, controlKnob, false, 0, 1, 1, 1.0f, 1.0f, 1.0f, 0.5f);
#endif

		spriteBatchEnd();


	}

	//touch=false; 
}


void Engine::playing(float time) {

}

void Engine::handleInput(int type, int direction_) {
	if (direction_ & LEFT) {
		if (!(direction & LEFT)) {
			legsAnimationRight = 0;
			framesRight = 1;
		}
	}

	if (direction_ & RIGHT) {
		if (!(direction & RIGHT)) {
			legsAnimationLeft = 0;
			framesLeft = 1;
		}
	}
	direction = direction_;
}


void Engine::setTouch(bool _touch)
{
	touch = _touch;
}
