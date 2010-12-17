#pragma once

#include <lualib.h>
#import "HTTPServer.h"

@interface luaHTTPServer : HTTPServer
{
	lua_State* luaVM;
}

- (lua_State*)lua;
- (void)setLua:(lua_State*)newLua;

@end