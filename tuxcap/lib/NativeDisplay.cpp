#include "NativeDisplay.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
NativeDisplay::NativeDisplay()
{
	mRGBBits = 0;

	mRedMask = 0;
	mGreenMask = 0;
	mBlueMask = 0;

	mRedBits = 0;
	mGreenBits = 0;
	mBlueBits = 0;

	mRedShift = 0;
	mGreenShift = 0;
	mBlueShift = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
NativeDisplay::~NativeDisplay()
{
}

