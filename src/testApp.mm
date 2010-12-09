#include "testApp.h"
#include "bludImage.h"
#include "bludGraphics.h"

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
	Luna<bludGraphics>::Register(luaVM);
	
	int error = luaL_dofile(luaVM, [[[NSBundle mainBundle] pathForResource:@"boot" ofType:@"lua"] cString]);
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
	
	httpServer = [luaHTTPServer new];
	[httpServer setPort:1073];
	[httpServer setLua:luaVM];
	
	[httpServer setType:@"_http._tcp."];
	[httpServer setConnectionClass:[MyHTTPConnection class]];

	NSString *root = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
	[httpServer setDocumentRoot:[NSURL fileURLWithPath:root]];
	string s = "blud.doc_root = '";
	s += [root cString];
	s += "'";
	error = luaL_dostring(luaVM, s.c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
	
	
	NSError *httpError;
	if(![httpServer start:&httpError])
	{
		NSLog(@"Error starting HTTP Server: %@", httpError);
	}
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "update"); /* function to be called */
	lua_pushnumber(luaVM, ofGetElapsedTimeMillis()-lastTime);
	lastTime = ofGetElapsedTimeMillis();
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in update: %s\n", lua_tostring(luaVM, -1));
	}
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
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "touchdown"); /* function to be called */
	lua_pushnumber(luaVM, touch.x);
	lua_pushnumber(luaVM, touch.y);
	lua_pushnumber(luaVM, touch.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touchdown: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "touchmoved"); /* function to be called */
	lua_pushnumber(luaVM, touch.x);
	lua_pushnumber(luaVM, touch.y);
	lua_pushnumber(luaVM, touch.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touchmoved: %s\n", lua_tostring(luaVM, -1));
	}		
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "touchup"); /* function to be called */
	lua_pushnumber(luaVM, touch.x);
	lua_pushnumber(luaVM, touch.y);
	lua_pushnumber(luaVM, touch.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touchup: %s\n", lua_tostring(luaVM, -1));
	}	
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

