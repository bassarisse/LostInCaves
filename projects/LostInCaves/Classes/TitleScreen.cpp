//
//  TitleScreen.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "TitleScreen.h"
#include "SimpleAudioEngine.h"
#include "Common.h"
#include "Credits.h"
#include "Intro.h"
#include "Cave.h"

using namespace CocosDenshion;

TitleScreen::~TitleScreen() {
    
}

Scene* TitleScreen::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    TitleScreen *layer = TitleScreen::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool TitleScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_back.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_move.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_select.wav");
    
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("logos.plist");
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chars.plist");
    SpriteBatchNode *mainBatchNode = SpriteBatchNode::create("chars.png");
    
    Sprite *bg = Sprite::create("bg_menu.png");
    bg->getTexture()->setAliasTexParameters();
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, 0));
    
    Sprite *gameLogo = Sprite::createWithSpriteFrameName("game_logo.png");
    gameLogo->getTexture()->setAliasTexParameters();
    gameLogo->setAnchorPoint(ccp(0.5f, 0.5f));
    gameLogo->setPosition(ccp(80, 103));
    
    pressStartLabel = LabelBMFont::create("PRESS START", "whitefont.fnt", 160, kTextAlignmentCenter);
    pressStartLabel->setColor(gbLightestColor3);
    pressStartLabel->getTexture()->setAliasTexParameters();
    pressStartLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    pressStartLabel->setPosition(ccp(80, 30));
    
    startGameLabel = LabelBMFont::create("Start game", "whitefont.fnt", 160, kTextAlignmentCenter);
    startGameLabel->setColor(gbLightColor3);
    startGameLabel->getTexture()->setAliasTexParameters();
    startGameLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    startGameLabel->setPosition(ccp(80, 30));
    
    creditsLabel = LabelBMFont::create("Credits", "whitefont.fnt", 160, kTextAlignmentCenter);
    creditsLabel->setColor(gbLightColor3);
    creditsLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    creditsLabel->setPosition(ccp(80, 30));
    
    levelLabel = LabelBMFont::create("Level", "whitefont.fnt", 78, kTextAlignmentRight);
    levelLabel->setColor(gbLightColor3);
    levelLabel->setAnchorPoint(ccp(1.0f, 0.5f));
    levelLabel->setPosition(ccp(78, 30));
    
    levelNumberLabel = LabelBMFont::create("1", "whitefont.fnt", 78, kTextAlignmentLeft);
    levelNumberLabel->setColor(gbLightColor3);
    levelNumberLabel->setAnchorPoint(ccp(0.0f, 0.5f));
    levelNumberLabel->setPosition(ccp(82, 30));
    
    arrowUp = Sprite::createWithSpriteFrameName("arrowup.png");
    arrowUp->setAnchorPoint(ccp(0.5f, 0.0f));
    arrowUp->getTexture()->setAliasTexParameters();
    
    arrowDown = Sprite::createWithSpriteFrameName("arrowdown.png");
    arrowDown->setAnchorPoint(ccp(0.5f, 1.0f));
    
    mainBatchNode->addChild(arrowUp);
    mainBatchNode->addChild(arrowDown);
    
    this->addChild(bg);
    this->addChild(gameLogo);
    this->addChild(pressStartLabel);
    this->addChild(startGameLabel);
    this->addChild(creditsLabel);
    this->addChild(levelLabel);
    this->addChild(levelNumberLabel);
    this->addChild(mainBatchNode);
    
    optionIndex = 0;
    cursorIndex = 0;
    maxLevel = UserDefault::sharedUserDefault()->getIntegerForKey("MaxLevel", 1);
    if (maxLevel > kLastLevel) maxLevel = kLastLevel;
    selectedLevel = maxLevel;
    
    this->updateMenu();
    
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("song_title.wav", true);
    
    return true;
}

void TitleScreen::handleKey(int keyCode, bool pressed) {
    
    if (pressed) return;
    
    BaseLayer::handleKey(keyCode, pressed);
}

