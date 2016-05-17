
var HelloWorldLayer = cc.Layer.extend({
    ctor:function () {
        this._super();
        var size = cc.director.getWinSize();
        cc.eventManager.addListener({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function(touch, event) {  
                Bridge.JS_CC.getInstance().test();
                return true;
            },
            onTouchMoved: function(touch, event) {},
            onTouchEnded: function(touch, event) {},
            onTouchCancelled: function(touch, event) {},
        }, this);
        return true;
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});
