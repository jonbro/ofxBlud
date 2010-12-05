#include "testApp.h"

#define method(class, name) {#name, &class::name}

class bludImage {
	lua_Number m_balance;
public:
	static const char className[];
	static Luna<bludImage>::RegType methods[];
	ofImage _image;
	bludImage(lua_State *L) { }
	int load (lua_State *L) { _image.loadImage(luaL_checkstring(L, 1)); return 0; }
	int draw(lua_State *L) { _image.draw(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return 0; }
	~bludImage() { printf("deleted image (%p)\n", this); }
};

const char bludImage::className[] = "bludImage";

Luna<bludImage>::RegType bludImage::methods[] = {
	method(bludImage, load),
	method(bludImage, draw),
	{0,0}
};

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	ofBackground(127,127,127);
	
	luaVM = lua_open();
	
	if (NULL == luaVM)
	{
		printf("Error Initializing lua\n");
	}
	
	luaL_openlibs(luaVM);
	Luna<bludImage>::Register(luaVM);
	
	int error = luaL_dofile(luaVM, [[[NSBundle mainBundle] pathForResource:@"boot" ofType:@"lua"] cString]);
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
	
	
	
	
	httpServer = [HTTPServer new];
	[httpServer setPort:1073];
	
	[httpServer setType:@"_http._tcp."];
	[httpServer setConnectionClass:[MyHTTPConnection class]];

//	[httpServer setDocumentRoot:[[NSBundle mainBundle] bundleURL]];
	NSString *root = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
	[httpServer setDocumentRoot:[NSURL fileURLWithPath:root]];

	NSError *httpError;
	if(![httpServer start:&httpError])
	{
		NSLog(@"Error starting HTTP Server: %@", httpError);
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "draw"); /* function to be called */
	if(lua_pcall(luaVM, 0, 0, 0) != 0){
		printf("error in draw: %s\n", lua_tostring(luaVM, -1));
	}
}

//--------------------------------------------------------------
void testApp::exit(){
	lua_close( luaVM );
	
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::lostFocus(){
	
}

//--------------------------------------------------------------
void testApp::gotFocus(){
	
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
	
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
	
}

