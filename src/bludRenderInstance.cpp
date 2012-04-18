/*
 *  bludPdInstance.cpp
 *  spaceHero
 *
 *  Created by jonbroFERrealz on 6/29/11.
 *  Copyright 2011 Heavy Ephemera Industries. All rights reserved.
 *
 */

#include "bludRenderInstance.h"

const char bludRenderer::className[] = "bludRenderer";

Lunar<bludRenderer>::RegType bludRenderer::methods[] = {
	method(bludRenderer, setSpriteSheet),
	{0,0}
};

bludRenderSingleton* bludRenderInstance::instance = NULL; 

bludRenderSingleton* bludRenderInstance::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new bludRenderSingleton();
	return instance;	
}