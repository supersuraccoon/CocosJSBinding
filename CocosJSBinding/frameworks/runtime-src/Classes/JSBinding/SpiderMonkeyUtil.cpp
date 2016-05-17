//
//  SpiderMonkeyUtil.cpp
//  CocosJSBinding
//
//  Created by SuperSuRaccoon on 16/4/8.
//
//

#include "SpiderMonkeyUtil.h"
#include "cocos2d.h"
#include "cocos2d_specifics.hpp"

using namespace SpiderMonkey;


void hexDump (std::string desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char *)addr;
    printf ("%s:\n", desc.c_str());

    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf ("  %s\n", buff);
            printf ("  %04x ", i);
        }
        printf (" %02x", pc[i]);
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }
    printf ("  %s\n", buff);
}

void Util::test()
{
    double d1 = 234.5;
    char s1[] = "a 15char string";
    char s2[] = "This is a slightly longer string";
    hexDump ("d1", &d1, sizeof d1);
    hexDump ("s1", &s1, sizeof s1);
    hexDump ("s2", &s2, sizeof s2);
    
    cocos2d::Scene* scene = cocos2d::Director::getInstance()->getRunningScene();
    hexDump ("cocos2d::Scene", &scene, sizeof scene);
    std::string description = scene->getDescription();
    const char* s3 = description.c_str();
    hexDump ("cocos2d::Scene::description", &s3, description.size());
}


void Util::callGlobalFunction(const std::string& funcName)
{
    JS::RootedValue ret(GLOBAL_CONTEXT());
    ScriptingCore::getInstance()->executeFunctionWithOwner(getGlobalObjectJSValue(), funcName.c_str(), 0, NULL, &ret);
}

void Util::callGlobalFunction(const std::string& funcName, JS::RootedValue ret)
{
    ScriptingCore::getInstance()->executeFunctionWithOwner(getGlobalObjectJSValue(), funcName.c_str(), 0, NULL, &ret);
}

void Util::callGlobalFunction(const std::string& funcName, jsval arg, JS::RootedValue ret)
{
    ScriptingCore::getInstance()->executeFunctionWithOwner(getGlobalObjectJSValue(), funcName.c_str(), 1, &arg, &ret);
}

void Util::callGlobalFunction(const std::string& funcName, int count, jsval* args, JS::RootedValue ret)
{
    ScriptingCore::getInstance()->executeFunctionWithOwner(getGlobalObjectJSValue(), funcName.c_str(), count, args, &ret);
}

void Util::callJSObjectFunction(const std::string& funcName, jsval jsObject, JS::RootedValue ret)
{
    ScriptingCore::getInstance()->executeFunctionWithOwner(jsObject, funcName.c_str(), 0, NULL, &ret);
}

jsval Util::getGlobalObjectJSValue()
{
    return OBJECT_TO_JSVAL(GLOBAL_OBJECT());
}


std::string Util::getIndent(int layer)
{
    std::string indent = "";
    for (int i = 0; i < layer; i ++) {
        indent += "\t";
    }
    return indent;
}

