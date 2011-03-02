#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxBlud.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		void audioRequested( float * output, int bufferSize, int nChannels );

		ofxBlud blud;
		ofAudioEventArgs e; // required for audio passing, poco does not currently work
};

#endif
