
// C++ includes
#include <map>
#include <functional>
#include <string>

// test inclues
#include "controller.h"
#include "testResource.h"
#include "tests.h"


struct {
	const char *test_name;
	std::function<TestScene*()> callback;
} g_aTestNames[] = {

	{ "Accelerometer", []() { return new AccelerometerTestScene(); } },
	{ "ActionManagerTest", [](){return new ActionManagerTestScene(); } },
	{ "ActionsEaseTest", [](){return new ActionsEaseTestScene();} },
	{ "ActionsProgressTest", [](){return new ProgressActionsTestScene(); } },
	{ "ActionsTest", [](){ return new ActionsTestScene(); } },
	{ "Box2dTest", []() { return new Box2DTestScene(); } },
	{ "Box2dTestBed", []() { return new Box2dTestBedScene(); } },
	{ "BugsTest", []() { return new BugsTestScene(); } },
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
	{ "ChipmunkTest", []() { return new ChipmunkAccelTouchTestScene(); } },
#endif
	{ "ClickAndMoveTest", [](){return new ClickAndMoveTestScene(); } },
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
	{ "ClippingNodeTest", []() { return new ClippingNodeTestScene(); } },
#endif
	{ "CocosDenshionTest", []() { return new CocosDenshionTestScene(); } },
	{ "ConfigurationTest", []() { return new ConfigurationTestScene(); } },
#if (CC_TARGET_PLATFORM != CC_PLATFORM_EMSCRIPTEN)
#if (CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
	{ "CurlTest", []() { return new CurlTestScene(); } },
#endif
#endif
#endif
#endif
	{ "CurrentLanguageTest", []() { return new CurrentLanguageTestScene(); } },
	{ "DrawPrimitivesTest", [](){return new DrawPrimitivesTestScene();} },
	{ "EffectAdvancedTest", []() { return new EffectAdvanceScene(); } },
	{ "EffectsTest", [](){return new EffectTestScene();} },
	{ "ExtensionsTest", []() { return new ExtensionsTestScene(); } },
	{ "FileUtilsTest", []() { return new FileUtilsTestScene(); } },
	{ "FontTest", []() { return new FontTestScene(); } },
	{ "IntervalTest", [](){return new IntervalTestScene(); } },
#ifdef KEYBOARD_SUPPORT
	{ "KeyboardTest", []() { return new KeyboardTestScene(); } },
#endif
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
	{ "KeypadTest", []() { return new KeypadTestScene(); } },
#endif
	{ "LabelTest", [](){return new AtlasTestScene(); } },
	{ "LayerTest", [](){return new LayerTestScene();} },
	{ "MenuTest", [](){return new MenuTestScene();} },
	{ "MotionStreakTest", [](){return new MotionStreakTestScene();} },
	{ "MutiTouchTest", []() { return new MutiTouchTestScene(); } },
	{ "NodeTest", [](){return new CocosNodeTestScene();} },
	{ "ParallaxTest", [](){return new ParallaxTestScene(); } },
	{ "ParticleTest", [](){return new ParticleTestScene(); } },
	{ "PerformanceTest", []() { return new PerformanceTestScene(); } },
	{ "RenderTextureTest", [](){return new RenderTextureScene(); } },
	{ "RotateWorldTest", [](){return new RotateWorldTestScene(); } },
	{ "SceneTest", [](){return new SceneTestScene();} },
	{ "SchedulerTest", [](){return new SchedulerTestScene(); } },
	{ "ShaderTest", []() { return new ShaderTestScene(); } },
	{ "SpineTest", []() { return new SpineTestScene(); } },
	{ "SpriteTest", [](){return new SpriteTestScene(); } },
	{ "TextInputTest", [](){return new TextInputTestScene(); } },
	{ "Texture2DTest", [](){return new TextureTestScene(); } },
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
	{ "TextureCacheTest", []() { return new TextureCacheTestScene(); } },
#endif
	{ "TexturePackerEncryption", []() { return new TextureAtlasEncryptionTestScene(); } },
	{ "TileMapTest", [](){return new TileMapTestScene(); } },
	{ "TouchesTest", [](){return new PongScene();} },
	{ "TransitionsTest", [](){return new TransitionsTestScene();} },
	{ "UserDefaultTest", []() { return new UserDefaultTestScene(); } },
	{ "ZwoptexTest", []() { return new ZwoptexTestScene(); } },
};

static int g_testCount = sizeof(g_aTestNames) / sizeof(g_aTestNames[0]);

#define LINE_SPACE          40

static Point s_tCurPos = PointZero;

TestController::TestController()
: _beginPos(PointZero)
{
    // add close menu
    MenuItemImage *pCloseItem = MenuItemImage::create(s_pPathClose, s_pPathClose, CC_CALLBACK_1(TestController::closeCallback, this) );
    Menu* pMenu =Menu::create(pCloseItem, NULL);

    pMenu->setPosition( PointZero );
    pCloseItem->setPosition(ccp( VisibleRect::right().x - 30, VisibleRect::top().y - 30));

    // add menu items for tests
    _itemMenu = Menu::create();
    for (int i = 0; i < g_testCount; ++i)
    {
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
//         LabelBMFont* label = LabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
// #else
        LabelTTF* label = LabelTTF::create( g_aTestNames[i].test_name, "Arial", 24);
// #endif        
        MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(TestController::menuCallback, this));

        _itemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( VisibleRect::center().x, (VisibleRect::top().y - (i + 1) * LINE_SPACE) ));
    }

    _itemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (g_testCount + 1) * (LINE_SPACE)));
    _itemMenu->setPosition(s_tCurPos);
    addChild(_itemMenu);

    setTouchEnabled(true);

    addChild(pMenu, 1);

}

TestController::~TestController()
{
}

void TestController::menuCallback(Object * pSender)
{

	Director::sharedDirector()->purgeCachedData();

    // get the userdata, it's the index of the menu item clicked
    MenuItem* pMenuItem = (MenuItem *)(pSender);
    int idx = pMenuItem->getZOrder() - 10000;

    // create the test scene and run it
    TestScene* pScene = g_aTestNames[idx].callback();

    if (pScene)
    {
        pScene->runThisTest();
        pScene->release();
    }
}

void TestController::closeCallback(Object * pSender)
{
    Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TestController::ccTouchesBegan(Set *pTouches, Event *pEvent)
{
    Touch* touch = (Touch*)pTouches->anyObject();

    _beginPos = touch->getLocation();    
}

void TestController::ccTouchesMoved(Set *pTouches, Event *pEvent)
{
    Touch* touch = (Touch*)pTouches->anyObject();

    Point touchLocation = touch->getLocation();    
    float nMoveY = touchLocation.y - _beginPos.y;

    Point curPos  = _itemMenu->getPosition();
    Point nextPos = ccp(curPos.x, curPos.y + nMoveY);

    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(PointZero);
        return;
    }

    if (nextPos.y > ((g_testCount + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        _itemMenu->setPosition(ccp(0, ((g_testCount + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }

    _itemMenu->setPosition(nextPos);
    _beginPos = touchLocation;
    s_tCurPos   = nextPos;
}
