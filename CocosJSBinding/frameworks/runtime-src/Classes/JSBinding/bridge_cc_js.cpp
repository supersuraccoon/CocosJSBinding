//
//  CC_JS.cpp
//
//  Created by SuperSuRaccoon on 14-8-12.
//
//

#include "bridge_cc_js.h"

using namespace Bridge;

static CC_JS *s_SharedInstance = nullptr;

CC_JS* CC_JS::getInstance()
{
    if (!s_SharedInstance) {
        s_SharedInstance = new CC_JS();
    }
    return s_SharedInstance;
}


CC_JS::CC_JS()
{
    
}

CC_JS::~CC_JS()
{
    
}

void CC_JS::init()
{
}








