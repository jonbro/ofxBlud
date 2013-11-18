#pragma once

#include "ofxBlud.h"
#include "Lua-cURL.h"
#include "ofThread.h"
#include "lua.hpp"
#include "lunar.h"

/**
 * maybe just maintain a internal lua state that we can call over to from the main lua thread :D
 * haha, I am a madman!
 */

class bludAsycCurl  : public ofThread {
public:
    static const char className[];
	static Lunar<bludAsycCurl>::RegType methods[];
	bludAsycCurl(lua_State *L){
        luaVM = lua_open();
        if (NULL == luaVM)
        {
            ofLog(OF_LOG_ERROR, "Error Initializing lua");
        }
        ofSetLogLevel(OF_LOG_ERROR);
        luaL_openlibs(luaVM);
        luaopen_cURL(luaVM);
        
        // load the bootfile, which has placeholder for all the callbacks
        int error = luaL_dostring(luaVM, ofxBlud::blud_boot);	
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
        hasCallback = false;
        complete = false;
        mutex = bludLock::getInstance();
	}
    int process(lua_State *L) {
        // gets a chunk of lua code and processes it on another thread
        execute = luaL_checkstring(L, 1);
        // check to see if there is an included callback function
        if(lua_isfunction(L, 2)){
            // push the value of the function (that is at position 1), onto the top of the stack
            lua_pushvalue(L, 2);
            // store this stack position in the registry index
            callbackFunc = luaL_ref(L, LUA_REGISTRYINDEX);
            hasCallback = true;
            mainVM = L;
        }
        startThread(true, false);   // blocking, verbose
        return 1;
    }
    //--------------------------
    void threadedFunction(){
        // do the actual processing of the lua code over here
        int startStack = lua_gettop(luaVM);
        if(luaL_dostring(luaVM, execute.c_str()) != 0)
            printf("error in thread: %s\n", lua_tostring(luaVM, -1));
        // The function above pushes return values to the stack. We should probably store these and return them to the original thread
        if(lock()){
            if(hasCallback){
                // todo: should probably lock lua, so that things don't blow up here
                mutex->lock();
                numReturns = lua_gettop(luaVM) - startStack;
                lua_rawgeti(mainVM, LUA_REGISTRYINDEX, callbackFunc );
                lua_pushstring(mainVM, luaL_checkstring(luaVM, -1*numReturns));                
                if(lua_pcall(mainVM, numReturns, 0, 0) != 0){
                    ofLog(OF_LOG_ERROR, "async callback error");
                    ofLog(OF_LOG_ERROR, lua_tostring(mainVM, -1));
                }
                mutex->unlock();
            }
            complete = true;
            unlock();
        }else{
            
        }
    };
    int getReturnValue(lua_State *L) {
        if(complete){
            lua_pushstring(L, lua_tostring(luaVM,-1));
        }
        return 1;
    };
    int isComplete(lua_State *L) {
        if( lock() ){
            lua_pushboolean(L, complete);
            unlock();
        }else{
            cout << "can't lock!\neither an error\nor the thread has stopped" << endl;
        }
        return 1;
    };
private:
    lua_State* luaVM;
    lua_State* mainVM;
    bool complete, hasCallback;
    int callbackFunc, numReturns;
    std::string execute;
    std::string returnVal;
    ofMutex* mutex;
};