#pragma once

#define SLOONGNET_IMAGE_METHOD_NAME "class_sloongnet_image"
typedef unsigned char UCHAR;

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// cimag
#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

//#include "jpeg.h"

#include <string>
#include <univ/exception.h>
#include <univ/log.h>
using namespace std;
using namespace Sloong::Universal;
