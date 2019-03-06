#include "Thunder.h"
#include <cmath>
#include <algorithm>
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()

USING_NS_CC;

using namespace experimental;
using namespace CocosDenshion;

void Thunder::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }


/*
	to create a physicsworld
*/
Scene* Thunder::createScene(bool a) {
	srand((unsigned)time(NULL));
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);

	scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));

	auto layer = Thunder::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	layer->setJoint();
	layer->setcheat(a);
	scene->addChild(layer);
	// work while testing and debuging
	if (layer->cheat)  layer->ProducerTest();
	return scene;
}

/*
	to initialize
*/
bool Thunder::init() {
	if (!Layer::init()) {
		return false;
	}

	speed = 150;
	mode = 0;
	mode2 = 0;
	hp = 1000;
	life = 3;
	flag = 0;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	x1 = 1;
	x2 = visibleSize.width - 1;

	// add background
	// background will move as the hp of boss decreasing
	bgsprite = Sprite::create("background.jpg");
	bgsprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 58));
	bgsprite->setScale(0.7);
	this->addChild(bgsprite, 0);
	preLoad();
	addSprite();

	// add timer
	timeLabel = Label::create();
	timeLabel->setColor(Color3B::BLACK);
	timeLabel->setSystemFontSize(36);
	timeLabel->setString("00:00:00");
	timeLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	addChild(timeLabel);

	
	addKeyboardListener();
	addContactListener();

	
	scheduleUpdate();
	schedule(schedule_selector(Thunder::shoot), 0.2f, kRepeatForever, 1.0f);
	schedule(schedule_selector(Thunder::eneshoot), 0, kRepeatForever, 1.0f);

	return true;
}

void Thunder::update(float ft) {
	fresh();
	bound();
	timeCal();
}

/*
	timing
*/
void Thunder::timeCal() {
	ds++;
	if (ds == 60) {
		ds = 0;
		s++;
	}
	if (s == 60) {
		s = 0;
		m++;
	}
	char* dstime = new char[2];
	sprintf(dstime, "%d", ds);
	char* stime = new char[2];
	sprintf(stime, "%d", s);
	char* mtime = new char[2];
	sprintf(mtime, "%d", m);
	std::string temp;
	temp.append(mtime);
	temp.append(":");
	temp.append(stime);
	temp.append(":");
	temp.append(dstime);
	timeLabel->setString(temp);
}

/*
	change the attack mode of player and boss
*/
void Thunder::fresh() {
	if (pT->getPercentage() <= 90 && mode2 == 0) {
		nextmode();
	}
	else if (pT->getPercentage() <= 72 && mode2 == 1) {
		enemy->removeFromParent();
		enemy2->removeFromParent();
		nextmode();
	}
	else if (pT->getPercentage() <= 54 && mode2 == 2) {
		nextmode();
	}
	else if (pT->getPercentage() <= 36 && mode2 == 3) {
		nextmode();
	}
	else if (pT->getPercentage() <= 18 && mode2 == 4) {
		nextmode();
	}


	if (pT->getPercentage() <= 75 && pT->getPercentage() > 50 && mode == 0) {
		mode++;
		AudioEngine::play2d("music/2D.mp3", false, 0.5f);
	}
	else if (pT->getPercentage() <= 50 && pT->getPercentage() > 25 && mode == 1) {
		mode++;
		AudioEngine::play2d("music/yareyare.mp3", false, 0.5f);
	}
}

/*
	setjoint and link decision point with player
*/
void Thunder::setJoint() {
	auto joint1 = PhysicsJointPin::construct(
		point->getPhysicsBody(), player->getPhysicsBody(),
		player->getPosition()
	);
	m_world->addJoint(joint1);
}

