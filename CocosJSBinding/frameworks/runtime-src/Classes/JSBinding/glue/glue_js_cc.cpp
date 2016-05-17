//
//  glue_js_cc.cpp
//
//  Created by SuperSuRaccoon on 14-8-12.
//
//

#include "glue_js_cc.h"
#include "cocos2d_specifics.hpp"

#include "bridge_js_cc.h"

JSClass* bridge_js_cc_class;
JSObject* bridge_js_cc_prototype;

bool bridge_js_cc_constructor(JSContext* cx, uint32_t argc, jsval* vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Bridge::JS_CC* cobj = new Bridge::JS_CC();
    cocos2d::Ref* ccobj = dynamic_cast<cocos2d::Ref*>(cobj);
    if (ccobj) {
        ccobj->autorelease();
    }
    TypeTest<Bridge::JS_CC> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    
    JS::RootedObject proto(cx, typeClass->proto.ref());
    JS::RootedObject parent(cx, typeClass->parentProto.ref());
    JS::RootedObject obj(cx, JS_NewObject(cx, typeClass->jsclass, proto, parent));
    args.rval().set(OBJECT_TO_JSVAL(obj));
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    AddNamedObjectRoot(cx, &p->obj, "Bridge::JS_CC");
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    return true;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}

void bridge_js_cc_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOG("JSBindings: finallizing JS object %p JSB", obj);
}

//
bool bridge_js_cc_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    Bridge::JS_CC* ret = Bridge::JS_CC::getInstance();
    jsval jsret = JSVAL_NULL;
    do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<Bridge::JS_CC>(cx, (Bridge::JS_CC*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        }
        else {
            jsret = JSVAL_NULL;
        }
    } while (0);
    args.rval().set(jsret);
    return true;
}

bool bridge_js_cc_test(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
    Bridge::JS_CC::getInstance()->test();
    argv.rval().setUndefined();
    return true;
}


// register
void register_all_glue_js_cc(JSContext* cx, JS::HandleObject obj)
{
    // namespace
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "Bridge", &ns);

    // class
    bridge_js_cc_class = (JSClass *)calloc(1, sizeof(JSClass));
    bridge_js_cc_class->name = "JS_CC";
    bridge_js_cc_class->addProperty = JS_PropertyStub;
    bridge_js_cc_class->delProperty = JS_DeletePropertyStub;
    bridge_js_cc_class->getProperty = JS_PropertyStub;
    bridge_js_cc_class->setProperty = JS_StrictPropertyStub;
    bridge_js_cc_class->enumerate = JS_EnumerateStub;
    bridge_js_cc_class->resolve = JS_ResolveStub;
    bridge_js_cc_class->convert = JS_ConvertStub;
    bridge_js_cc_class->finalize = bridge_js_cc_finalize;
    bridge_js_cc_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);
    
    static JSPropertySpec properties[] = {
        JS_PSG("__nativeObj", js_is_native_obj, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };
    
    static JSFunctionSpec funcs[] = {
        JS_FN("test", bridge_js_cc_test, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", bridge_js_cc_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    bridge_js_cc_prototype = JS_InitClass(cx, ns,
                                        JS::NullPtr(), // parent proto
                                        bridge_js_cc_class,
                                        bridge_js_cc_constructor, 0, // constructor
                                        properties,
                                        funcs,
                                        NULL, // no static properties
                                        st_funcs);
    
    JS::RootedObject proto(cx, bridge_js_cc_prototype);
    jsb_register_class<Bridge::JS_CC>(cx, bridge_js_cc_class, proto, JS::NullPtr());
}


