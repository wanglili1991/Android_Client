#ifndef __TENNISDRILL_HELPER_H__
#define __TENNISDRILL_HELPER_H__

#include "I_TennisDrill.h"

class TennisDrillHelper
{
public:
    static I_TennisDrill* CreateTennisDrill( const char *szFileName );
    static void           DestroyTennisDrill( I_TennisDrill *pTennisDrill );

private:
    static void *s_pModule;
    static CREATE_TENNISDRILL_INTERFACE s_pCreateTennisDrillInterface;
};

#endif