/*
 *  bludPdInstance.cpp
 *  spaceHero
 *
 *  Created by jonbroFERrealz on 6/29/11.
 *  Copyright 2011 Heavy Ephemera Industries. All rights reserved.
 *
 */

#include "bludRenderInstance.h"
void bludRenderSingleton::render(){
    // make sure we are using the right shader
    /*
//    fboTex.unbind();
    */
#if defined TARGET_OF_IPHONE
    mainShader.setUniform1f("width", ofGetWidth());
    mainShader.setUniform1f("height", ofGetHeight());
    noTexShader.setUniform1f("width", ofGetWidth());
    noTexShader.setUniform1f("height", ofGetHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glDisable(GL_DEPTH_TEST);
	ofViewport(0, 0, fboTex->getWidth(), fboTex->getHeight(), false);
//    ofSetupScreenPerspective(fboTex->getWidth(), fboTex->getHeight(), OF_ORIENTATION_DEFAULT, false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glBlend should be disabled at this point
    glDisable(GL_BLEND);
#endif
    for(int i=0; i < sheets.size(); i++)
    {
#if defined TARGET_OF_IPHONE
        if(sheets[i]->hasTexture && currentShader != &mainShader){
            currentShader->end();
            mainShader.begin();
            currentShader = &mainShader;
        }else if (!sheets[i]->hasTexture && currentShader != &noTexShader){
            currentShader->end();
            noTexShader.begin();
            currentShader = &noTexShader;
        }
        currentShader->setUniform1f("width", ofGetWidth());
        currentShader->setUniform1f("height", ofGetHeight());
#endif    
        if(currentBlend != sheets[i]->blendMode){
            if(sheets[i]->blendMode == 0){
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }else if(sheets[i]->blendMode == 1){
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            }else if(sheets[i]->blendMode == 2){
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }
            currentBlend = sheets[i]->blendMode;
        }
        if(alpha != sheets[i]->alpha){
            alpha = sheets[i]->alpha;
            if(alpha){
                glEnable(GL_BLEND);
            }else{
                glDisable(GL_BLEND);
            }
        }
        sheets[i]->spriteRenderer->draw(currentShader);
        sheets[i]->spriteRenderer->clear();
    }
#if defined TARGET_OF_IPHONE
    currentShader->end();
    currentShader = fboShader;
    int h = ofGetHeight();
    int w = ofGetWidth();
    fboShader->begin();
    fboShader->setUniform1f("width", w);
    fboShader->setUniform1f("height", h);
    if(ofGetOrientation() == OF_ORIENTATION_90_LEFT){
        fboShader->setUniform1f("flipAmt", 1);
    }else{
        fboShader->setUniform1f("flipAmt", -1);
    }
    alpha = false;
    glDisable(GL_BLEND);
    // this should rebind the root renderbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 1);
	ofViewport(0, 0, h,w, true);
//    ofSetupScreenPerspective(h, w, OF_ORIENTATION_DEFAULT, true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // NOTHING IS EVER EASY
    fboShader->setUniformTexture("Texture", *fboTex, fboTex->getTextureData().textureID);
    fboShader->setUniform1f("t", ofGetElapsedTimef());
    glVertexAttribPointer(glGetAttribLocation(fboShader->getProgram(), "position"), 3, GL_SHORT, GL_FALSE, sizeof(vertexStruct), &points[0].position);
    glVertexAttribPointer(glGetAttribLocation(fboShader->getProgram(), "color"), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertexStruct), &points[0].color);
    glVertexAttribPointer(glGetAttribLocation(fboShader->getProgram(), "TexCoordIn"), 2, GL_FLOAT, GL_TRUE, sizeof(vertexStruct), &points[0].texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, pointCount);
    fboShader->end();
#endif 
}

const char bludRenderer::className[] = "bludRenderer";

Lunar<bludRenderer>::RegType bludRenderer::methods[] = {
	method(bludRenderer, setSpriteSheet),
    method(bludRenderer, removeSheet),
    method(bludRenderer, addAtStart),
    method(bludRenderer, loadFboPixelShader),
	{0,0}
};

bludRenderSingleton* bludRenderInstance::instance = NULL; 

bludRenderSingleton* bludRenderInstance::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new bludRenderSingleton();
	return instance;	
}