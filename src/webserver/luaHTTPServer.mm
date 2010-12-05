/*
 *  luaHTTPServer.mm
 *  iPhoneEmptyExample
 *
 *  Created by jonbroFERrealz on 12/5/10.
 *  Copyright 2010 Heavy Ephemera Industries. All rights reserved.
 *
 */

#include "luaHTTPServer.h"

@implementation luaHTTPServer

-(lua_State*)lua
{
	return luaVM;
}
-(void)setLua:(lua_State*)newLua
{
	luaVM = newLua;
}

@end