void Util::dumpJSObject(JSObject* jsoTargetObject, int layer)
{
    JS::RootedObject jsrTargetObject(GLOBAL_CONTEXT(), jsoTargetObject);
    JS::RootedObject jsrPropertyIterator(GLOBAL_CONTEXT(), JS_NewPropertyIterator(GLOBAL_CONTEXT(), jsrTargetObject));
    
    while (true) {
        JS::RootedId jsrIdp(GLOBAL_CONTEXT());
        JS::RootedValue jsrKey(GLOBAL_CONTEXT());
        
        if (!JS_NextProperty(GLOBAL_CONTEXT(), jsrPropertyIterator, jsrIdp.address()) ||
            !JS_IdToValue(GLOBAL_CONTEXT(), jsrIdp, &jsrKey)) {
            CCLOG("%sJS_NextProperty || JS_IdToValue failed !!!\n\n", getIndent(layer).c_str());
            break;
        }
        if (jsrKey.isNullOrUndefined()) {
            CCLOG("%s------------ End ------------\n", getIndent(layer - 1).c_str());
            break;
        }
        if (jsrKey.isInt32()) {
            JS::RootedValue value(GLOBAL_CONTEXT());
            JS_GetPropertyById(GLOBAL_CONTEXT(), jsrTargetObject, jsrIdp, &value);
            CCLOG("%sisInt32 Key: %d\t(value: %d)", getIndent(layer).c_str(), jsrKey.toInt32(), value.toInt32());
        }
        else if (jsrKey.isDouble()) {
            CCLOG("%sisDouble Key: %f", getIndent(layer).c_str(), jsrKey.toDouble());
        }
        else if (jsrKey.isString()) {
            JSStringWrapper keyWapper(jsrKey.toString(), GLOBAL_CONTEXT());
            CCLOG("%s==> %s", getIndent(layer).c_str(), keyWapper.get());

            JS::RootedValue value(GLOBAL_CONTEXT());
            JS_GetPropertyById(GLOBAL_CONTEXT(), jsrTargetObject, jsrIdp, &value);
            
            if (value.isObject()) {
                // check JS_IsGlobalObject to filter the "window" object
                if (JS_IsGlobalObject(&value.toObject())) {
                    CCLOG("%s==> JS_IsGlobalObject SKIPPED !!!", getIndent(layer).c_str());
                    continue;
                }
                if (JS_IsArrayObject(GLOBAL_CONTEXT(), value)) {
                    CCLOG("%s    [Array]", getIndent(layer).c_str());
                    
                    JS::RootedObject jsrObjectTemp(GLOBAL_CONTEXT(), &value.toObject());
                    uint32_t length;
                    JS_GetArrayLength(GLOBAL_CONTEXT(), jsrObjectTemp, &length);
                    for( uint32_t i = 0; i < length; i ++) {
                        JS::RootedValue valarg(GLOBAL_CONTEXT());
                        JS_GetElement(GLOBAL_CONTEXT(), jsrObjectTemp, i, &valarg);
                        
                        if (valarg.isInt32()) {
                            CCLOG("%s    [Int32]\t(value: %d)", getIndent(layer).c_str(), valarg.toInt32());
                        }
                        else if (valarg.isDouble()) {
                            CCLOG("%s    [Double]\t(value: %f)", getIndent(layer).c_str(), valarg.toDouble());
                        }
                        if (valarg.isBoolean()) {
                            CCLOG("%s    [Boolean]\t(value: %d)", getIndent(layer).c_str(), valarg.toBoolean());
                        }
                        if (valarg.isString()) {
                            JSStringWrapper jsstring(valarg, GLOBAL_CONTEXT());
                            CCLOG("%s    [String]\t(value: %s)", getIndent(layer).c_str(), jsstring.get());
                        }
                        if (valarg.isObject()) {
                            CCLOG("%s    [Object]", getIndent(layer).c_str());
                        }
                    }
                    CCLOG("");
                }
                else if (JS_ObjectIsFunction(GLOBAL_CONTEXT(), &value.toObject())) {
                    JSFunction* jsFunc = JS_ValueToFunction(GLOBAL_CONTEXT(), value);
                    JSString* functionDisplayIdJSString = JS_GetFunctionDisplayId(jsFunc);
                    JSStringWrapper functionDisplayIdWapper;
                    JSStringWrapper functionIdWapper;
                    if (functionDisplayIdJSString != NULL) {
                        functionDisplayIdWapper.set(functionDisplayIdJSString, GLOBAL_CONTEXT());
                    }
                    JSString* functionIdJSString = JS_GetFunctionId(jsFunc);
                    if (functionIdJSString != NULL) {
                        functionIdWapper.set(functionIdJSString, GLOBAL_CONTEXT());
                    }
                    
                    CCLOG("%s    [Function]\t(DisplayId: %s)\t(FunctionId: %s)",
                          getIndent(layer).c_str(),
                          (functionDisplayIdJSString == NULL ? "NULL" : functionDisplayIdWapper.get()),
                          (functionIdJSString == NULL ? "NULL" : functionIdWapper.get())
                    );
                    
                    dumpJSObject(&value.toObject(), layer + 1);
                }
                else {
                    dumpJSObject(&value.toObject(), layer + 1);
                }
            }
            else if (value.isBoolean()) {
                CCLOG("%s[Boolean]\t(value: %d)", getIndent(layer).c_str(), value.toBoolean());
            }
            else if (value.isDouble()) {
                CCLOG("%s[Double]\t(value: %f)", getIndent(layer).c_str(), value.toDouble());
            }
            else if (value.isInt32()) {
                CCLOG("%s[Int32]\t(value: %d)", getIndent(layer).c_str(), value.toInt32());
            }
            else if (value.isString()) {
                JSStringWrapper jsstring(value, GLOBAL_CONTEXT());
                CCLOG("%s[String]\t(value: %s)", getIndent(layer).c_str(), jsstring.get());
            }
            else if (value.isNullOrUndefined()) {
                CCLOG("%s[Null/Undefined]", getIndent(layer).c_str());
            }
            else {
                CCLOG("%sis??????", getIndent(layer).c_str());
            }
        }
        else {
            CCLOG("%sInvalid Key !!!", getIndent(layer).c_str());
        }
    }
}

void Util::dumpGlobalObject()
{
    CCLOG("==> Global");
    dumpJSObject(GLOBAL_OBJECT(), 1);
}


void Util::dumpGlobalTypeMap()
{
    for (auto iter = _js_global_type_map.begin(); iter != _js_global_type_map.end(); ++iter)
    {
        CCLOG("%s", iter->first.c_str());
    }
}


