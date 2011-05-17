#pragma once
#include "ofxOsc.h"

class bludOscMessage{
public:
	static const char className[];
	static Lunar<bludOscMessage>::RegType methods[];
	bludOscMessage(lua_State *L){
	}
	int setAddress (lua_State *L){
		message.setAddress(luaL_checkstring(L, 1));
		return 1;
	}
	int addIntArg (lua_State *L){
		message.addIntArg(luaL_checknumber(L, 1));
		return 1;
	}
	int addFloatArg (lua_State *L){
		message.addFloatArg(luaL_checknumber(L, 1));
		return 1;
	}
	int addStringArg (lua_State *L){
		message.addStringArg(luaL_checkstring(L, 1));
		return 1;
	}
	int getAddress(lua_State *L){
		lua_pushstring(L, message.getAddress());
		return 1;
	}
	int getNumArgs(lua_State *L){
		lua_pushnumber(L, message.getNumArgs());
		return 1;
	}
	int getArgTypeName(lua_State *L){
		lua_pushstring(L, message.getArgTypeName(luaL_checknumber(L, 1)));
		return 1;
	}	
	int getArgAsInt(lua_State *L){
		lua_pushnumber(L, message.getArgAsInt32(luaL_checknumber(L, 1)));
		return 1;
	}	
	int getArgAsFloat(lua_State *L){
		lua_pushnumber(L, message.getArgAsFloat(luaL_checknumber(L, 1)));
		return 1;
	}	
	int getArgAsString(lua_State *L){
		lua_pushstring(L, message.getArgAsString(luaL_checknumber(L, 1)));
		return 1;
	}
	~bludOscMessage() {
		// remove the connection to the sampler
	}
	ofxOscMessage message;
};

const char bludOscMessage::className[] = "bludOscMessage";

Lunar<bludOscMessage>::RegType bludOscMessage::methods[] = {
	method(bludOscMessage, setAddress),
	method(bludOscMessage, addIntArg),
	method(bludOscMessage, addFloatArg),
	method(bludOscMessage, addStringArg),
	method(bludOscMessage, getAddress),
	method(bludOscMessage, getNumArgs),
	method(bludOscMessage, getArgTypeName),
	method(bludOscMessage, getArgAsInt),
	method(bludOscMessage, getArgAsFloat),
	method(bludOscMessage, getArgAsString),
	{0,0}
};

class bludOsc{
public:
	static const char className[];
	static Lunar<bludOsc>::RegType methods[];
	bludOsc(lua_State *L){
	}
	int setup (lua_State *L){
		sender.setup(luaL_checkstring(L, 1), luaL_checknumber(L, 2));
		return 1;
	}
	int sendMessage (lua_State *L){
		bludOscMessage *m = Lunar<bludOscMessage>::check(L, 1);
		sender.sendMessage(m->message);
		return 1;
	}
	~bludOsc() {
		// remove the connection to the sampler
		printf("removed osc (%p)\n", this);
	}
private:
	ofxOscSender sender;
};

const char bludOsc::className[] = "bludOsc";

Lunar<bludOsc>::RegType bludOsc::methods[] = {
	method(bludOsc, setup),
	method(bludOsc, sendMessage),
	{0,0}
};

class bludOscReceiver{
public:
	static const char className[];
	static Lunar<bludOscReceiver>::RegType methods[];	
	bludOscReceiver(lua_State *L){
	}
	int setup (lua_State *L){
		receiver.setup(luaL_checknumber(L, 1));
		return 1;
	}
	int hasWaitingMessages (lua_State *L){
		lua_pushboolean(L, receiver.hasWaitingMessages());
		return 1;
	}
	int getNextMessage (lua_State *L){
		bludOscMessage *m = Lunar<bludOscMessage>::check(L, 1);
		lua_pushboolean(L, receiver.getNextMessage(&m->message));
		return 1;
	}	
private:
	ofxOscReceiver receiver;
};
const char bludOscReceiver::className[] = "bludOscReceiver";

Lunar<bludOscReceiver>::RegType bludOscReceiver::methods[] = {
	method(bludOscReceiver, setup),
	method(bludOscReceiver, hasWaitingMessages),
	method(bludOscReceiver, getNextMessage),
	{0,0}
};
