#pragma once

#include "ofSoundUnit.h"

// need to write the sampler class
// currently just using a test tone

class bludSynth {

	public:
		static const char className[];
		static Lunar<bludSynth>::RegType methods[];
		bludSynth(lua_State *L){
			mixer = bludMixer::getInstance();
			mixer->addInputFrom(&tone);
		}
		int load (lua_State *L){
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
		ofSoundSourceTestTone tone;
};

const char bludSynth::className[] = "bludSynth";

Lunar<bludSynth>::RegType bludSynth::methods[] = {
	{0,0}
};