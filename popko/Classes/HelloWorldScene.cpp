#include "HelloWorldScene.h"
#include "Thunder.h"
#include "RecordScene.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace experimental;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	AudioEngine::preload("music/combobreak.mp3");
	AudioEngine::play2d("music/pop.mp3", true, 0.1f);

	auto bgsprite = Sprite::create("background.jpg");
	bgsprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 58));
	bgsprite->setScale(0.7);
	this->addChild(bgsprite, 0);

	auto *jpnStrings = Dictionary::createWithContentsOfFile("menu.xml");

	// game scene
	const char *str1 = ((String*)jpnStrings->objectForKey("begin"))->getCString();
	auto beginLabel = Label::create(str1, "arial.ttf", 48);
	beginLabel->setColor(Color3B::BLACK);
	beginItem = MenuItemLabel::create(beginLabel, CC_CALLBACK_1(HelloWorld::menuBeginCallback, this));

	// record scene
	const char *str2 = ((String*)jpnStrings->objectForKey("record"))->getCString();
	auto recordLabel = Label::create(str2, "arial.ttf", 48);
	recordLabel->setColor(Color3B::BLACK);
	recordItem = MenuItemLabel::create(recordLabel, CC_CALLBACK_1(HelloWorld::menuRecordCallback, this));

	// exit
	const char *str3 = ((String*)jpnStrings->objectForKey("exit"))->getCString();
	auto exitLabel = Label::create(str3, "arial.ttf", 48);
	exitLabel->setColor(Color3B::BLACK);
	exitItem = MenuItemLabel::create(exitLabel, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	const char *str4 = ((String*)jpnStrings->objectForKey("cheat"))->getCString();
	auto cheatLabel = Label::create(str4, "arial.ttf", 48);
	cheatLabel->setColor(Color3B::BLACK);
	cheatItem = MenuItemLabel::create(cheatLabel, CC_CALLBACK_1(HelloWorld::menuCheatCallback, this));

	beginItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	cheatItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
	recordItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
	exitItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 150));


    // create menu, it's an autorelease object
    menu = Menu::create(beginItem, cheatItem, recordItem, exitItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    return true;
}

void HelloWorld::menuBeginCallback(cocos2d::Ref* pSender) {
	AudioEngine::play2d("music/combobreak.mp3", false, 1.0f);
	auto scene = Thunder::createScene(false);
	Director::getInstance()->pushScene(scene);
}

void HelloWorld::menuCheatCallback(cocos2d::Ref* pSender) {
	AudioEngine::play2d("music/combobreak.mp3", false, 1.0f);
	auto scene = Thunder::createScene(true);
	Director::getInstance()->pushScene(scene);
}

void HelloWorld::menuRecordCallback(cocos2d::Ref* pSender) {
	AudioEngine::play2d("music/combobreak.mp3", false, 1.0f);
	auto scene = RecordScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	AudioEngine::play2d("music/combobreak.mp3", false, 1.0f);
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
