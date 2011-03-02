#include <vector>
#include "ofEvents.h"
#include "ofTypes.h"
#include "ofUtils.h"

using namespace std;

class ofSoundSource;

/** ofSoundUnit
 
 Base class for sound units that can be strung together into a DSP-style chain.
 
 @author damian
 */

class ofSoundUnit
{
public:	
	virtual ~ofSoundUnit() {};
	
	/// Return the name of this synth unit.
	virtual string getName() = 0;
	
	/// Return our inputs in a vector (but at ofSoundUnit level we have no inputs).
	virtual vector<ofSoundSource*> getInputs()  { return vector<ofSoundSource*>(); }
	
protected:
	
	
};


/** ofSoundSource
 
 Base class for ofSoundUnits that generate some kind of sound output.
 
 @author damian
 */

class ofSoundSource: public virtual ofSoundUnit
{
public:
	virtual ~ofSoundSource() {};
	
	/// Set the sample rate. If you need to know about sample rate changes, override this function.
	virtual void setSampleRate( int rate ) {};
	
	/// Fill buffer with (numFrames*numChannels) floats of data, interleaved
	virtual void audioRequested( float* buffer, int numFrames, int numChannels ) = 0;
	
protected:
	
	
};

/**
 * mixes the outputs of multiple synths
 * 
 * Borrows from github/damiannz sound contributions to openframeworks
 * Uses a singleton pattern
 */

class bludMixer {
	public:
		static bludMixer* getInstance();
		bool addInputFrom( ofSoundSource* source );
		void audioRequested( ofAudioEventArgs &e );

	protected:
		ofMutex mutex;

	private:
		static bludMixer *instance;
		struct MixerInput
		{
			ofSoundSource* input;
			float volume;
			float pan;
			MixerInput( ofSoundSource* i, float v, float p )
			{
				input = i;
				volume = v; 
				pan = p;
			}
		};
		vector<MixerInput*> inputs;
		float* working; // mix buffer
};