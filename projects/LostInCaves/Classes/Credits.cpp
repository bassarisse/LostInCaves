//
//  Credits.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "Credits.h"
#include "TitleScreen.h"
#include "SimpleAudioEngine.h"
#include "Common.h"

using namespace CocosDenshion;

Credits::~Credits() {
    
}

Scene* Credits::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    Credits *layer = Credits::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool Credits::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_back.wav");
    
    LabelBMFont *label1 = LabelBMFont::create("LOST IN CAVES", "whitefont.fnt", 150, kTextAlignmentLeft);
    label1->setColor(gbLightestColor3);
    label1->setAnchorPoint(ccp(0, 1));
    label1->setPosition(ccp(5, 140));
    label1->getTexture()->setAliasTexParameters();
    
    LabelBMFont *label2 = LabelBMFont::create("Made for", "miniwhitefont.fnt", 150, kTextAlignmentRight);
    label2->setColor(gbLightColor3);
    label2->setAnchorPoint(ccp(1, 1));
    label2->setPosition(ccp(155, 116));
    label2->getTexture()->setAliasTexParameters();
    
    LabelBMFont *label3 = LabelBMFont::create("GameBoy JAM", "whitefont.fnt", 150, kTextAlignmentRight);
    label3->setColor(gbLightestColor3);
    label3->setAnchorPoint(ccp(1, 1));
    label3->setPosition(ccp(155, 109));
    
    LabelBMFont *label4 = LabelBMFont::create("Programming, Graphics", "miniwhitefont.fnt", 150, kTextAlignmentRight);
    label4->setColor(gbLightColor3);
    label4->setAnchorPoint(ccp(1, 1));
    label4->setPosition(ccp(155, 90));
    
    LabelBMFont *label5 = LabelBMFont::create("SFX, Game Design", "miniwhitefont.fnt", 150, kTextAlignmentRight);
    label5->setColor(gbLightColor3);
    label5->setAnchorPoint(ccp(1, 1));
    label5->setPosition(ccp(155, 82));
    
    LabelBMFont *label6 = LabelBMFont::create("Bruno Assarisse", "whitefont.fnt", 150, kTextAlignmentRight);
    label6->setColor(gbLightestColor3);
    label6->setAnchorPoint(ccp(1, 1));
    label6->setPosition(ccp(155, 75));
    
    LabelBMFont *label7 = LabelBMFont::create("Music", "miniwhitefont.fnt", 150, kTextAlignmentRight);
    label7->setColor(gbLightColor3);
    label7->setAnchorPoint(ccp(1, 1));
    label7->setPosition(ccp(155, 56));
    
    LabelBMFont *label8 = LabelBMFont::create("Felipe Oriani", "whitefont.fnt", 150, kTextAlignmentRight);
    label8->setColor(gbLightestColor3);
    label8->setAnchorPoint(ccp(1, 1));
    label8->setPosition(ccp(155, 49));
    
    LabelBMFont *label9 = LabelBMFont::create("Special Thanks", "miniwhitefont.fnt", 150, kTextAlignmentRight);
    label9->setColor(gbLightColor3);
    label9->setAnchorPoint(ccp(1, 1));
    label9->setPosition(ccp(155, 30));
    
    LabelBMFont *label10 = LabelBMFont::create("Caroline Rodolfo <3", "whitefont.fnt", 150, kTextAlignmentRight);
    label10->setColor(gbLightestColor3);
    label10->setAnchorPoint(ccp(1, 1));
    label10->setPosition(ccp(155, 23));
    
    LabelBMFont *label11 = LabelBMFont::create("Murilo Clemente", "whitefont.fnt", 150, kTextAlignmentRight);
    label11->setColor(gbLightestColor3);
    label11->setAnchorPoint(ccp(1, 1));
    label11->setPosition(ccp(155, 13));
    
    Sprite *bg = Sprite::create("bg_menu.png");
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, 0));
    bg->getTexture()->setAliasTexParameters();
    
    this->addChild(bg);
    this->addChild(label1);
    this->addChild(label2);
    this->addChild(label3);
    this->addChild(label4);
    this->addChild(label5);
    this->addChild(label6);
    this->addChild(label7);
    this->addChild(label8);
    this->addChild(label9);
    this->addChild(label10);
    this->addChild(label11);
    
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("song_title.wav", true);
    
    return true;
}

void Credits::buttonB(bool pressed) {
    if (pressed) return;
    SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_back.wav");
    Director::sharedDirector()->replaceScene(TitleScreen::scene());
}