#include "bludColor.h"

const char bludColor::className[] = "bludColor";

Lunar<bludColor>::RegType bludColor::methods[] = {
	method(bludColor, set),
    method(bludColor, get),
	{0,0}
};