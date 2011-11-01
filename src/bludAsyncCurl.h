#pragma once

#include "Lua-cURL.h"
#include "ofThread.h"
#include "lua.hpp"
#include "blud_boot.h"

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

        complete = false;
	}
    int process(lua_State *L) {
        // gets a chunk of lua code and processes it on another thread
        execute = luaL_checkstring(L, 1);
        startThread(true, false);   // blocking, verbose
        return 1;
    }
    //--------------------------
    void threadedFunction(){
        // do the actual processing of the lua code over here
        if(luaL_dostring(luaVM, execute.c_str()) != 0)
            printf("error in thread: %s\n", lua_tostring(luaVM, -1));

        // The function above pushes return values to the stack. We should probably store these and return them to the original thread
        if(lock()){
            complete = true;
            unlock();
        }else{
            
        }
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
    bool complete;
    std::string execute;
    std::string returnVal;
};


const char bludAsycCurl::className[] = "bludAsycCurl";

Lunar<bludAsycCurl>::RegType bludAsycCurl::methods[] = {
	method(bludAsycCurl, process),
	method(bludAsycCurl, isComplete),
	{0,0}
};
