#include "TennisDrillImpl.h"
#include <ALoxHelper.h>

#define DEFAULT_PACKAGE_COUNT 3 // (2 * 4096) / sizeof( DrillPackage )
#define DEFAULT_DRILL_PACKAGE_SIZE DEFAULT_PACKAGE_COUNT * sizeof( DrillPackage )

TennisDrillImpl::TennisDrillImpl()
    : m_pDrillCallback( NULL )
    , m_pRecPackage( NULL )
{
}

TennisDrillImpl::~TennisDrillImpl()
{
    dispose();

    if ( m_pSocketPackage != NULL )
    {
        delete m_pSocketPackage;
        m_pSocketPackage = NULL;
    }

    if ( m_pRecPackage != NULL )
    {
        delete[] m_pRecPackage;
        m_pRecPackage = NULL;
    }
}

bool TennisDrillImpl::init( const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty )
{
    m_pDrillCallback = pDrillCallback;
    if (!connectServer())
        return false;

    m_pRecPackage    = new DrillPackage[DEFAULT_PACKAGE_COUNT];
    m_pSocketPackage = new SocketClientPackage();
    m_pSocketPackage->Drill.difficulty = drillDifficulty;
    if (!TennisBaseImpl::init( initParam ))
    {
        delete[] m_pRecPackage;
        m_pRecPackage = NULL;

        delete m_pSocketPackage;
        m_pSocketPackage = NULL;

        return false;
    }

    return true;
}

void TennisDrillImpl::readcb( struct bufferevent *bev, void *user_data )
{
    TennisDrillImpl *pThis = (TennisDrillImpl *)user_data;
    I_DrillCallback *pDrillCallback = pThis->m_pDrillCallback;

    bufferevent_flush( bev, EV_READ | EV_WRITE, BEV_FLUSH );
    size_t readSize = bufferevent_read( bev, (char *)pThis->m_pRecPackage + pThis->m_packageOffset, DEFAULT_DRILL_PACKAGE_SIZE - pThis->m_packageOffset );
    size_t totalSize = pThis->m_packageOffset + readSize;
    Log_Verbose( "TennisDrillImpl::1 readSize = %d, totalSize = %d, command=%d\n", readSize, totalSize, pThis->m_pRecPackage->serverDrillCommand );
    if ( totalSize >= sizeof( DrillPackage ) )
    {
        DrillPackage *pDrillPackage = pThis->m_pRecPackage;
        do
        {
            Log_Verbose( "TennisDrillImpl::2 readSize = %d, totalSize = %d, command=%d\n", readSize, totalSize, pDrillPackage->serverDrillCommand );
            switch ( pDrillPackage->serverDrillCommand )
            {
                case DRILL_ON_BEGIN:
                {
                    pDrillCallback->onBegin();
                    break;
                }
                case DRILL_ON_DATA_CHANGE:
                {
                    pDrillPackage->drillBoxScore.pBallPointHit = pDrillPackage->OnDataChange.ballPointHit;
                    pDrillPackage->drillBoxScore.pBallPointDown = pDrillPackage->OnDataChange.ballPointDown;
                    pDrillCallback->onDataChange( pDrillPackage->drillBoxScore );

                    break;
                }
                case DRILL_ON_SPEED_CHANGE:
                {
                    pDrillCallback->onSpeedChange( pDrillPackage->OnSpeedChange.speed );
                    break;
                }
                case DRILL_ON_END:
                {
                    pDrillCallback->onEnd();
                    break;
                }
                case DRILL_ON_DRILL_NEXT:
                {
                    pDrillCallback->onDrillNext();
                    break;
                }
                case DRILL_ON_ERROR:
                {
                    pDrillCallback->onError( pDrillPackage->OnError.code, pDrillPackage->OnError.errorMessage );
                    break;
                }
                case DRILL_ON_THREAD:
                {
                    pDrillCallback->onThread( pDrillPackage->OnThread.isExit );
                    break;
                }
                case DRILL_STATE:
                {
                    pthread_mutex_lock( &pThis->m_pthread_mutex );
                    pThis->m_isSuccess = pDrillPackage->isSuccess;
                    pthread_cond_signal( &pThis->m_pthread_con );
                    pthread_mutex_unlock( &pThis->m_pthread_mutex );
                    break;
                }
                default:
                    break;
            }

            pDrillPackage++;
            totalSize -= sizeof( DrillPackage );
        } while(totalSize >= sizeof( DrillPackage ));

        if (totalSize == 0)
        {
            memset( pThis->m_pRecPackage, 0, DEFAULT_DRILL_PACKAGE_SIZE );
        }
        else
        {
            memmove( pThis->m_pRecPackage, pDrillPackage, totalSize );
        }
    }

    pThis->m_packageOffset = totalSize;
}

void TennisDrillImpl::onThread(bool isExit)
{
    m_pDrillCallback->onThread(isExit);
}