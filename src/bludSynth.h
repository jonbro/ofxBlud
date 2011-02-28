#pragma once

class bludSynth {
public:
	static const char className[];
	static Lunar<bludImage>::RegType methods[];
	bludSynth(lua_State *L) {
		
	}
	~bludSynth() { printf("deleted synth (%p)\n", this); }
};

const char bludImage::className[] = "bludSynth";

Lunar<bludImage>::RegType bludImage::methods[] = {
	{0,0}
};