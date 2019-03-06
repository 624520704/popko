#ifndef __RecordScene_SCENE_H__
#define __RecordScene_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class RecordScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void menuBeginCallback(cocos2d::Ref* pSender);
	void setrecord();
    
    // implement the "static create()" method manually
    CREATE_FUNC(RecordScene);

private:
	cocos2d::Menu* menu;
	cocos2d::MenuItemLabel* beginItem;
	cocos2d::MenuItemLabel* recordItem;
	cocos2d::MenuItemLabel* exitItem;
};

#endif // __RecordScene_SCENE_H__
