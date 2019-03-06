#include "RecordScene.h"
#include "AudioEngine.h"
#define database	UserDefault::getInstance()


USING_NS_CC;
using namespace experimental;

Scene* RecordScene::createScene()
{
    return RecordScene::create();
}


// on "init" you need to initialize your instance
bool RecordScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bgsprite = Sprite::create("background.jpg");
	bgsprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 58));
	bgsprite->setScale(0.7);
	this->addChild(bgsprite, 0);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	auto *jpnStrings = Dictionary::createWithContentsOfFile("menu.xml");
	const char *str1 = ((String*)jpnStrings->objectForKey("back"))->getCString();
	auto backLabel = Label::create(str1, "arial.ttf", 48);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(RecordScene::menuBeginCallback, this));
	backItem->setPosition(50, visibleSize.height - 50);
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	setrecord();

    
    return true;
}

void RecordScene::menuBeginCallback(cocos2d::Ref* pSender) {
	AudioEngine::play2d("music/combobreak.mp3");
	auto scene = RecordScene::createScene();
	Director::getInstance()->popScene();
}

void RecordScene::setrecord() {
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);

		for (int i = 0; i < 10; i++) {
			const char cds[4] = { i + 48,'d','s',0 };
			const char cs[3] = { i + 48,'s',0 };
			const char cm[3] = { i + 48,'m',0 };

			database->setIntegerForKey(cds, 59);
			database->setIntegerForKey(cs, 59);
			database->setIntegerForKey(cm, 59);
		}
	}
		for (int i = 0; i < 10; i++) {
			const char cds[4] = { i + 48,'d','s',0 };
			const char cs[3] = { i + 48,'s',0 };
			const char cm[3] = { i + 48,'m',0 };
			int a = database->getIntegerForKey(cds);
			int b = database->getIntegerForKey(cs);
			int c = database->getIntegerForKey(cm);

			char* dstime = new char[2];
			sprintf(dstime, "%d", a);
			char* stime = new char[2];
			sprintf(stime, "%d", b);
			char* mtime = new char[2];
			sprintf(mtime, "%d", c);
			std::string temp;
			temp.append(mtime);
			temp.append(":");
			temp.append(stime);
			temp.append(":");
			temp.append(dstime);
			auto timeLabel = Label::create();
			timeLabel->setString(temp);
			timeLabel->setColor(Color3B::BLACK);
			timeLabel->setSystemFontSize(48);
			timeLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height - 100 - i * 50);
			addChild(timeLabel);
		}
}