#include "bludMixer.h"

bludMixer* bludMixer::instance = NULL; 

bludMixer* bludMixer::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new bludMixer();
	return instance;	
}

bool bludMixer::addInputFrom( ofSoundSource* source ){
	mutex.lock();
	inputs.push_back( new MixerInput( source, 1.0f, 0.5f ) );
	source->setSampleRate( sampleRate );
	mutex.unlock();
	return true;	
}

void bludMixer::audioRequested( ofAudioEventArgs &e ){
	// wipe the output
	memset( e.buffer, 0, e.bufferSize*e.nChannels*sizeof(float) );
	
	mutex.lock();
	
	// allocate working space
	if ( !working ) {
		working = new float[e.bufferSize*e.nChannels];
	}
	vector<float> volumePerChannel;
	volumePerChannel.resize( e.nChannels );
	for ( int i=0; i<(int)inputs.size(); i++ ) {
		// clear working
		memset( working, 0, e.bufferSize*e.nChannels*sizeof(float) );
		
		// render input into working
		inputs[i]->input->audioRequested( working, e.bufferSize, e.nChannels );
		
		// construct precalculated volume + pan array (for efficiency)
		float vol_l = inputs[i]->volume*(1.0f-inputs[i]->pan);
		float vol_r = inputs[i]->volume*inputs[i]->pan;
		for ( int j=0; j<e.nChannels; j++ ) {
			volumePerChannel[j] = (j==0?vol_l:vol_r) * masterVolume;
		}
		// mix working into output, respecting pan and volume and preserving interleaving
		float *working_ptr = working;
		float *output_ptr = e.buffer;
		for ( int j=0; j<e.bufferSize; j++ ) {
			for ( int k=0; k<e.nChannels; k++ ) {
				*output_ptr++ += *working_ptr++ * volumePerChannel[k];
			}
		}
	}
	
	mutex.unlock();	
}
