#pragma once

// need to write the mixer class

// need to write the sampler class

class bludSynth {
public:
	static const char className[];
	static Lunar<bludImage>::RegType methods[];
	bludSynth(lua_State *L) {
		
	}
	int load (lua_State *L) {
	
	}
	int trigger(lua_State *L) { }	
	int setLoop(lua_State *L){ }
	int setNote(lua_State *L){ }
	
	~bludSynth() { printf("deleted synth (%p)\n", this); }
};

const char bludImage::className[] = "bludSynth";

Lunar<bludImage>::RegType bludImage::methods[] = {
	{0,0}
};