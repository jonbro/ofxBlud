#pragma once

#include "ofSoundUnit.h"
#include "ofxSynthSampler.h"

// need to write the sampler class
// currently just using a test tone

class bludSynth {

	public:
		static const char className[];
		static Lunar<bludSynth>::RegType methods[];
		bludSynth(lua_State *L){
			mixer = bludMixer::getInstance();
			mixer->addInputFrom(&sampler);
		}
		int load (lua_State *L){
			sampler.loadFile(luaL_checkstring(L, 1));
			return 1;
		}
		int trigger(lua_State *L){	return 1; }	
		int setLoop(lua_State *L){ return 1; }
		int setNote(lua_State *L){ return 1; }
		
		~bludSynth() {
			printf("deleted synth (%p)\n", this);
		}
	private:
		ofSoundMixer *mixer;
		ofxSynthSampler sampler;
};

const char bludSynth::className[] = "bludSynth";

Lunar<bludSynth>::RegType bludSynth::methods[] = {
	method(bludSynth, load),
	{0,0}
};