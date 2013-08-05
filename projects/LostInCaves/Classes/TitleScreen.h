//
//  TitleScreen.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#ifndef __LostInCaves__TitleScreen__
#define __LostInCaves__TitleScreen__

#include "cocos2d.h"
#include "BaseLayer.h"

using namespace cocos2d;

class TitleScreen : public BaseLayer
{
public:
    
    ~TitleScreen();
    
    virtual bool init();
    
    static Scene* scene();
    
    CREATE_FUNC(TitleScreen);
    
    virtual void handleKey(int keyCode, bool pressed);
    
    virtual void buttonUp(bool pressed);
    virtual void buttonDown(bool pressed);
    virtual void buttonA(bool pressed);
    virtual void buttonB(bool pressed);
    virtual void buttonStart(bool pressed);
    virtual void buttonSelect(bool pressed);
    
    LabelBMFont *pressStartLabel;
    
    LabelBMFont *startGameLabel;
    LabelBMFont *creditsLabel;
    LabelBMFont *levelLabel;
    LabelBMFont *levelNumberLabel;
    Sprite *arrowUp;
    Sprite *arrowDown;
    
    int optionIndex;
    int cursorIndex;
    
    int maxLevel;
    int selectedLevel;
    
    void updateMenu();
    
};

#endif /* defined(__LostInCaves__TitleScreen__) */
