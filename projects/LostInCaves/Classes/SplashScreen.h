//
//  SplashScreen.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#ifndef __LostInCaves__SplashScreen__
#define __LostInCaves__SplashScreen__

#include "cocos2d.h"
#include "BaseLayer.h"

using namespace cocos2d;

class SplashScreen : public BaseLayer
{
public:
    
    ~SplashScreen();
    
    virtual bool init();
    
    static Scene* scene();
    
    CREATE_FUNC(SplashScreen);
    
    void goToTitleScreen();
    
};

#endif /* defined(__LostInCaves__SplashScreen__) */
