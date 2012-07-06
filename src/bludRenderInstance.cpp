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
    for(int i=0; i < sheets.size(); i++)
    {
        if(currentBlend != sheets[i]->blendMode){
            if(sheets[i]->blendMode == 0){
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }else if(sheets[i]->blendMode == 1){
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            }
            //                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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
        sheets[i]->spriteRenderer->draw();
    }
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