/*
	add player, lives, boss, hp of boss
*/
void Thunder::addSprite() {
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);

	auto heart0 = Sprite::create("heart.png");
	auto heart1 = Sprite::create("heart.png");
	auto heart2 = Sprite::create("heart.png");
	heart0->setScale(0.2);
	heart1->setScale(0.2);
	heart2->setScale(0.2);
	heart0->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 20));
	heart1->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 40));
	heart2->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 60));
	addChild(heart0, 4);
	addChild(heart1, 4);
	addChild(heart2, 4);
	hearts.pushBack(heart0);
	hearts.pushBack(heart1);
	hearts.pushBack(heart2);

	enemy = Sprite::create("bullet.png");
	/*enemy->setPosition(Vec2(2, boss->getPosition().y));
	auto b1 = PhysicsBody::create();
	b1->setCategoryBitmask(0x0000);
	b1->setCollisionBitmask(0x0000);
	b1->setContactTestBitmask(0x0000);
	b1->setDynamic(false);
	enemy->setPhysicsBody(b1);*/
	addChild(enemy);

	enemy2 = Sprite::create("bullet.png");
	/*enemy2->setPosition(Vec2(visibleSize.width-2, boss->getPosition().y));
	auto b2 = PhysicsBody::create();
	b2->setCategoryBitmask(0x0000);
	b2->setCollisionBitmask(0x0000);
	b2->setContactTestBitmask(0x0000);
	b2->setDynamic(false);
	enemy2->setPhysicsBody(b2);*/
	addChild(enemy2);

	boss = Sprite::create("Popko.png");
	boss->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height - 100));
	boss->setScale(0.3);
	auto bossbody = PhysicsBody::createBox(boss->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
	bossbody->setTag(0);
	bossbody->setCategoryBitmask(0xF000);
	bossbody->setCollisionBitmask(0x0000);
	bossbody->setContactTestBitmask(0x0F00);
	bossbody->setDynamic(false);
	boss->setPhysicsBody(bossbody);
	addChild(boss, 2);

	texture = Director::getInstance()->getTextureCache()->addImage("Pipimi.png");
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 137, 211)));
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2, 40));
	player->setScale(0.3);
	auto playbody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
	playbody->setCategoryBitmask(0x0000);
	playbody->setContactTestBitmask(0x0000);
	playbody->setCollisionBitmask(0x0000);
	playbody->setTag(2);
	playbody->setDynamic(false);
	player->setPhysicsBody(playbody);
	addChild(player, 2);

	point = Sprite::create("RedPoint.png");
	point->setScale(0.5);
	point->setPosition(player->getPosition());
	auto pointbody = PhysicsBody::createCircle(5.0f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
	pointbody->setTag(1);
	pointbody->setDynamic(true);
	pointbody->setGravityEnable(false);
	pointbody->setContactTestBitmask(0x00F0);
	pointbody->setCollisionBitmask(0x0000);
	pointbody->setCategoryBitmask(0x000F);
	point->setPhysicsBody(pointbody);
	addChild(point, 10);


	Rotation();
}

/*
	preload picture and sound
*/
void Thunder::preLoad() {
	Director::getInstance()->getTextureCache()->addImage("LeftHand.png");
	Director::getInstance()->getTextureCache()->addImage("RightHand.png");
	Director::getInstance()->getTextureCache()->addImage("BluePoint.png");
	Director::getInstance()->getTextureCache()->addImage("GreenPoint.png");
	Director::getInstance()->getTextureCache()->addImage("RedPoint.png");
	Director::getInstance()->getTextureCache()->addImage("bullet.png");
	Director::getInstance()->getTextureCache()->addImage("bullet2.png");
	AudioEngine::preload("music/ka.mp3");
	AudioEngine::preload("music/okotta.mp3");
}

/*
	player plays a animation
	boss rotates forever
*/
void Thunder::Rotation() {
	rotation.reserve(6);
	for (int i = 0; i < 6; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(137 * i, 0, 137, 211)));
		rotation.pushBack(frame);
	}
	auto animationR = Animation::createWithSpriteFrames(rotation, 0.1f);
	auto animateR = Animate::create(animationR);
	RepeatForever *repeatForeverActiong = RepeatForever::create(animateR);
	player->runAction(repeatForeverActiong);

	auto ro1 = RotateBy::create(0.75, 90);
	auto ro2 = RotateBy::create(1.5, -180);
	auto ro3 = RotateBy::create(0.75, 90);
	auto ro = Sequence::create(ro1, ro2, ro3, nullptr);
	auto rorepeat = RepeatForever::create(ro);
	boss->runAction(rorepeat);
}

