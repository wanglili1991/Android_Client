#include "TennisBoutImpl.h"
#include <ALoxHelper.h>

#define DEFAULT_BOUT_PACKAGE_COUNT 16            // 2 * 4096 / sizeof( BoutPackage )
#define DEFAULT_BOUT_PACKAGE_SIZE DEFAULT_BOUT_PACKAGE_COUNT * sizeof( BoutPackage )

TennisBoutImpl::TennisBoutImpl()
    : m_pBoutCallback( NULL )
    , m_pRecPackage( NULL )
{
}

TennisBoutImpl::~TennisBoutImpl()
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

bool TennisBoutImpl::init( const InitParam &initParam, I_BoutCallback *pBoutCallback )
{
    m_pBoutCallback = pBoutCallback;
    if (!connectServer())
        return false;

    m_pRecPackage    = new BoutPackage[DEFAULT_BOUT_PACKAGE_COUNT];
    m_pSocketPackage = new SocketClientPackage();
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

bool TennisBoutImpl::ballPower( BallPower ballPower )
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );
	
    m_pSocketPackage->clientCommand = CLIENT_BALLPOWER;
    m_pSocketPackage->BallPower.ballPower = ballPower;
	bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );
	
    return m_isSuccess;
}

void TennisBoutImpl::readcb( struct bufferevent *bev, void *user_data )
{
    TennisBoutImpl *pThis = (TennisBoutImpl *)user_data;
    I_BoutCallback *pBoutCallback = pThis->m_pBoutCallback;

    bufferevent_flush( bev, EV_READ | EV_WRITE, BEV_FLUSH );
    size_t readSize = bufferevent_read( bev, (char *)pThis->m_pRecPackage + pThis->m_packageOffset, DEFAULT_BOUT_PACKAGE_SIZE - pThis->m_packageOffset );
    size_t totalSize = pThis->m_packageOffset + readSize;
    Log_Verbose( "TennisBoutImpl::1 readSize = %d, totalSize = %d, command=%d\n", readSize, totalSize, pThis->m_pRecPackage->serverBoutCommand );
    if ( totalSize >= sizeof( BoutPackage ) )
    {
		BoutPackage *pBoutPackage = pThis->m_pRecPackage;
		do
		{
		    Log_Verbose( "TennisBoutImpl::2 readSize = %d, totalSize = %d, command=%d\n", readSize, totalSize, pBoutPackage->serverBoutCommand );
	        switch ( pBoutPackage->serverBoutCommand )
	        {
	            case BOUT_ON_BALL:
	            {
	                pBoutCallback->onBall( pBoutPackage->OnBall.ball );
	                break;
	            }
	            case BOUT_ON_BALLACE:
	            {
	                pBoutCallback->onBallAce( pBoutPackage->OnBallAce.player );
	                break;
	            }
	            case BOUT_ON_SERVE:
	            {
	                pBoutCallback->onServe( pBoutPackage->OnServe.ball );
	                break;
	            }
	            case BOUT_ON_SERVE_FAILED:
	            {
	                pBoutCallback->onServeFailed();
	                break;
	            }
	            case BOUT_ON_SERVE_SUCCESSFUL:
	            {
	                pBoutCallback->onServeSuccessful();
	                break;
	            }
	            case BOUT_ON_SERVE_TOUCH_DOWN_POSITION:
	            {
	                pBoutCallback->onServeTouchDownPosition( pBoutPackage->OnServeTouchDownPosition.player,
	                    pBoutPackage->OnServeTouchDownPosition.serveType, pBoutPackage->OnServeTouchDownPosition.ball );
	                break;
	            }
	            case BOUT_ON_HIT_POSITION:
	            {
	                pBoutCallback->onHitPosition( pBoutPackage->OnHitPosition.player, pBoutPackage->OnHitPosition.ball );
	                break;
	            }
	            case BOUT_ON_ADD_SCORE:
	            {
	                pBoutCallback->onAddScore( pBoutPackage->OnAddScore.player, pBoutPackage->OnAddScore.matchBoxScoreA,
	                    pBoutPackage->OnAddScore.matchBoxScoreB, pBoutPackage->OnAddScore.ball );
	                break;
	            }
	            case BOUT_ON_ERROR:
	            {
	                pBoutCallback->onError( pBoutPackage->OnError.code, pBoutPackage->OnError.errorMessage );
	                break;
	            }
	            case BOUT_ON_THREAD:
	            {
	                pBoutCallback->onThread( pBoutPackage->OnThread.isExit );
	                break;
	            }
	            case BOUT_STATE:
	            {
	                pthread_mutex_lock( &pThis->m_pthread_mutex );
	                pThis->m_isSuccess = pBoutPackage->isSuccess;
	                pthread_cond_signal( &pThis->m_pthread_con );
	                pthread_mutex_unlock( &pThis->m_pthread_mutex );
	            }
	            default:
	                break;
	        }

            pBoutPackage++;
            totalSize -= sizeof( BoutPackage );
        } while(totalSize >= sizeof( BoutPackage ));

        Log_Error( "TennisBoutImpl::3 readSize = %d, totalSize = %d, command=%d\n", readSize, totalSize, pBoutPackage->serverBoutCommand );
        if (totalSize == 0)
        {
            memset( pThis->m_pRecPackage, 0, DEFAULT_BOUT_PACKAGE_SIZE );
        }
        else
        {
            memmove( pThis->m_pRecPackage, pBoutPackage, totalSize );
        }
    }

    pThis->m_packageOffset = totalSize;
}

void TennisBoutImpl::onThread(bool isExit)
{
    m_pBoutCallback->onThread(isExit);
}