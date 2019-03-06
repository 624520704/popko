#ifndef __THUNDER_H__
#define __THUNDER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
class Thunder : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene(bool a);
	void setPhysicsWorld(PhysicsWorld * world);
	void setcheat(bool a) {
		cheat = a;
	}

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	// implement the "static create()" method manually
	CREATE_FUNC(Thunder);

private:
	bool cheat;
	PhysicsWorld * m_world;
	cocos2d::ProgressTimer* pT;
	cocos2d::Texture2D* texture;
	Vector<Sprite*> bullets;
	Vector<Sprite*> enemys;
	Vector<Sprite*> hearts;
	cocos2d::Label* timeLabel;
	Sprite* player;
	Sprite* point;
	Sprite* boss;
	Sprite* bgsprite;
	Sprite* enemy;
	Sprite* enemy2;
	int x1;
	int x2;
	bool mode2flag = false;
	Size visibleSize;
	Vec2 origin;
	int speed;
	int mode;
	int mode2;
	int life;
	int hp;
	int flag = 0;
	int flag2 = 0;
	int flag3 = 0;
	bool flag4 = 0;
	bool win = 0;
	float r = 0;
	float r2 = 0;
	float ran;
	bool z = 0;
	bool x = 0;
	bool c = 0;
	int ds = 0;
	int s = 0;
	int m = 0;
	cocos2d::Vector<SpriteFrame*> rotation;

	void addKeyboardListener();
	void addContactListener();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onConcactBegin(PhysicsContact & contact);
	void menuBeginCallback(cocos2d::Ref* pSender);
	void invincible(float ft);
	void nextmode();
	void fresh();
	void bound();
	void timeCal();
	void shoot(float ft);
	void eneshoot(float ft);
	void update(float ft);
	void victory();
	void defeat();
	void ProducerTest();

	void setJoint();
	void cleanEnemys();
	void preLoad();
	void Rotation();
	void addSprite();
	void setrecord();
};

#endif // __THUNDER_H__
