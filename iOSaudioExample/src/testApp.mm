#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	blud.setup();
	ofxRegisterMultitouch(this);

	ofSoundStreamSetup(2,0,this, 44100,256, 4);
	cout << blud.executeFile("audioExample.lua") << endl; // this returns an error code for the compiled code
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
}
void testApp::audioRequested(float * output, int bufferSize, int nChannels){
	e.buffer = output;
	e.bufferSize = bufferSize;
	e.nChannels = nChannels;
	blud.audioRequested(e);
}

//--------------------------------------------------------------
void testApp::exit(){
	
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::lostFocus(){
	
}

//--------------------------------------------------------------
void testApp::gotFocus(){
	
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
	
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
	
}

