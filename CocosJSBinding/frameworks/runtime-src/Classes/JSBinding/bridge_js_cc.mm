//
//  js_cc_bridge.cpp
//
//  Created by SuperSuRaccoon on 14-8-12.
//
//

#include "bridge_js_cc.h"
#include "bridge_cc_js.h"

#include "SpiderMonkeyUtil.h"

using namespace Bridge;

static Bridge::JS_CC *s_SharedInstance = nullptr;

JS_CC* JS_CC::getInstance()
{
    if (!s_SharedInstance) {
        s_SharedInstance = new JS_CC();
        s_SharedInstance->init();
    }
    return s_SharedInstance;
}


JS_CC::JS_CC()
{
    
}

JS_CC::~JS_CC()
{

}

bool JS_CC::init()
{
    return true;
}


//
bool JS_CC::test()
{
    CCLOG("JS_CC::test");
    SpiderMonkey::Util::callGlobalFunction("cc_js_test");
    SpiderMonkey::Util::createGlobalObject("XXX");
    SpiderMonkey::Util::callGlobalFunction("cc_js_test");
    
//    SpiderMonkey::Util::test();
//    SpiderMonkey::Util::dumpGlobalTypeMap();
    SpiderMonkey::Util::dumpGlobalObject();
//    SpiderMonkey::Util::dumpGlobalJSObject();
    
    return true;
}