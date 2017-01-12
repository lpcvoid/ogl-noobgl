#pragma once

#ifndef nglCallbacks_h__
#define nglCallbacks_h__

#include <Windows.h>
#include <functional>

typedef void(CALLBACK* NGL_CALLBACK_MAIN_LOOP_UPDATE)(float delta_time);


typedef std::function<void(void)> NGL_CALLBACK_VOIDCALL;



enum NGL_CALLBACK_TYPE {

	NGL_CALLBACK_TYPE_VOID_VOID_NOTIFYDIRTY

};


#endif // nglCallbacks_h__