/*
	limit player in the bound
	remove bullets out of bound
*/
void Thunder::bound() {
	auto position = player->getPosition();
	if (position.x <= 0) {
		player->setPosition(1, position.y);
	}
	if (position.x >= visibleSize.width) {
		player->setPosition(visibleSize.width - 1, position.y);
	}
	if (position.y <= 0) {
		player->setPosition(position.x, 1);
	}
	if (position.y >= visibleSize.height) {
		player->setPosition(position.x, visibleSize.height - 1);
	}
	for (int i = 0; i < bullets.size(); i++) {
		auto a = bullets.at(i);
		auto p = a->getPosition();
		if (p.x < 0 || p.x > visibleSize.width || p.y < 0 || p.y > visibleSize.height) {
			a->removeFromParent();
			bullets.erase(i);
			i = -1;
		}
	}
	for (int i = 0; i < enemys.size(); i ++) {
		auto a = enemys.at(i);
		auto p = a->getPosition();
		if (p.x < 0 || p.x > visibleSize.width || p.y < 0 || p.y > visibleSize.height) {
			a->removeFromParent();
			enemys.erase(i);
			i = -1;
		}
	}
	//point->setPosition(position);
}

/*
	callback function
	player shoots
*/
void Thunder::shoot(float ft) {
	if (mode == 0 || mode == 1 || mode == 2) {
		auto bullet = Sprite::create("bowknot.png");
		bullet->setPosition(Vec2(player->getPosition().x, player->getPosition().y + 20));
		bullet->setScale(0.3);
		auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
		bulletBody->setVelocity(Vec2(0, 400));
		bulletBody->setCategoryBitmask(0x0F00);
		bulletBody->setCollisionBitmask(0x0000);
		bulletBody->setContactTestBitmask(0xF000);
		bulletBody->setTag(3);
		bulletBody->setDynamic(true);
		bulletBody->setGravityEnable(false);
		bullets.pushBack(bullet);
		bullet->setPhysicsBody(bulletBody);
		addChild(bullet, 1);
	}
	if (mode == 1 || mode == 2) {
		auto bullet = Sprite::create("RightHand.png");
		bullet->setPosition(Vec2(player->getPosition().x - 15, player->getPosition().y + 20));
		bullet->setScale(0.1);

		auto bullet2 = Sprite::create("LeftHand.png");
		bullet2->setPosition(Vec2(player->getPosition().x + 15, player->getPosition().y + 20));
		bullet2->setScale(0.1);

		auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
		bulletBody->setVelocity(Vec2(0, 400));
		bulletBody->setCategoryBitmask(0x0F00);
		bulletBody->setCollisionBitmask(0x0000);
		bulletBody->setContactTestBitmask(0xF000);
		bulletBody->setTag(3);
		bulletBody->setDynamic(true);
		bulletBody->setGravityEnable(false);

		auto bulletBody2 = PhysicsBody::createBox(bullet2->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
		bulletBody2->setVelocity(Vec2(0, 400));
		bulletBody2->setCategoryBitmask(0x0F00);
		bulletBody2->setCollisionBitmask(0x0000);
		bulletBody2->setContactTestBitmask(0xF000);
		bulletBody2->setTag(3);
		bulletBody2->setDynamic(true);
		bulletBody2->setGravityEnable(false);

		bullets.pushBack(bullet);
		bullets.pushBack(bullet2);
		bullet2->setPhysicsBody(bulletBody2);
		bullet->setPhysicsBody(bulletBody);
		addChild(bullet, 1);
		addChild(bullet2, 1);
	}
	if (mode == 2) {
		auto bullet = Sprite::create("bullet2.png");
		bullet->setPosition(Vec2(player->getPosition().x - 40, player->getPosition().y + 20));
		bullet->setScale(0.1);

		auto bullet2 = Sprite::create("bullet2.png");
		bullet2->setPosition(Vec2(player->getPosition().x + 40, player->getPosition().y + 20));
		bullet2->setScale(0.1);

		auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
		bulletBody->setCategoryBitmask(0x0F00);
		bulletBody->setCollisionBitmask(0x0000);
		bulletBody->setContactTestBitmask(0xF000);
		bulletBody->setTag(3);
		bulletBody->setDynamic(true);
		bulletBody->setGravityEnable(false);

		auto bulletBody2 = PhysicsBody::createBox(bullet2->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
		bulletBody2->setCategoryBitmask(0x0F00);
		bulletBody2->setCollisionBitmask(0x0000);
		bulletBody2->setContactTestBitmask(0xF000);
		bulletBody2->setTag(3);
		bulletBody2->setDynamic(true);
		bulletBody2->setGravityEnable(false);

		bullets.pushBack(bullet);
		bullets.pushBack(bullet2);
		bullet2->setPhysicsBody(bulletBody2);
		bullet->setPhysicsBody(bulletBody);
		addChild(bullet, 1);
		addChild(bullet2, 1);

		auto mt = MoveTo::create(1, boss->getPosition());
		auto mt2 = MoveTo::create(1, boss->getPosition());
		bullet->runAction(mt);
		bullet2->runAction(mt2);
	}
}

/*
	callback function
	enemy shoots
*/
void Thunder::eneshoot(float ft) {
	if (mode2 == 0 || mode2 == 1) {
		auto bullet = Sprite::create("BluePoint.png");
		auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
		auto angle = boss->getRotation();
		if (flag == 0) {
			bullet->setPosition(boss->getPosition());
			angle = angle * std::_Pi / 180;
			bulletBody->setVelocity(Vec2(-100 * sin(angle), -100 * cos(angle)));
			log(sin(angle));
			log(cos(angle));
			bulletBody->setCategoryBitmask(0x00F0);
			bulletBody->setCollisionBitmask(0x0000);
			bulletBody->setContactTestBitmask(0x000F);
			bulletBody->setTag(4);
			bulletBody->setDynamic(true);
			bulletBody->setGravityEnable(false);
			enemys.pushBack(bullet);
			bullet->setPhysicsBody(bulletBody);
			addChild(bullet, 3);
			flag++;
		}
		else if (flag == 1) {
			flag = 0;
		}
	}

	if (mode2 == 1) {
		if (x1 < visibleSize.width / 2 - 20 && mode2flag == false) {
			x1 += 2;
			x2 -= 2;
		}
		else if (x1 >= visibleSize.width / 2 - 20 && mode2flag == false) {
			mode2flag = true;
		}

		if (x1 > 1 && mode2flag == true) {
			x1 -= 2;
			x2 += 2;
		}
		else if (x1 <= 1 && mode2flag == true) {
			mode2flag = false;
		}

		enemy->setPosition(x1, boss->getPosition().y);
		enemy2->setPosition(x2, boss->getPosition().y);

		auto bullet = Sprite::create("bullet.png");
		auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));

		auto bullet2 = Sprite::create("bullet.png");
		auto bulletBody2 = PhysicsBody::createBox(bullet2->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));

		switch (flag2) {
		case 0:
			AudioEngine::play2d("music/ka.mp3", false, 0.5f);
			bullet->setPosition(Vec2(x1, boss->getPosition().y));
			bulletBody->setVelocity(Vec2(0, -200));
			bulletBody->setCategoryBitmask(0x00F0);
			bulletBody->setCollisionBitmask(0x0000);
			bulletBody->setContactTestBitmask(0x000F);
			bulletBody->setTag(4);
			bulletBody->setDynamic(true);
			bulletBody->setGravityEnable(false);
			enemys.pushBack(bullet);
			bullet->setPhysicsBody(bulletBody);
			addChild(bullet, 3);

			bullet2->setPosition(Vec2(x2, boss->getPosition().y));
			bulletBody2->setVelocity(Vec2(0, -200));
			bulletBody2->setCategoryBitmask(0x00F0);
			bulletBody2->setCollisionBitmask(0x0000);
			bulletBody2->setContactTestBitmask(0x000F);
			bulletBody2->setTag(4);
			bulletBody2->setDynamic(true);
			bulletBody2->setGravityEnable(false);
			enemys.pushBack(bullet2);
			bullet2->setPhysicsBody(bulletBody2);
			addChild(bullet2, 3);

			flag2++;
			break;
		case 1:
			flag2++;
			break;
		case 2:
			flag2++;
			break;
		case 3:
			flag2 = 0;
			break;
		default:
			flag2 = 0;
			break;
		}
	}

	if (mode2 == 2) {
		if (flag3 == 0) {
			int n = visibleSize.width / 2;
			for (int i = 0; i < n; i += 10) {
				AudioEngine::play2d("music/ka.mp3", false, 0.5f);
				auto bullet = Sprite::create("bullet.png");
				auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
				bullet->setPosition(Vec2(i, boss->getPosition().y));
				bulletBody->setVelocity(Vec2(0, -200));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}

			flag3++;
		}
		else if (flag3 >= 1 && flag3 <= 29) {
			AudioEngine::play2d("music/ka.mp3", false, 0.5f);
			flag3++;
		}
		else if (flag3 == 30) {
			int n = visibleSize.width / 2;
			for (int i = visibleSize.width; i > n; i -= 10) {
				auto bullet = Sprite::create("bullet.png");
				auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
				bullet->setPosition(Vec2(i, boss->getPosition().y));
				bulletBody->setVelocity(Vec2(0, -200));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag3++;
		}
		else if (flag3 >= 31 && flag3 <= 58) {
			AudioEngine::play2d("music/ka.mp3", false, 0.5f);
			flag3++;
		}
		else if (flag3 == 59) flag3 = 0;
		else flag3 = 0;
	}

	if (mode2 == 3) {
		if (flag == 0) {
			for (int i = 0; i < 360; i += 6) {
				auto bullet = Sprite::create("BluePoint.png");
				auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
				auto angle = boss->getRotation();
				bullet->setPosition(boss->getPosition());
				angle = i * std::_Pi / 180;
				bulletBody->setVelocity(Vec2(-100 * sin(angle), -100 * cos(angle)));
				log(sin(angle));
				log(cos(angle));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag++;
		}
		else if (flag >= 1 && flag <= 19) {
			flag++;
		}
		else if (flag == 20) {
			for (int i = 2; i < 360; i += 6) {
				auto bullet = Sprite::create("GreenPoint.png");
				auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
				auto angle = boss->getRotation();
				bullet->setPosition(boss->getPosition());
				angle = i * std::_Pi / 180;
				bulletBody->setVelocity(Vec2(-200 * sin(angle), -200 * cos(angle)));
				log(sin(angle));
				log(cos(angle));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag++;
		}
		else if (flag >= 21 && flag <= 39) {
			flag++;
		}
		else if (flag == 40) {
			for (int i = 4; i < 360; i += 6) {
				auto bullet = Sprite::create("GreenPoint.png");
				auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
				auto angle = boss->getRotation();
				bullet->setPosition(boss->getPosition());
				angle = i * std::_Pi / 180;
				bulletBody->setVelocity(Vec2(-200 * sin(angle), -200 * cos(angle)));
				log(sin(angle));
				log(cos(angle));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag++;
		}
		else if (flag >= 41 && flag <= 58) {
			flag++;
		}
		else if (flag == 59) flag = 0;
	}

	if (mode2 == 4) {
		if (flag >= 0 && flag <= 29) {
			for (int i = 0; i < 12; i ++) {
				auto bullet = Sprite::create("BluePoint.png");
				auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
				auto angle = boss->getRotation();
				bullet->setPosition(boss->getPosition());
				angle = (r / 48 + i * 30) * std::_Pi / 180;
				bulletBody->setVelocity(Vec2(-300 * sin(angle), -300 * cos(angle)));
				log(sin(angle));
				log(cos(angle));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
				r ++;
			}
			flag++;
		}
		else if (flag == 30) {
			for (int i = 0; i < 360; i += 12) {
				auto bullet = Sprite::create("GreenPoint.png");
				auto bulletBody = PhysicsBody::createCircle(7.5f, PhysicsMaterial(1.0f, 0.0f, 0.0f));
				auto angle = boss->getRotation();
				bullet->setPosition(boss->getPosition());
				angle = i * std::_Pi / 180;
				bulletBody->setVelocity(Vec2(-100 * sin(angle), -100 * cos(angle)));
				log(sin(angle));
				log(cos(angle));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag=0;
		}
	}

	if (mode2 == 5) {
		if (flag3 == 0) {
			int n = std::_Pi/2;
			for (float i = 0; i < n; i += 0.1) {
				AudioEngine::play2d("music/ka.mp3", false, 0.5f);
				auto bullet = Sprite::create("bullet.png");
				auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
				bullet->setPosition(Vec2(1, visibleSize.height - 1));
				bulletBody->setVelocity(Vec2(150 * sin(i), -150 * cos(i)));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				ran = player->getPosition().x;
				addChild(bullet, 3);
			}

			flag3++;
		}
		else if (flag3 >= 1 && flag3 <= 29) {
			Sprite* bullet;
			if (flag4 == 0) {
				bullet = Sprite::create("BluePoint.png");
				flag4 = 1;
			}
			else {
				bullet = Sprite::create("GreenPoint.png");
				flag4 = 0;
			}
			auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
			bullet->setPosition(Vec2(ran, boss->getPosition().y));
			bulletBody->setVelocity(Vec2(0, -200));
			bulletBody->setCategoryBitmask(0x00F0);
			bulletBody->setCollisionBitmask(0x0000);
			bulletBody->setContactTestBitmask(0x000F);
			bulletBody->setTag(4);
			bulletBody->setDynamic(true);
			bulletBody->setGravityEnable(false);
			enemys.pushBack(bullet);
			bullet->setPhysicsBody(bulletBody);
			addChild(bullet, 3);
			flag3++;
		}
		else if (flag3 == 30) {
			int n = std::_Pi / 2;
			for (float i = 0; i < n; i += 0.1) {
				AudioEngine::play2d("music/ka.mp3", false, 0.5f);
				auto bullet = Sprite::create("bullet.png");
				auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
				bullet->setPosition(Vec2(visibleSize.width - 1, visibleSize.height-1));
				bulletBody->setVelocity(Vec2(-150 * sin(i), -150 * cos(i)));
				bulletBody->setCategoryBitmask(0x00F0);
				bulletBody->setCollisionBitmask(0x0000);
				bulletBody->setContactTestBitmask(0x000F);
				bulletBody->setTag(4);
				bulletBody->setDynamic(true);
				bulletBody->setGravityEnable(false);
				enemys.pushBack(bullet);
				bullet->setPhysicsBody(bulletBody);
				addChild(bullet, 3);
			}
			flag3++;
		}
		else if (flag3 >= 31 && flag3 <= 58) {
			flag3++;
		}
		else if (flag3 == 59) {
			flag3 = 0;
		}
		else flag = 0;
	}
}

void Thunder::addKeyboardListener() {
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

/*
	to control player
	player can press two or more keys at the same time
*/
void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	auto v = player->getPhysicsBody()->getVelocity();
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x - speed, v.y));
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x + speed, v.y));
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x, v.y + speed));
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x, v.y - speed));
		break;
	}
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	auto v = player->getPhysicsBody()->getVelocity();
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x + speed, v.y));
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x - speed, v.y));
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x, v.y - speed));
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player->getPhysicsBody()->setVelocity(Vec2(v.x, v.y + speed));
		break;
	}
}


