//
//  GameEnd.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "GameEnd.h"
#include "SplashScreen.h"
#include "Common.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

GameEnd::~GameEnd() {
    
}

Scene* GameEnd::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    GameEnd *layer = GameEnd::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool GameEnd::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    LabelBMFont *skipLabel = LabelBMFont::create("Press Start to skip", "miniwhitefont.fnt", 160, kTextAlignmentRight);
    skipLabel->setColor(gbLightColor3);
    skipLabel->setAnchorPoint(ccp(1, 0));
    skipLabel->setPosition(ccp(157, 0));
    skipLabel->setVisible(false);
    skipLabel->getTexture()->setAliasTexParameters();
    
    LabelBMFont *phrase1Label = LabelBMFont::create("Finally, sun light!", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase1Label->setColor(gbLightestColor3);
    phrase1Label->setAnchorPoint(ccp(0, 1));
    phrase1Label->setPosition(ccp(5, 140));
    phrase1Label->setVisible(false);
    phrase1Label->getTexture()->setAliasTexParameters();
    
    LabelBMFont *phrase2Label = LabelBMFont::create("Now, I just have to go back to the camp.", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase2Label->setColor(gbLightestColor3);
    phrase2Label->setAnchorPoint(ccp(0, 1));
    phrase2Label->setPosition(ccp(5, 119));
    phrase2Label->setVisible(false);
    
    LabelBMFont *phrase3Label = LabelBMFont::create("But, wait...", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase3Label->setColor(gbLightestColor3);
    phrase3Label->setAnchorPoint(ccp(0, 1));
    phrase3Label->setPosition(ccp(5, 86));
    phrase3Label->setVisible(false);
    
    LabelBMFont *phrase4Label = LabelBMFont::create("Where am I?!", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase4Label->setColor(gbLightestColor3);
    phrase4Label->setAnchorPoint(ccp(0, 1));
    phrase4Label->setPosition(ccp(5, 65));
    phrase4Label->setVisible(false);
    
    LabelBMFont *thanksLabel = LabelBMFont::create("THANK YOU FOR PLAYING!", "whitefont.fnt", 150, kTextAlignmentCenter);
    thanksLabel->setColor(gbLightestColor3);
    thanksLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    thanksLabel->setPosition(ccp(80, 72));
    thanksLabel->setVisible(false);
    
    LayerColor *bg = LayerColor::create(ccc4(255, 255, 255, 255));
    bg->setColor(gbDarkestColor3);
    
    float colorDelay = 0.25f;
    
    Action *bgAction = Sequence::create(
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightestColor3.r, gbLightestColor3.g, gbLightestColor3.b),
                                      NULL);
    
    bg->runAction(bgAction);
    
    Action *skipLabelAction = Sequence::create(
                                        DelayTime::create(colorDelay * 3),
                                        Show::create(),
                                        NULL);
    
    skipLabel->runAction(skipLabelAction);
    
    this->addChild(bg);
    this->addChild(skipLabel);
    this->addChild(phrase1Label);
    this->addChild(phrase2Label);
    this->addChild(phrase3Label);
    this->addChild(phrase4Label);
    this->addChild(thanksLabel);
    
    labelAnimationFinishCount = 0;
    finished = false;
    
    this->animateLabel(phrase1Label, 2);
    this->animateLabel(phrase2Label, 4);
    this->animateLabel(phrase3Label, 6);
    this->animateLabel(phrase4Label, 8);
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("song_gameend.wav");
    
    return true;
}

void GameEnd::buttonStart(bool pressed) {
    if (pressed || finished) return;
    finished = true;
    
    Array *children = this->getChildren();
    
    for (int i = 1; i < 6; i++) {
        Node *node = (Node *)children->objectAtIndex(i);
        node->stopAllActions();
        node->setVisible(false);
    }
    
    LabelBMFont *thanksLabel = (LabelBMFont *)children->objectAtIndex(6);
    
    float colorDelay = 0.25f;
    
    Action *action = Sequence::create(
                                      DelayTime::create(colorDelay),
                                      Show::create(),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkestColor3.r, gbDarkestColor3.g, gbDarkestColor3.b),
                                      DelayTime::create(7.0f),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightestColor3.r, gbLightestColor3.g, gbLightestColor3.b),
                                      DelayTime::create(colorDelay),
                                      CallFunc::create(this, callfunc_selector(GameEnd::goToTitleScreen)),
                                      NULL);
    
    thanksLabel->runAction(action);
}

void GameEnd::animateLabel(Node *node, float delay) {
    
    float colorDelay = 0.25f;
    
    Action *action = Sequence::create(
                                      DelayTime::create(delay),
                                      Show::create(),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkestColor3.r, gbDarkestColor3.g, gbDarkestColor3.b),
                                      DelayTime::create(11.0f - delay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightestColor3.r, gbLightestColor3.g, gbLightestColor3.b),
                                      DelayTime::create(colorDelay),
                                      CallFunc::create(this, callfunc_selector(GameEnd::finishLabelAnimation)),
                                      NULL);
    
    node->runAction(action);
}

void GameEnd::finishLabelAnimation() {
    labelAnimationFinishCount++;
    if (labelAnimationFinishCount >= 4)
        this->buttonStart(false);
}

void GameEnd::goToTitleScreen() {
    
    Director::sharedDirector()->replaceScene(SplashScreen::scene());
    
}