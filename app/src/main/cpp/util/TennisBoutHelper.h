#ifndef __TENNISBOUT_HELPER_H__
#define __TENNISBOUT_HELPER_H__

#include "I_TennisBout.h"

class TennisBoutHelper
{
public:
    static I_TennisBout* CreateTennisBout( const char *szFileName );
    static void          DestroyTennisBout( I_TennisBout *pTennisBout );

private:
    static void *s_pModule;
    static CREATE_TENNISBOUT_INTERFACE s_pCreateTennisBoutInterface;
};

#endif