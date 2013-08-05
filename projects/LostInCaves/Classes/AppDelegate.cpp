#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "SplashScreen.h"

//#include "Cave.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    Director* pDirector = Director::sharedDirector();
    EGLView* pEGLView = EGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    pDirector->setProjection(kDirectorProjection2D);
	
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    //pDirector->setAnimationInterval(1.0 / 30);

    // create a scene. it's an autorelease object
    Scene *pScene = SplashScreen::scene();
    //Scene *pScene = Cave::sceneWithLevel(kLastLevel);

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
