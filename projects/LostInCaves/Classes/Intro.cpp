//
//  Intro.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "Intro.h"
#include "Cave.h"

Intro::~Intro() {
    
}

Scene* Intro::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    Intro *layer = Intro::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool Intro::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    LabelBMFont *skipLabel = LabelBMFont::create("Press Start to skip", "miniwhitefont.fnt", 160, kTextAlignmentRight);
    skipLabel->setColor(gbDarkColor3);
    skipLabel->setAnchorPoint(ccp(1, 0));
    skipLabel->setPosition(ccp(157, 0));
    skipLabel->getTexture()->setAliasTexParameters();
    
    LabelBMFont *phrase1Label = LabelBMFont::create("I should not have departed from the group.", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase1Label->setColor(gbDarkestColor3);
    phrase1Label->setAnchorPoint(ccp(0, 1));
    phrase1Label->setPosition(ccp(5, 140));
    phrase1Label->getTexture()->setAliasTexParameters();
    
    LabelBMFont *phrase2Label = LabelBMFont::create("What a strange cave!", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase2Label->setColor(gbDarkestColor3);
    phrase2Label->setAnchorPoint(ccp(0, 1));
    phrase2Label->setPosition(ccp(5, 107));
    
    LabelBMFont *phrase3Label = LabelBMFont::create("Even without my flashlight, I can see clearly in here, but...", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase3Label->setColor(gbDarkestColor3);
    phrase3Label->setAnchorPoint(ccp(0, 1));
    phrase3Label->setPosition(ccp(5, 86));
    
    LabelBMFont *phrase4Label = LabelBMFont::create("Where am I?", "whitefont.fnt", 150, kTextAlignmentLeft);
    phrase4Label->setColor(gbDarkestColor3);
    phrase4Label->setAnchorPoint(ccp(0, 1));
    phrase4Label->setPosition(ccp(5, 41));
    
    LayerColor *bg = LayerColor::create(gbDarkestColor);
    
    this->addChild(bg);
    this->addChild(skipLabel);
    this->addChild(phrase1Label);
    this->addChild(phrase2Label);
    this->addChild(phrase3Label);
    this->addChild(phrase4Label);
    
    labelAnimationFinishCount = 0;
    
    this->animateLabel(phrase1Label, 1);
    this->animateLabel(phrase2Label, 3);
    this->animateLabel(phrase3Label, 5);
    this->animateLabel(phrase4Label, 7);
    
    return true;
}

void Intro::buttonStart(bool pressed) {
    if (pressed) return;
    Director::sharedDirector()->replaceScene(Cave::sceneWithLevel(1));
}

void Intro::animateLabel(Node *node, float delay) {
    
    float colorDelay = 0.25f;
    
    Action *action = Sequence::create(
                                      DelayTime::create(delay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbLightestColor3.r, gbLightestColor3.g, gbLightestColor3.b),
                                      DelayTime::create(12.0f - delay),
                                      TintTo::create(0, gbLightColor3.r, gbLightColor3.g, gbLightColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkColor3.r, gbDarkColor3.g, gbDarkColor3.b),
                                      DelayTime::create(colorDelay),
                                      TintTo::create(0, gbDarkestColor3.r, gbDarkestColor3.g, gbDarkestColor3.b),
                                      DelayTime::create(colorDelay),
                                      CallFunc::create(this, callfunc_selector(Intro::finishLabelAnimation)),
                                      NULL);
    
    node->runAction(action);
}

void Intro::finishLabelAnimation() {
    labelAnimationFinishCount++;
    if (labelAnimationFinishCount >= 4)
        this->buttonStart(false);
}