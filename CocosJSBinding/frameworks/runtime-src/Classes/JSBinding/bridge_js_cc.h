//
//  bridge_js_cc.h
//
//  Created by SuperSuRaccoon on 14-8-12.
//
//

#ifndef __js_cc__
#define __js_cc__

#include "cocos2d.h"

namespace Bridge {

    class JS_CC : public cocos2d::Ref {
    public:
        JS_CC();
        ~JS_CC();
        
        static JS_CC* getInstance();
        virtual bool init();
        
        //
        bool test();
    };
}

#endif /* defined(__js_cc__) */