void TitleScreen::buttonUp(bool pressed) {
    
    if (optionIndex == 1) { // MODE SELECT
        cursorIndex--;
        if (cursorIndex < 0)
            cursorIndex = 0;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
    }
    
    else if (optionIndex == 2) { // LEVEL SELECT
        selectedLevel--;
        if (selectedLevel < 1)
            selectedLevel = 1;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
    }
    
    this->updateMenu();
    
}

void TitleScreen::buttonDown(bool pressed) {
    
    if (optionIndex == 1) { // MODE SELECT
        cursorIndex++;
        if (cursorIndex > 1)
            cursorIndex = 1;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
    }
    
    else if (optionIndex == 2) { // LEVEL SELECT
        selectedLevel++;
        if (selectedLevel > maxLevel)
            selectedLevel = maxLevel;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
    }
    
    this->updateMenu();
}

void TitleScreen::buttonA(bool pressed) {
    
    if (optionIndex > 0)
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_select.wav");
    
    if (optionIndex == 1) { // MODE SELECT
        
        if (cursorIndex == 0) { // Start Game
            optionIndex = 2;
            if (maxLevel == 1)
                this->buttonA(pressed);
        }
        else if (cursorIndex == 1) { // Credits
            Director::sharedDirector()->replaceScene(Credits::scene());
        }
        
    }
    
    else if (optionIndex == 2) { // LEVEL SELECT
        
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        
        if (selectedLevel == 1)
            Director::sharedDirector()->replaceScene(Intro::scene());
        else
            Director::sharedDirector()->replaceScene(Cave::sceneWithLevel(selectedLevel));
        
    }
    
    this->updateMenu();
    
}

void TitleScreen::buttonB(bool pressed) {
    
    if (optionIndex > 0)
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_back.wav");
    
    if (optionIndex == 1) { // PRESS START
        
        optionIndex = 0;
        cursorIndex = 0;
        
    } else if (optionIndex == 2) { // LEVEL SELECT
        
        optionIndex = 1;
        cursorIndex = 0;
        
    }
    
    this->updateMenu();
    
}

void TitleScreen::buttonStart(bool pressed) {
    
    if (optionIndex == 0) { // PRESS START
        
        optionIndex = 1;
        cursorIndex = 0;
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_select.wav");
        
        this->updateMenu();
    } else {
        this->buttonA(pressed);
    }
    
}

void TitleScreen::buttonSelect(bool pressed) {
    
    this->buttonA(pressed);
    
}

void TitleScreen::updateMenu() {
    
    pressStartLabel->stopAllActions();
    pressStartLabel->setVisible(false);
    startGameLabel->setVisible(false);
    creditsLabel->setVisible(false);
    levelLabel->setVisible(false);
    levelNumberLabel->setVisible(false);
    arrowUp->setVisible(false);
    arrowDown->setVisible(false);
    
    if (optionIndex == 0) { // PRESS START
        pressStartLabel->setVisible(true);
        pressStartLabel->runAction(RepeatForever::create(Sequence::create(
                                                                          DelayTime::create(3.0f),
                                                                          Hide::create(),
                                                                          DelayTime::create(0.3f),
                                                                          Show::create(),
                                                                          NULL
                                                                          )));
    }
    
    else if (optionIndex == 1) { // MODE SELECT
        
        arrowUp->setPosition(ccp(80, 34));
        arrowDown->setPosition(ccp(80, 26));
        
        if (cursorIndex == 0) {
            startGameLabel->setVisible(true);
            arrowDown->setVisible(true);
        } else {
            creditsLabel->setVisible(true);
            arrowUp->setVisible(true);
        }
        
    }
    
    else if (optionIndex == 2) { // LEVEL SELECT
        
        arrowUp->setPosition(ccp(91, 34));
        arrowDown->setPosition(ccp(91, 26));
        
        levelLabel->setVisible(true);
        levelNumberLabel->setVisible(true);
        
        if (selectedLevel > 1)
            arrowUp->setVisible(true);
        if (selectedLevel < maxLevel)
            arrowDown->setVisible(true);
        
    }
    
    levelNumberLabel->setCString(String::createWithFormat("%03d", selectedLevel)->getCString());
    
}