#ifndef __EDITLISTENER_H__
#define __EDITLISTENER_H__

#include "Common.h"
#include "SDL_keysym.h"

namespace Sexy
{

class EditListener
{
public:
	virtual void			EditWidgetText(int theId, const SexyString& theString) {};

	virtual bool			AllowKey(int theId, SDLKey theKey) { return true; }
	virtual bool			AllowChar(int theId, SexyChar theChar) { return true; }
	virtual bool			AllowText(int theId, const SexyString& theText) { return true; }
};

}

#endif //__EDITLISTENER_H__
