#pragma once

#include "ofSoundUnit.h"

/**
 * mixes the outputs of multiple synths
 * 
 * a singleton wrapper for the ofSoundMixer
 * Uses a singleton pattern
 */

class bludMixer{
	public:
		static ofSoundMixer* getInstance();
	private:
		static ofSoundMixer *instance;
};