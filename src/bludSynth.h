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
		int trigger(lua_State *L){
			sampler.trigger();
			return 1;
		}
		int setLoopType(lua_State *L){ 
			sampler.setLoopType(luaL_checknumber(L, 1));
			return 1;
		}
		int setNote(lua_State *L){
			sampler.setFrequencyMidiNote(luaL_checknumber(L, 1));
			return 1;
		}
		
		~bludSynth() {
			// remove the connection to the sampler
			mixer->removeInputFrom(&sampler);
			printf("deleted synth (%p)\n", this);
		}
	private:
		ofSoundMixer *mixer;
		ofxSynthSampler sampler;
};

const char bludSynth::className[] = "bludSynth";

Lunar<bludSynth>::RegType bludSynth::methods[] = {
	method(bludSynth, load),
	method(bludSynth, trigger),
	method(bludSynth, setNote),
	method(bludSynth, setLoopType),
	{0,0}
};