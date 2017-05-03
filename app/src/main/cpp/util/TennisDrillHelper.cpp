#include "TennisDrillHelper.h"

#include "ALoxHelper.h"

#include <dlfcn.h>

void *TennisDrillHelper::s_pModule = NULL;
CREATE_TENNISDRILL_INTERFACE TennisDrillHelper::s_pCreateTennisDrillInterface = NULL;

I_TennisDrill * TennisDrillHelper::CreateTennisDrill( const char *szFileName )
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

    if ( s_pCreateTennisDrillInterface == NULL )
    {
        *(void **)( &s_pCreateTennisDrillInterface ) = dlsym( s_pModule, "createTennisDrillInterface" );
        if ( s_pCreateTennisDrillInterface == NULL )
        {
            Log_Error( "Failed to call dlsym(), dlerror = %s", dlerror() );
            return NULL;
        }
    }

    I_TennisDrill *pTennisDrill = s_pCreateTennisDrillInterface();
    if ( pTennisDrill == NULL )
    {
        Log_Error( "Failed to call createTennisDrillInterface()." );
        return NULL;
    }

    pTennisDrill->addRef();

    return pTennisDrill;
}

void TennisDrillHelper::DestroyTennisDrill( I_TennisDrill *pTennisDrill )
{
    unsigned int refCount = pTennisDrill->releaseRef();
    if ( refCount == 0 )
    {
        s_pCreateTennisDrillInterface = NULL;

        dlclose( s_pModule );
        s_pModule = NULL;
    }
}