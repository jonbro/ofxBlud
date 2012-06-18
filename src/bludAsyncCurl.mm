#include "bludAsyncCurl.h"

const char bludAsycCurl::className[] = "bludAsycCurl";

Lunar<bludAsycCurl>::RegType bludAsycCurl::methods[] = {
	method(bludAsycCurl, process),
	method(bludAsycCurl, isComplete),
    method(bludAsycCurl, getReturnValue),
	{0,0}
};
