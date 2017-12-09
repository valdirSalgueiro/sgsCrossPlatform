#include "pch.h"
#include "Hellburguer.h"
#include "gmUtil.h"

#include "sound.h"



const unsigned int hellburguer_texcoords[] = {
	0,0,277,263, // 0        Hellburger0024.png
	279,0,277,263, // 1      Hellburger0023.png
	558,0,277,263, // 2      Hellburger0022.png
	0,265,277,263, // 3      Hellburger0021.png
	279,265,277,263, // 4    Hellburger0020.png
	558,265,277,263, // 5    Hellburger0019.png
	0,530,277,263, // 6      Hellburger0018.png
	279,530,277,263, // 7    Hellburger0016.png
	279,530,277,263, // 8    Hellburger0017.png
	558,530,277,263, // 9    Hellburger0014.png
	558,530,277,263, // 10   Hellburger0015.png
	0,795,277,263, // 11     Hellburger0013.png
	279,795,277,263, // 12   Hellburger0012.png
	558,795,277,263, // 13   Hellburger0011.png
	0,1060,277,263, // 14    Hellburger0010.png
	0,1325,277,263, // 15    Hellburger0008.png
	0,1325,277,263, // 16    Hellburger0009.png
	0,1590,277,263, // 17    Hellburger0007.png
	279,1060,277,263, // 18          Hellburger0005.png
	279,1060,277,263, // 19          Hellburger0006.png
	558,1060,277,263, // 20          Hellburger0004.png
	279,1325,277,263, // 21          Hellburger0003.png
	279,1590,277,263, // 22          Hellburger0002.png
	558,1325,277,263, // 23          Hellburger0001.png
};

glImage sprHB[24];
int hbAnimation = 0;

extern std::vector<Object*> objs;
extern sound* alienTiro1;

Hellburguer::Hellburguer() {
	std::string bulletPattern("<bulletml type=\"vertical\" xmlns=\"http://www.asahi-net.or.jp/~cs8k-cyu/bulletml\"><action label=\"top\"><fire><direction type=\"absolute\">0</direction><bulletRef label=\"curved\" /></fire><fire><direction type=\"absolute\">180</direction><bulletRef label=\"curved\" /></fire></action><bullet label=\"curved\"><action><changeDirection><direction type=\"absolute\">-90</direction><term>30</term></changeDirection><wait>30</wait><fire><bullet /></fire></action></bullet></bulletml>");

	isAlive = false;

	selected_path = rand() % 2;

	glLoadSpriteset("hb.png", sprHB, 1024, 2048, 24, hellburguer_texcoords, 0);

	path.push_back(Vector2D<float>(1600 + sprHB[0].width, rand() % 960));
	path.push_back(Vector2D<float>(800 + rand() % 700, 900));
	path.push_back(Vector2D<float>(800 + rand() % 700, 500));
	path.push_back(Vector2D<float>(800 + rand() % 700, 100));
	path.push_back(Vector2D<float>(800 + rand() % 700, 500));
	path.push_back(Vector2D<float>(800 + rand() % 700, 900));
	path.push_back(Vector2D<float>(200 + rand() % 700, 960 + sprHB[0].height));



	bp = new BulletMLParserTinyXML(bulletPattern);
	bp->build();

	radius = sprHB[0].height / 2;

}

Hellburguer::~Hellburguer() {
	delete bp;
}

bool Hellburguer::update(float time) {
	Enemy::update(time);
	pos += dir;
	loops++;

	if (loops % 4 == 0)
		hbAnimation++;

	if (hbAnimation > 23)
		hbAnimation = 0;

	if (x >= 1.0f) {
		x = 0.0f;
		oldPosition = pos;
		pathIter++;

		if (pathIter >= path.size()) {
			if (life > 0) {
				pos = path.at(0);
				oldPosition = pos;
				pathIter = 1;
			}
			else {
				isAlive = false;
			}
		}

	}
	else {
		if (selected_path == 1)
			pos = catmullRomSpline(x, oldPosition, oldPosition, path.at(pathIter), path.at(pathIter));
		else
			pos = catmullRomSpline(x, oldPosition, oldPosition, Vector2D<float>(path.at(pathIter).x, 960 - path.at(pathIter).y), Vector2D<float>(path.at(pathIter).x, 960 - path.at(pathIter).y));
	}


	if (loops % 180 == 0) {
		alienTiro1->play();
		objs.push_back(new Bullet(bp, pos.x, pos.y, 0, 0, COMMON));
	}

	x += 0.01f;
	life--;

	Aabb.Init(pos.x / 2 - sprHB[hbAnimation].width / 4, pos.y / 2 - sprHB[hbAnimation].height / 4, sprHB[hbAnimation].width / 2, sprHB[hbAnimation].height / 2);
	return true;
}


bool Hellburguer::render(float time) {
	if (wasHurt) {
		glSprite(pos.x, pos.y, GL2D_CENTER, &sprHB[hbAnimation], false, 0, 1, 1, 100,100,100,100);
	}
	else {
		glSprite(pos.x, pos.y, GL2D_CENTER, &sprHB[hbAnimation]);
	}
	return true;
}


void Hellburguer::init() {
	loops = 0;
	isAlive = true;
	pos = path.at(0);
	oldPosition = pos;
	pathIter = 1;
	x = 0;

	life = 1000;
	energy = 5;

	Aabb.Init(pos.x / 2 - sprHB[0].width / 4, pos.y / 2 - sprHB[0].height / 4, sprHB[0].width / 2, sprHB[0].height / 2);
}

void Hellburguer::cleanUp() {
}

bool Hellburguer::collides(Vector2D<float> pos, Vector2D<float> size) {
	return false;
}