//
//  SplashScreen.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "SplashScreen.h"
#include "TitleScreen.h"
#include "Common.h"

SplashScreen::~SplashScreen() {
    
}

Scene* SplashScreen::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    SplashScreen *layer = SplashScreen::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("logos.plist");
    
    Sprite *gbJamLogo = Sprite::createWithSpriteFrameName("gbjam_logo.png");
    gbJamLogo->setPosition(ccp(80, 216));
    gbJamLogo->getTexture()->setAliasTexParameters();
    
    Sprite *baLogo = Sprite::createWithSpriteFrameName("ba_logo.png");
    baLogo->setPosition(ccp(80, 216));
    
    LayerColor *bg = LayerColor::create(gbDarkestColor);
    
    this->addChild(bg);
    this->addChild(gbJamLogo);
    this->addChild(baLogo);
    
    Action *gbJamLogoAction = Sequence::create(
                                               DelayTime::create(0.2f),
                                               MoveBy:: create(0.25f, ccp(0, -144)),
                                               DelayTime::create(2.0f),
                                               MoveBy:: create(0.25f, ccp(0, -144)),
                                               NULL);
    
    Action *baLogoAction = Sequence::create(
                                            DelayTime::create(2.7f),
                                            MoveBy:: create(0.25f, ccp(0, -144)),
                                            DelayTime::create(2.0f),
                                            MoveBy:: create(0.25f, ccp(0, -144)),
                                            DelayTime::create(0.2f),
                                            CallFunc::create(this, callfunc_selector(SplashScreen::goToTitleScreen)),
                                            NULL);
    
    gbJamLogo->runAction(gbJamLogoAction);
    baLogo->runAction(baLogoAction);
    
    return true;
}

void SplashScreen::goToTitleScreen() {
    Director::sharedDirector()->replaceScene(TitleScreen::scene());
}