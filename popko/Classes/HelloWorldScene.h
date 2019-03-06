#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	void menuBeginCallback(cocos2d::Ref* pSender);
	void menuCheatCallback(cocos2d::Ref* pSender);
	void menuRecordCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	cocos2d::Menu* menu;
	cocos2d::MenuItemLabel* beginItem;
	cocos2d::MenuItemLabel* cheatItem;
	cocos2d::MenuItemLabel* recordItem;
	cocos2d::MenuItemLabel* exitItem;
};

#endif // __HELLOWORLD_SCENE_H__
