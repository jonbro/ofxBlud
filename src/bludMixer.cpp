#include "bludMixer.h"

ofSoundMixer* bludMixer::instance = NULL; 

ofSoundMixer* bludMixer::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new ofSoundMixer();
	return instance;	
}