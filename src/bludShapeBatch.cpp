#include "bludShapeBatch.h"

const char bludShapeBatch::className[] = "bludShapeBatch";

Lunar<bludShapeBatch>::RegType bludShapeBatch::methods[] = {
	method(bludShapeBatch, addRect),
	method(bludShapeBatch, addCircle),
	method(bludShapeBatch, draw),
	method(bludShapeBatch, clear),
	method(bludShapeBatch, setColor),
	{0,0}
};
