#pragma once

#include "bludMixer.h"

// need to write the sampler class

class bludSynth {

	public:
		static const char className[];
		static Lunar<bludImage>::RegType methods[];
		bludSynth(lua_State *L) {
			mixer = bludMixer::getInstance();
		}
		int load (lua_State *L) {
		
		}
		int trigger(lua_State *L) { }	
		int setLoop(lua_State *L){ }
		int setNote(lua_State *L){ }
		
		~bludSynth() { printf("deleted synth (%p)\n", this); }
	private:
		bludMixer *mixer;
};

const char bludImage::className[] = "bludSynth";

Lunar<bludImage>::RegType bludImage::methods[] = {
	{0,0}
};