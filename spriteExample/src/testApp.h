#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxBlud.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		
		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);
	
		void audioRequested( float * output, int bufferSize, int nChannels );

		ofxBlud blud;
		ofAudioEventArgs e; // required for audio passing, poco does not currently work
};

#endif
