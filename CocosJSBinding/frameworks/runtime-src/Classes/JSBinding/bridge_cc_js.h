//
//  bridge_js_cc.h
//
//  Created by SuperSuRaccoon on 14-8-12.
//
//

#ifndef __CC_JS__
#define __CC_JS__

namespace Bridge {
    
    class CC_JS {
    public:
        CC_JS();
        ~CC_JS();
        
        static CC_JS* getInstance();
        void init();
    private:
        
    };
}

#endif /* defined(__CC_JS__) */
