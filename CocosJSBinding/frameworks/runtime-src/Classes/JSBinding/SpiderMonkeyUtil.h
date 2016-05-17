//
//  SpiderMonkeyUtil.h
//  CocosJSBinding
//
//  Created by SuperSuRaccoon on 16/4/8.
//
//

#ifndef SpiderMonkeyUtil_h
#define SpiderMonkeyUtil_h

#include "ScriptingCore.h"


#define GLOBAL_OBJECT() ScriptingCore::getInstance()->getGlobalObject()
#define GLOBAL_CONTEXT() ScriptingCore::getInstance()->getGlobalContext()

#define FILL_JSCONTEXT_STACK_FRAME()\
JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();\
JSObject*obj =ScriptingCore::getInstance()->getGlobalObject();\
JSAutoCompartment ac(cx, obj);


namespace SpiderMonkey {
    
class Util {
    // public
public:
    static void test();
    static jsval getGlobalObjectJSValue();
    static void callGlobalFunction(const std::string& funcName);
    static void callGlobalFunction(const std::string& funcName, JS::RootedValue ret);
    static void callGlobalFunction(const std::string& funcName, jsval arg, JS::RootedValue ret);
    static void callGlobalFunction(const std::string& funcName, int count, jsval* args, JS::RootedValue ret);
    static void callJSObjectFunction(const std::string& funcName, jsval jsObject, JS::RootedValue ret);
    
    static void dumpGlobalTypeMap();
    static void dumpGlobalJSObject();
    static void dumpGlobalObject();
    static void dumpJSObject(JSObject* jsObject, int layer);
    
    
    static void createGlobalObject(const std::string& objectName);
    
    // type conversion
    static void to();
    
    // value
    static JS::HandleValue valueToHandleValue(JS::Value jsvValue);
    static JS::RootedValue valueToRootedValue(JS::Value jsvValue);
    
    static JS::HandleValue rootedValueToHandleValue(JS::RootedValue jsrValue);
    static JS::Value rootedValueToValue(JS::RootedValue jsrValue);
    
    static JS::RootedValue handleValueToRootedValue(JS::HandleValue jshValue);
    static JS::Value handleValueToValue(JS::HandleValue jshValue);
    
    // object
    static JS::HandleObject jsObjectToHandleObject(JSObject* jsoObject);
    static JS::RootedObject jsObjectToRootedObject(JSObject* jsoObject);

    static JS::HandleObject rootedObjectToHandleObject(JS::RootedObject jsrObject);
    static JSObject* rootedObjectToJSObject(JS::RootedObject jsrObject);

    static JS::RootedObject handleObjectToRootedObject(JS::HandleObject jshObject);
    static JSObject* handleObjectToJSObject(JS::HandleObject jshObject);
    
    // value == object
    static JSObject* valueToJSObject(JS::Value jsvValue);
    static JS::Value jsObjectToValue(JSObject* jsoObject);
    
    
    static const JSClass* jsObjectToClass(JSObject* jsoObject);
    
    // private
private:
    static std::string getIndent(int layer);
};

}


#endif /* SpiderMonkeyUtil_h */
