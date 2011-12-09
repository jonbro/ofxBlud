#include "ofxBlud.h"

#include "bludImage.h"
#include "bludGraphics.h"
#include "bludSynth.h"
#include "bludSpriteSheet.h"
#include "bludShapeBatch.h"
#include "bludShapeBatch.h"
#include "bludFont.h"
#include "bludLine.h"
#include "bludOsc.h"
#include "bludAsyncCurl.h"
#include "bludWebview.h"
#include "blud_boot.h"
#include "FlurryAnalytics.h"

static void stackDump (lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
				
			case LUA_TSTRING:  /* strings */
				printf("`%s'", lua_tostring(L, i));
				break;
				
			case LUA_TBOOLEAN:  /* booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
				
			case LUA_TNUMBER:  /* numbers */
				printf("%g", lua_tonumber(L, i));
				break;
				
			default:  /* other values */
				printf("%s", lua_typename(L, t));

				
        }
        printf("  ");  /* put a separator */
	}
	printf("\n");  /* end the listing */
}

static int luaErrorHandler(lua_State *L) 
{
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return 1;
    }
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 0);
    lua_call(L, 2, 1);
    return 1;
}


void ofxBlud::setup(){

	// load the virtual machine
	luaVM = lua_open();
	if (NULL == luaVM)
	{
		ofLog(OF_LOG_ERROR, "Error Initializing lua");
	}
	ofSetLogLevel(OF_LOG_ERROR);
	// load the standard libs
	luaL_openlibs(luaVM);
    luaopen_debug(luaVM);
    
	// load the wrappers
	Lunar<bludImage>::Register(luaVM);
	Lunar<bludGraphics>::Register(luaVM);
	Lunar<bludSynth>::Register(luaVM);
	Lunar<bludSprite>::Register(luaVM);
	Lunar<bludSpriteSheet>::Register(luaVM);
	Lunar<bludShapeBatch>::Register(luaVM);
	Lunar<bludFont>::Register(luaVM);
	Lunar<bludLine>::Register(luaVM);
	Lunar<bludVideoPlayer>::Register(luaVM);
	Lunar<bludOsc>::Register(luaVM);
	Lunar<bludOscMessage>::Register(luaVM);
	Lunar<bludOscReceiver>::Register(luaVM);
    Lunar<bludAsycCurl>::Register(luaVM);
    Lunar<bludWebview>::Register(luaVM);
    
	// load the bootfile, which has placeholder for all the callbacks
	int error = luaL_dostring(luaVM, blud_boot);	
	if (error) {
		ofLog(OF_LOG_ERROR, "Blud Bootload failed");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
	
	// set the default paths so that loading properly works
	string s = "blud.bundle_root = '";
	s += ofToDataPath("");
	s += "'";
	error = luaL_dostring(luaVM, s.c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}	
	
	// document directory for writing to the file system
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"/"];

	// set the default paths so that loading properly works
	s = "blud.doc_root = '";
	s += [filePath cString];
	s += "'";
	error = luaL_dostring(luaVM, s.c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}	
	
	// touch events
	ofAddListener(ofEvents.touchDown, this, &ofxBlud::touchDown);
	ofAddListener(ofEvents.touchUp, this, &ofxBlud::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &ofxBlud::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap, this, &ofxBlud::touchDoubleTap);

	ofAddListener(ofEvents.keyPressed, this, &ofxBlud::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxBlud::keyReleased);
	
	// mouse events
	ofAddListener(ofEvents.mousePressed, this, &ofxBlud::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxBlud::mouseReleased);
	ofAddListener(ofEvents.mouseMoved, this, &ofxBlud::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxBlud::mouseDragged);	
		
	// sys events
	ofAddListener(ofEvents.draw, this, &ofxBlud::draw);
	ofAddListener(ofEvents.update, this, &ofxBlud::update);
	ofAddListener(ofEvents.exit, this, &ofxBlud::exit);

	lua_register(luaVM,"LuaXML_ParseFile",LuaXML_ParseFile);

	mixer = bludMixer::getInstance();
	ofEnableAlphaBlending();
	mutex = bludLock::getInstance();
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	int xpos = 0;
	float red = 1.0;
	// draw the first square
	mesh.addVertex(ofVec3f(0+xpos, 0, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(0+xpos, 50, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(50+xpos, 0, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(50+xpos, 50, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	// extra vertex to add discontinuity
	mesh.addVertex(ofVec3f(50+xpos, 50, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	
	xpos = 100;
	red = 0;
	// you need two extra vertexes for the full discontinuity
	mesh.addVertex(ofVec3f(0+xpos, 0, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	
	// draw the second square
	mesh.addVertex(ofVec3f(0+xpos, 0, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(0+xpos, 50, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(50+xpos, 0, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
	mesh.addVertex(ofVec3f(50+xpos, 50, 0));
	mesh.addColor(ofFloatColor(red, 1.0, 0));
	mesh.addIndex(mesh.getNumVertices()-1);
    ofFbo::Settings settings;    

    settings.width = 1024;    
    settings.height = 1024;    
    settings.internalformat = GL_RGBA;    
    settings.numSamples = 0;    
    settings.useDepth = false;    
    settings.useStencil = false; 

    fbo = new ofFbo();  

    fbo->allocate(settings);
    texScreen.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void ofxBlud::draw(ofEventArgs &e){
    //fbo->begin();
	mutex->lock();
    
    lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "draw"); /* function to be called */

    int error_index = lua_gettop(luaVM);
    //push error handler onto stack..
    lua_pushcfunction(luaVM, luaErrorHandler);
    lua_insert(luaVM, error_index);

    if(lua_pcall(luaVM, 0, 0, error_index) != 0){
		ofLog(OF_LOG_ERROR, "Blud draw error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
        [FlurryAnalytics logError:@"blud draw error" message:[NSString stringWithUTF8String:lua_tostring(luaVM, -1)] exception:nil];
        // pop the error message off the top of the stack
        lua_pop(luaVM, 1);
	}
    
	// need to pop once for global and once for the field.
	// the numbers and the calling of the function do not need to be called because they are automatically popped off the stack
    // pop again for the traceback
	lua_pop(luaVM,2);
	mutex->unlock();
//    ofPushMatrix();
//    ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
//    ofScale(1.1+sin(ofDegToRad(ofGetElapsedTimeMillis()/14.0))*0.1, 1.1+sin(ofDegToRad(ofGetElapsedTimeMillis()/16.0))*0.1);
//    ofRotate(sin(ofDegToRad(ofGetElapsedTimeMillis()/14.0)), 0, 0, 1);
//    ofTranslate(-ofGetWidth()/2,-ofGetHeight()/2);
//    feedbackColor.setHsb(ofGetElapsedTimeMillis()/10%255, 255, 255, 100+sin(ofDegToRad(ofGetElapsedTimeMillis()/3.0))*100.0);
//    ofSetColor(feedbackColor);
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    texScreen.draw(0,0,ofGetWidth(),ofGetHeight());
//    texScreen.loadScreenData(0,0,ofGetWidth(),ofGetHeight());
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    ofPopMatrix();
    //fbo->end();
    //ofSetColor(255, 255, 255, 255);
    //fbo->draw(0, 0);
	//mesh.drawFaces();
}
void ofxBlud::exit(ofEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "exit"); /* function to be called */
	if(lua_pcall(luaVM, 0, 0, 0) != 0){
        
		ofLog(OF_LOG_ERROR, "Blud exit error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
	// need to pop once for global and once for the field.
	// the numbers and the calling of the function do not need to be called because they are automatically popped off the stack
	lua_pop(luaVM,1);
	mutex->unlock();
}

void ofxBlud::update(ofEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "update"); /* function to be called */

	lua_pushnumber(luaVM, ofGetElapsedTimeMillis());

    int error_index = lua_gettop(luaVM) - 1; // subtract the number of params
    //push error handler onto stack..
    lua_pushcfunction(luaVM, luaErrorHandler);
    lua_insert(luaVM, error_index);

	if(lua_pcall(luaVM, 1, 0, error_index) != 0){
		ofLog(OF_LOG_ERROR, "Blud update error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
        [FlurryAnalytics logError:@"blud update error" message:[NSString stringWithUTF8String:lua_tostring(luaVM, -1)] exception:nil];
        // pop the error message off the top of the stack
        lua_pop(luaVM, 1);
	}
	lua_pop(luaVM,2);
	mutex->unlock();
}

string ofxBlud::execute(string code){
	mutex->lock();
	int error = luaL_dostring(luaVM, code.c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	mutex->unlock();
	return "";
}

string ofxBlud::executeFile(std::string filename){
	mutex->lock();
	int error = luaL_dofile(luaVM, ofToDataPath(filename).c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	mutex->unlock();
	return "";
}
// event callbacks
void ofxBlud::keyPressed(ofKeyEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "key"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, e.key);
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in key.pressed: %s\n", lua_tostring(luaVM, -1));
	}
	mutex->unlock();
}
void ofxBlud::keyReleased(ofKeyEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "key"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, e.key);
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in key.released: %s\n", lua_tostring(luaVM, -1));
	}
	mutex->unlock();
}

// event callbacks
void ofxBlud::mousePressed(ofMouseEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.pressed: %s\n", lua_tostring(luaVM, -1));
	}
    lua_pop(luaVM,2);
    mutex->unlock();
}
void ofxBlud::mouseMoved(ofMouseEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.moved: %s\n", lua_tostring(luaVM, -1));
	}
    lua_pop(luaVM,2);
    mutex->unlock();
}
void ofxBlud::mouseDragged(ofMouseEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "dragged"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.dragged: %s\n", lua_tostring(luaVM, -1));
	}
    lua_pop(luaVM,2);
    mutex->unlock();
}
void ofxBlud::mouseReleased(ofMouseEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.released: %s\n", lua_tostring(luaVM, -1));
	}	
    lua_pop(luaVM,2);
    mutex->unlock();
}

// all of the touch events
// I wonder if all this stuff could be handled through a macro or something?
void ofxBlud::touchDown(ofTouchEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "down"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.down: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,2);
	mutex->unlock();
}
void ofxBlud::touchMoved(ofTouchEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.moved: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,2);
	mutex->unlock();
}
void ofxBlud::touchUp(ofTouchEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "up"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.up: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,2);
//	cout << "lua stack depth: " << lua_gettop(luaVM) << endl;
	mutex->unlock();
}
void ofxBlud::touchDoubleTap(ofTouchEventArgs &e){
	mutex->lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "double_tap"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.double_tap: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,2);
	mutex->unlock();
}

// does not pass audio processing through to lua
// manages triggering synths and feeding synth data to the output

// temporarily testing with noise
#include "ofMath.h"

void ofxBlud::audioRequested(ofAudioEventArgs &e){
	// getting called
	//mixer->audioRequested(e.buffer, e.bufferSize, e.nChannels);
}

// via http://lua-users.org/wiki/LuaXml

void LuaXML_ParseNode (lua_State *L,TiXmlNode* pNode) { 
	if (!pNode) return;
	// resize stack if neccessary
	luaL_checkstack(L, 5, "LuaXML_ParseNode : recursion too deep");
	
	TiXmlElement* pElem = pNode->ToElement();
	if (pElem) {
		// element name
		lua_pushstring(L,"name");
		lua_pushstring(L,pElem->Value());
		lua_settable(L,-3);
		
		// parse attributes
		TiXmlAttribute* pAttr = pElem->FirstAttribute();
		if (pAttr) {
			lua_pushstring(L,"attr");
			lua_newtable(L);
			for (;pAttr;pAttr = pAttr->Next()) {
				lua_pushstring(L,pAttr->Name());
				lua_pushstring(L,pAttr->Value());
				lua_settable(L,-3);
				
			}
			lua_settable(L,-3);
		}
	}
	
	// children
	TiXmlNode *pChild = pNode->FirstChild();
	if (pChild) {
		int iChildCount = 0;
		for(;pChild;pChild = pChild->NextSibling()) {
			switch (pChild->Type()) {
				case TiXmlNode::DOCUMENT: break;
				case TiXmlNode::ELEMENT: 
					// normal element, parse recursive
					lua_newtable(L);
					LuaXML_ParseNode(L,pChild);
					lua_rawseti(L,-2,++iChildCount);
					break;
				case TiXmlNode::COMMENT: break;
				case TiXmlNode::TEXT: 
					// plaintext, push raw
					lua_pushstring(L,pChild->Value());
					lua_rawseti(L,-2,++iChildCount);
					break;
				case TiXmlNode::DECLARATION: break;
				case TiXmlNode::UNKNOWN: break;
			};
		}
		lua_pushstring(L,"n");
		lua_pushnumber(L,iChildCount);
		lua_settable(L,-3);
	}
}

static int LuaXML_ParseFile (lua_State *L) {
	const char* sFileName = luaL_checkstring(L,1);
	TiXmlDocument doc(sFileName);
	doc.LoadFile();
	lua_newtable(L);
	LuaXML_ParseNode(L,&doc);
	return 1;
}
