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
    mainShader.begin();
    mainShader.setUniform1f("width", ofGetWidth());
    mainShader.setUniform1f("height", ofGetHeight());
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glDisable(GL_DEPTH_TEST);
	ofViewport(0, 0, fboTex->getWidth(), fboTex->getHeight(), false);
    ofSetupScreenPerspective(fboTex->getWidth(), fboTex->getHeight(), OF_ORIENTATION_DEFAULT, false);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);

    for(int i=0; i < sheets.size(); i++)
    {
        if(currentBlend != sheets[i]->blendMode){
            if(sheets[i]->blendMode == 0){
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }else if(sheets[i]->blendMode == 1){
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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
        sheets[i]->spriteRenderer->draw(&mainShader);
    }
    mainShader.end();
     
    fboShader.begin();


    glDisable(GL_BLEND);
    int h = ofGetHeight();
    int w = ofGetWidth();
    fboShader.setUniform1f("width", w);
    fboShader.setUniform1f("height", h);

    // this should rebind the root renderbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 1);
	ofViewport(0, 0, h,w, false);
    ofSetupScreenPerspective(h, w, OF_ORIENTATION_DEFAULT, false);
    glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
     /**/
    // NOTHING IS EVER EASY
    GLfloat tex_coords[] = {
		0,0,
		1.0,0,
		1.0,1.0,
		0,1.0
	};
    GLfloat color_coords[] = {
		1.0,1.0,1.0,1.0, // w
		0.0,0.0,1.0,1.0, // b
		1.0,0.0,0.0,1.0, // r
		0.0,1.0,0.0,1.0, // g
	};
	GLfloat verts[] = {
		0,0,
		w,0,
		w,h,
		0,h,
	};
    // NOTHING IS EVER EASY
    //fboShader.setUniformTexture("Texture", *sheets[0]->texture, sheets[0]->texture->getTextureData().textureID);
    fboShader.setUniformTexture("Texture", *fboTex, fboTex->getTextureData().textureID);
    glVertexAttribPointer(glGetAttribLocation(fboShader.getProgram(), "position"), 2, GL_FLOAT, GL_FALSE, 0, verts);
    glVertexAttribPointer(glGetAttribLocation(fboShader.getProgram(), "color"), 4, GL_FLOAT, GL_FALSE, 0, color_coords);
    glVertexAttribPointer(glGetAttribLocation(fboShader.getProgram(), "TexCoordIn"), 2, GL_FLOAT, GL_FALSE, 0, tex_coords);
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    fboShader.end();
}

const char bludRenderer::className[] = "bludRenderer";

Lunar<bludRenderer>::RegType bludRenderer::methods[] = {
	method(bludRenderer, setSpriteSheet),
    method(bludRenderer, removeSheet),
    method(bludRenderer, addAtStart),
	{0,0}
};

bludRenderSingleton* bludRenderInstance::instance = NULL; 

bludRenderSingleton* bludRenderInstance::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new bludRenderSingleton();
	return instance;	
}