void Util::dumpGlobalJSObject()
{
    std::unordered_map<JSObject*, js_proxy_t*> js_native = list_js_native_();
    CCLOG("dumpGlobalJSObject size: %lu", js_native.size());
    for (auto iter = js_native.begin(); iter != js_native.end(); ++iter) {
        CCLOG("GlobalJSObject: %s", JS_GetClass(iter->first)->name);
        dumpJSObject(iter->first, 1);
        
        JS::RootedObject jsrTargetObject(GLOBAL_CONTEXT(), iter->first);
        JS::RootedObject protop(GLOBAL_CONTEXT());
        
        if (JS_GetPrototype(GLOBAL_CONTEXT(), jsrTargetObject, &protop)) {
            CCLOG("===>: %s", JS_GetClass(protop)->name);
            dumpJSObject(protop.get(), 1);
        }
    }
    
    
    std::unordered_map<void*, js_proxy_t*> native_js = list_native_js_();
    CCLOG("dumpGlobalNativeObject size: %lu", native_js.size());
//    for (auto iter = js_native.begin(); iter != js_native.end(); ++iter) {
//        CCLOG("GlobalJSObject: %s", JS_GetClass(iter->first)->name);
//        dumpJSObject(iter->first, 1);
//        
//        JS::RootedObject jsrTargetObject(GLOBAL_CONTEXT(), iter->first);
//        JS::RootedObject protop(GLOBAL_CONTEXT());
//        
//        if (JS_GetPrototype(GLOBAL_CONTEXT(), jsrTargetObject, &protop)) {
//            CCLOG("===>: %s", JS_GetClass(protop)->name);
//            dumpJSObject(protop.get(), 1);
//        }
//    }
}


void Util::createGlobalObject(const std::string& objectName)
{
    JS::RootedValue jsrPropertyValue(GLOBAL_CONTEXT());
    JS::RootedObject jsrNewObject(GLOBAL_CONTEXT());
    JS::RootedObject jsrGlobalObject(GLOBAL_CONTEXT(), GLOBAL_OBJECT());
    
    JS_GetProperty(GLOBAL_CONTEXT(), jsrGlobalObject, objectName.c_str(), &jsrPropertyValue);
    if (jsrPropertyValue.isNullOrUndefined()) {
        jsrNewObject.set(JS_NewObject(GLOBAL_CONTEXT(), NULL, JS::NullPtr(), JS::NullPtr()));
        jsrPropertyValue = OBJECT_TO_JSVAL(jsrNewObject);
        JS_SetProperty(GLOBAL_CONTEXT(), jsrGlobalObject, objectName.c_str(), jsrPropertyValue);
    }
}


// conversion
void Util::to()
{
}

/*
 ### JS::Rooted:
 - Local variable value is always rooted.
 - Used for local variables
 - Used for non-rooted values being passed to a function that requires a handle
 
 
 ### JS::Handle:
 - Variable value that has been rooted elsewhere. 
 - Useful as a parameter type
 - Guarantees the lvalue is properly rooted.

 ### JS::Value:
 - The interface for a single JavaScript Engine value.
 
 
 ### jsval
 - The same type as JS::Value but old.
 - For backwards compatibility
 
 
 ###
 - Handle is a const reference to a Rooted.
 - MutableHandle is a non-const reference to Rooted.
 
 Rooted<T> ----> Handle<T>
    |               ^
    |               |
    |               |
    +---> MutableHandle<T>
    (via &)
 
 */


JS::HandleValue Util::valueToHandleValue(JS::Value jsvValue)
{
    /// use JS::RootedValue here
    return JS::RootedValue(GLOBAL_CONTEXT(), jsvValue);
}
JS::RootedValue Util::valueToRootedValue(JS::Value jsvValue)
{
    // never use this
    // return JS::RootedValue(GLOBAL_CONTEXT(), jsvValue);
}

JS::HandleValue Util::rootedValueToHandleValue(JS::RootedValue jsrValue)
{
    return jsrValue;
}
JS::Value Util::rootedValueToValue(JS::RootedValue jsrValue)
{
    return jsrValue.get();
}

JS::RootedValue Util::handleValueToRootedValue(JS::HandleValue jshValue)
{
    // nerver use this
}
JS::Value Util::handleValueToValue(JS::HandleValue jshValue)
{
    return jshValue.get();
}


JS::HandleObject Util::jsObjectToHandleObject(JSObject* jsoObject)
{
    return JS::RootedObject(GLOBAL_CONTEXT(), jsoObject);
}

JS::RootedObject Util::jsObjectToRootedObject(JSObject* jsoObject)
{
    // never use this
}

JS::HandleObject Util::rootedObjectToHandleObject(JS::RootedObject jsrObject)
{
    return jsrObject;
}

JSObject* Util::rootedObjectToJSObject(JS::RootedObject jsrObject)
{
    return jsrObject.get();
}

JS::RootedObject Util::handleObjectToRootedObject(JS::HandleObject jshObject)
{
    // never use this
}
JSObject* Util::handleObjectToJSObject(JS::HandleObject jshObject)
{
    return jshObject.get();
}

JSObject* Util::valueToJSObject(JS::Value jsvValue)
{
    return jsvValue.toObjectOrNull();
}

JS::Value Util::jsObjectToValue(JSObject* jsoObject)
{
    // never use this
}

const JSClass* Util::jsObjectToClass(JSObject* jsoObject)
{
    return JS_GetClass(jsoObject);
}