void Thunder::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Thunder::onConcactBegin, this);
	//contactListener->onContactPreSolve = CC_CALLBACK_1(HitBrick::onContactPreSolve, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

/*
	judge the sprite concact
	decrease the hp of boss
	decrease the lives of player
*/
bool Thunder::onConcactBegin(PhysicsContact & contact) {
	auto c1 = contact.getShapeA()->getBody(), c2 = contact.getShapeB()->getBody();
	if (c1->getTag() == 0 && c2->getTag() == 3) {
			hp -= 1;
			pT->setPercentage(hp / 10);
			bgsprite->runAction(MoveBy::create(1.0f, Vec2(0, -0.116)));
		if (hp == 0) {
			win = 1;
			victory();
		}
		c2->getNode()->removeFromParent();
		return true;
	}
	else if (c1->getTag() == 3 && c2->getTag() == 0) {
			hp -= 1;
			pT->setPercentage(hp / 10);
			bgsprite->runAction(MoveBy::create(1.0f, Vec2(0, -0.116)));
		if (hp == 0) {
			win = 1;
			victory();
		}
		c1->getNode()->removeFromParent();
		return true;
	}

	if (c1->getTag() == 1 && c2->getTag() == 4) {
		AudioEngine::play2d("music/okotta.mp3", false, 1.0f);
		life--;
		hearts.at(hearts.size() - 1)->removeFromParent();
		hearts.popBack();
		if (life == 0 && !win) defeat();
		else {
			cleanEnemys();
			auto blink = Blink::create(2, 4);
			player->runAction(blink);
			point->getPhysicsBody()->setContactTestBitmask(0x0000);
			point->getPhysicsBody()->setCollisionBitmask(0x0000);
			point->getPhysicsBody()->setCategoryBitmask(0x0000);
			scheduleOnce(schedule_selector(Thunder::invincible), 2.0f);
		}
		return true;
	}
	else if (c1->getTag() == 4 && c2->getTag() == 1) {
		AudioEngine::play2d("music/okotta.mp3", false, 1.0f);
		life--;
		hearts.at(hearts.size() - 1)->removeFromParent();
		hearts.popBack();
		if (life == 0 && !win) defeat();
		else {
			cleanEnemys();
			auto blink = Blink::create(2, 4);
			player->runAction(blink);
			point->getPhysicsBody()->setContactTestBitmask(0x0000);
			point->getPhysicsBody()->setCollisionBitmask(0x0000);
			point->getPhysicsBody()->setCategoryBitmask(0x0000);
			scheduleOnce(schedule_selector(Thunder::invincible), 2.0f);
		}
		return true;
	}
	return true;
}

