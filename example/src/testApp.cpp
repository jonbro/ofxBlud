#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	blud.setup();
	cout << blud.execute("bg = bludGraphics(); bg:setColor(0,0,0,255);") << endl; // add a graphics object
	cout << blud.execute("function blud.draw() bg:drawRect(10,10,100,100); end") << endl; // setup the draw loop
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	blud.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

