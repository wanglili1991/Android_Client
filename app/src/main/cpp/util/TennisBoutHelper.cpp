#include "TennisBoutHelper.h"

#include "ALoxHelper.h"

#include <dlfcn.h>

void *TennisBoutHelper::s_pModule = NULL;
CREATE_TENNISBOUT_INTERFACE TennisBoutHelper::s_pCreateTennisBoutInterface = NULL;

I_TennisBout * TennisBoutHelper::CreateTennisBout( const char *szFileName )
{
    if ( s_pModule == NULL )
    {
        s_pModule = dlopen( szFileName, RTLD_LAZY );
        if ( s_pModule == NULL )
        {
            Log_Error( "Failed to call dlopen(), dlerror = %s", dlerror() );
            return NULL;
        }
    }

    if ( s_pCreateTennisBoutInterface == NULL )
    {
        *(void **)( &s_pCreateTennisBoutInterface ) = dlsym( s_pModule, "createTennisBoutInterface" );
        if ( s_pCreateTennisBoutInterface == NULL )
        {
            Log_Error( "Failed to call dlsym(), dlerror = %s", dlerror() );
            return NULL;
        }
    }

    I_TennisBout *pTennisBout = s_pCreateTennisBoutInterface();
    if ( pTennisBout == NULL )
    {
        Log_Error( "Failed to call createTennisBoutInterface()." );
        return NULL;
    }

    pTennisBout->addRef();

    return pTennisBout;
}

void TennisBoutHelper::DestroyTennisBout( I_TennisBout *pTennisBout )
{
    unsigned int refCount = pTennisBout->releaseRef();
    if ( refCount == 0 )
    {
        s_pCreateTennisBoutInterface = NULL;

        dlclose( s_pModule );
        s_pModule = NULL;
    }
}