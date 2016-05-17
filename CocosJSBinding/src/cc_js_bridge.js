
function cc_js_test() {
    cc.log("cc_js_test");
	try {
		cc.log(XXX);
    	cc.log(XXX.Sprite);
    } catch (e) {
    	cc.log(e);
    }
};

var FunctionObject = function () {
    this.aaa = 1;
    this.bbb = 2;
    this.ccc = 3;
};

var ppp = new FunctionObject();