/*
	call when player wins
*/
void Thunder::victory() {
	unscheduleAllSelectors();
	boss->stopAllActions();
	
	cleanEnemys();

	auto pa = ParticleGalaxy::create();
	pa->setPosition(boss->getPosition());
	addChild(pa);
	boss->removeFromParent();

	AudioEngine::play2d("music/okottenai.mp3", false, 1.0f);

	auto *jpnStrings = Dictionary::createWithContentsOfFile("menu.xml");
	const char *str1 = ((String*)jpnStrings->objectForKey("back"))->getCString();
	auto backLabel = Label::create(str1, "arial.ttf", 48);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(Thunder::menuBeginCallback, this));
	backItem->setPosition(50, visibleSize.height - 50);
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	const char *str2 = ((String*)jpnStrings->objectForKey("win"))->getCString();
	auto dieLabel = Label::create(str2, "arial.ttf", 96);
	dieLabel->setColor(Color3B::BLACK);
	dieLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(dieLabel, 5);

	setrecord();
}

/*
	call when player is defeated
*/
void Thunder::defeat() {
	unscheduleAllSelectors();
	boss->stopAllActions();
	player->stopAllActions();
	
	point->getPhysicsBody()->setContactTestBitmask(0x0000);
	point->getPhysicsBody()->setCollisionBitmask(0x0000);
	point->getPhysicsBody()->setCategoryBitmask(0x0000);

	for (auto a : enemys) {
		a->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}


	auto *jpnStrings = Dictionary::createWithContentsOfFile("menu.xml");
	const char *str1 = ((String*)jpnStrings->objectForKey("back"))->getCString();
	auto backLabel = Label::create(str1, "arial.ttf", 48);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(Thunder::menuBeginCallback, this));
	backItem->setPosition(50, visibleSize.height - 50);
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	const char *str2 = ((String*)jpnStrings->objectForKey("die"))->getCString();
	auto dieLabel = Label::create(str2, "arial.ttf", 96);
	dieLabel->setColor(Color3B::BLACK);
	dieLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(dieLabel, 5);

}

