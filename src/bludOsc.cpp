#include "bludOsc.h"

const char bludOscReceiver::className[] = "bludOscReceiver";

Lunar<bludOscReceiver>::RegType bludOscReceiver::methods[] = {
	method(bludOscReceiver, setup),
	method(bludOscReceiver, hasWaitingMessages),
	method(bludOscReceiver, getNextMessage),
	{0,0}
};

const char bludOsc::className[] = "bludOsc";

Lunar<bludOsc>::RegType bludOsc::methods[] = {
	method(bludOsc, setup),
	method(bludOsc, sendMessage),
	{0,0}
};

const char bludOscMessage::className[] = "bludOscMessage";

Lunar<bludOscMessage>::RegType bludOscMessage::methods[] = {
	method(bludOscMessage, setAddress),
	method(bludOscMessage, addIntArg),
	method(bludOscMessage, addFloatArg),
	method(bludOscMessage, addStringArg),
	method(bludOscMessage, getAddress),
	method(bludOscMessage, getNumArgs),
	method(bludOscMessage, getArgTypeName),
	method(bludOscMessage, getArgAsInt),
	method(bludOscMessage, getArgAsFloat),
	method(bludOscMessage, getArgAsString),
	{0,0}
};