/*
	back to main page
*/
void Thunder::menuBeginCallback(cocos2d::Ref* pSender) {
	AudioEngine::play2d("music/combobreak.mp3");
	Director::getInstance()->popScene();
}

/*
	will be called after 3s after death
*/
void Thunder::invincible(float ft) {
	point->getPhysicsBody()->setContactTestBitmask(0x00F0);
	point->getPhysicsBody()->setCollisionBitmask(0x0000);
	point->getPhysicsBody()->setCategoryBitmask(0x000F);
}

/*
	clean all enemys
	add particle effects
*/
void Thunder::cleanEnemys() {
	for (auto a : enemys) {
		auto pa = ParticleExplosion::create();
		pa->setPosition(a->getPosition());
		pa->setTotalParticles(100);
		addChild(pa);
		a->removeFromParent();
	}
	enemys.clear();
}

/*
	called when testing and debuging
*/
void Thunder::ProducerTest() {
	point->getPhysicsBody()->setContactTestBitmask(0x0000);
	point->getPhysicsBody()->setCollisionBitmask(0x0000);
	point->getPhysicsBody()->setCategoryBitmask(0x0000);
}

/*
	set the new record if win and great enough 
*/
void Thunder::setrecord() {
	if (cheat) return;
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
		
		database->setIntegerForKey("0ds", ds);
		database->setIntegerForKey("0s", s);
		database->setIntegerForKey("0m", m);

		for (int i = 1; i < 10; i ++) {
			const char cds[4] = { i + 48,'d','s',0 };
			const char cs[3] = { i + 48,'s',0 };
			const char cm[3] = { i + 48,'m',0 };

			database->setIntegerForKey(cds, 59);
			database->setIntegerForKey(cs, 59);
			database->setIntegerForKey(cm, 59);
		}
	}
	else {
		std::vector<int> vds;
		std::vector<int> vs;
		std::vector<int> vm;

		for (int i = 0; i < 10; i++) {
			const char cds[4] = { i + 48,'d','s',0 };
			const char cs[3] = { i + 48,'s',0 };
			const char cm[3] = { i + 48,'m',0 };



			vds.push_back(database->getIntegerForKey(cds));
			vs.push_back(database->getIntegerForKey(cs));
			vm.push_back(database->getIntegerForKey(cm));
		}
		for (int i = 0; i < 10; i++) {
			bool f = false;
			if (m < vm[i]) {
				f = true;
			}
			else if (m == vm[i]) {
				if (s < vs[i]) {
					f = true;
				}
				else if (s == vs[i]) {
					if (ds < vds[i]) {
						f = true;
					}
				}
			}

			if (f == true) {
				vds.insert(vds.begin() + i, ds);
				vs.insert(vs.begin() + i, s);
				vm.insert(vm.begin() + i, m);
				auto *jpnStrings = Dictionary::createWithContentsOfFile("menu.xml");
				const char *str2 = ((String*)jpnStrings->objectForKey("new"))->getCString();
				auto dieLabel = Label::create(str2, "arial.ttf", 68);
				dieLabel->setColor(Color3B::BLACK);
				dieLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 60);
				addChild(dieLabel, 5);
				break;
			}
		}
		for (int i = 0; i < 10; i++) {
			const char cds[4] = { i + 48,'d','s',0 };
			const char cs[3] = { i + 48,'s',0 };
			const char cm[3] = { i + 48,'m',0 };

			database->setIntegerForKey(cds, vds[i]);
			database->setIntegerForKey(cs, vs[i]);
			database->setIntegerForKey(cm, vm[i]);
		}
	}
}

void Thunder::nextmode() {
	r = 0;
	r2 = 0;
	flag = 0;
	flag2 = 0;
	flag3 = 0;
	mode2++;
	cleanEnemys();
	unschedule(schedule_selector(Thunder::eneshoot));
	schedule(schedule_selector(Thunder::eneshoot), 0, kRepeatForever, 3.0f);
	unschedule(schedule_selector(Thunder::shoot));
	schedule(schedule_selector(Thunder::shoot), 0.2f, kRepeatForever, 3.0f);
}