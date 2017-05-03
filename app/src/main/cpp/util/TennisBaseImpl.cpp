#include "TennisBaseImpl.h"
#include <ALoxHelper.h>

TennisBaseImpl::TennisBaseImpl()
    : m_uRefCount( 0 )
    , m_isSuccess( false )
    , m_connectStat( CONNECTSTAT_INVALID )
    , m_pEventBase( NULL )
    , m_pBufferEvent( NULL )
    , m_pSocketPackage( NULL )
    , m_uThreadId(-1)
    , m_packageOffset( 0 )
{
}

TennisBaseImpl::~TennisBaseImpl()
{
}

bool TennisBaseImpl::init( const InitParam &initParam )
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_INIT;
    m_pSocketPackage->socketInitParam.playModel     = initParam.playModel;
    m_pSocketPackage->socketInitParam.servePosition = initParam.servePosition;
    m_pSocketPackage->socketInitParam.courtSize     = initParam.courtSize;
    memcpy( m_pSocketPackage->socketInitParam.szAppPath, initParam.strAppPath.c_str(), initParam.strAppPath.size() );
    memcpy( m_pSocketPackage->socketInitParam.szCfgPath, initParam.strCfgPath.c_str(), initParam.strCfgPath.size() );
    memcpy( m_pSocketPackage->socketInitParam.szTmpPath, initParam.strTmpPath.c_str(), initParam.strTmpPath.size() );

    bool isSuccess = bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) == 0;
    if ( isSuccess )
    {
        pthread_mutex_lock( &m_pthread_mutex );
        pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
        pthread_mutex_unlock( &m_pthread_mutex );
    }

    if ( !isSuccess || !m_isSuccess )
    {
        dispose();
        Log_Error( "TennisBaseImpl:: failed to write!\n" );

        return false;
    }

    return m_isSuccess;
}

bool TennisBaseImpl::start()
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_START;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

bool TennisBaseImpl::pause()
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_PAUSE;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

bool TennisBaseImpl::resume()
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_RESUME;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

bool TennisBaseImpl::stop()
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_STOP;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

bool TennisBaseImpl::runOnce()
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_RUNONCE;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

bool TennisBaseImpl::restart( CourtArea serveArea )
{
    m_isSuccess = false;
    memset( m_pSocketPackage, 0, sizeof( SocketClientPackage ) );

    m_pSocketPackage->clientCommand = CLIENT_RESTART;
    m_pSocketPackage->Restart.courtArea = serveArea;
    bufferevent_flush( m_pBufferEvent, EV_READ | EV_WRITE, BEV_FLUSH );
    if (bufferevent_write( m_pBufferEvent, m_pSocketPackage, sizeof( SocketClientPackage ) ) != 0)
        return false;

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_isSuccess;
}

void TennisBaseImpl::dispose()
{
    m_connectStat = CONNECTSTAT_DISPOSE;

    if ( m_pBufferEvent != NULL )
    {
        bufferevent_free( m_pBufferEvent );
        m_pBufferEvent = NULL;
    }

    if ( m_pEventBase != NULL )
    {
        pthread_mutex_lock( &m_pthread_mutex );
        pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
        pthread_mutex_unlock( &m_pthread_mutex );
    }
}

bool TennisBaseImpl::connectServer()
{
    pthread_mutex_init( &m_pthread_mutex, NULL );
    pthread_cond_init( &m_pthread_con, NULL );

    if ( pthread_create( &m_uThreadId, NULL, conn_server, (void *)this ) != 0 )
    {
        Log_Error( "TennisBaseImpl::init:: pthread_create failed" );
        return false;
    }

    pthread_mutex_lock( &m_pthread_mutex );
    pthread_cond_timedwait( &m_pthread_con, &m_pthread_mutex, init_timeout() );
    pthread_mutex_unlock( &m_pthread_mutex );

    return m_connectStat == CONNECTSTAT_CONNECTED;
}

timespec* TennisBaseImpl::init_timeout()
{
    struct timeval now;
    struct timespec *spec = new timespec();
    gettimeofday( &now, NULL );
    spec->tv_sec  = now.tv_sec + 10000;
    spec->tv_nsec = now.tv_usec * 1000;
    return spec;
}

void TennisBaseImpl::conn_readcb( struct bufferevent *bev, void *user_data )
{
    TennisBaseImpl *pThis = (TennisBaseImpl *)user_data;
    pThis->readcb(bev, user_data);
}

void TennisBaseImpl::conn_eventcb( struct bufferevent *bev, short events, void *user_data )
{
    TennisBaseImpl *pThis = (TennisBaseImpl *)user_data;

    ConnectStat lastConnectStat = pThis->m_connectStat;
    if ( events & ( BEV_EVENT_EOF | BEV_EVENT_ERROR ) )
    {
        Log_Error( "Connection closed, events = %d, pThis->m_connectStat = %d.\n", events, pThis->m_connectStat );
        if (pThis->m_connectStat == CONNECTSTAT_DISPOSE)
        {
            event_base_loopbreak( pThis->m_pEventBase );
        }
        else
        {
            if (pThis->m_pBufferEvent != NULL)
            {
                bufferevent_free( pThis->m_pBufferEvent );
                pThis->m_pBufferEvent = NULL;
            }

            event_base_loopexit( pThis->m_pEventBase, NULL );

            if (pThis->m_connectStat == CONNECTSTAT_CONNECTED)
            {
                pThis->m_connectStat = CONNECTSTAT_RECONNECTING;
            }
        }
    }
    else if ( events & BEV_EVENT_CONNECTED )
    {
        pThis->m_connectStat = CONNECTSTAT_CONNECTED;
    }
    else
    {
        Log_Error( "TennisBaseImpl::conn_eventcb %d\n", events );
    }

    if (lastConnectStat == CONNECTSTAT_INVALID)
    {
        pthread_mutex_lock( &pThis->m_pthread_mutex );
        pthread_cond_signal( &pThis->m_pthread_con );
        pthread_mutex_unlock( &pThis->m_pthread_mutex );
    }
}

void * TennisBaseImpl::conn_server( void *user_data )
{
    TennisBaseImpl *pThis = (TennisBaseImpl *)user_data;

    evthread_use_pthreads();

    bool isSuccess = false;
    do
    {
        pThis->m_pEventBase = event_base_new();
        if ( pThis->m_pEventBase == NULL )
        {
            Log_Error( "TennisBaseImpl::failed to create event!\n" );
            break;
        }

        pThis->m_pBufferEvent = bufferevent_socket_new( pThis->m_pEventBase, -1, BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE ); //BEV_OPT_THREADSAFE BEV_OPT_CLOSE_ON_FREE
        if ( pThis->m_pBufferEvent == NULL )
        {
            Log_Error( "TennisBaseImpl:: failed to create socket!\n" );
            break;
        }

        bufferevent_setcb( pThis->m_pBufferEvent, conn_readcb, NULL, conn_eventcb, pThis );
        bufferevent_enable( pThis->m_pBufferEvent, EV_READ | EV_WRITE );

        memset( &pThis->m_sockAddr, 0, sizeof( pThis->m_sockAddr ) );
        pThis->m_sockAddr.sin_family = AF_INET;
        pThis->m_sockAddr.sin_addr.s_addr = inet_addr( DEFAULT_HOST_IP_STR );
        pThis->m_sockAddr.sin_port = htons( DEFAULT_HOST_PORT );
        if ( bufferevent_socket_connect( pThis->m_pBufferEvent, (struct sockaddr *)&pThis->m_sockAddr, sizeof( pThis->m_sockAddr ) ) != 0 )
        {
            Log_Error( "TennisBaseImpl:: failed to create socket connection!\n" );
            break;
        }

        isSuccess = true;
    } while (false);

    if ( !isSuccess )
    {
        if ( pThis->m_pBufferEvent != NULL )
        {
            bufferevent_free( pThis->m_pBufferEvent );
            pThis->m_pBufferEvent = NULL;
        }

        if ( pThis->m_pEventBase != NULL )
        {
            event_base_free( pThis->m_pEventBase );
            pThis->m_pEventBase = NULL;
        }

        pthread_mutex_lock( &pThis->m_pthread_mutex );
        pthread_cond_signal( &pThis->m_pthread_con );
        pthread_mutex_unlock( &pThis->m_pthread_mutex );

        return NULL;
    }

    pThis->onThread(false);
    event_base_loop( pThis->m_pEventBase, 0 );
    pThis->onThread(true);

    Log_Error( "TennisBaseImpl::thread exit\n" );

    event_base_free( pThis->m_pEventBase );
    pThis->m_pEventBase = NULL;

    if (pThis->m_connectStat == CONNECTSTAT_DISPOSE)
    {
        pthread_mutex_lock( &pThis->m_pthread_mutex );
        pthread_cond_signal( &pThis->m_pthread_con );
        pthread_mutex_unlock( &pThis->m_pthread_mutex );
    }
    else if (pThis->m_connectStat == CONNECTSTAT_RECONNECTING)
    {
        // TODO 不属于正常stop退出，告诉上层，底层退出，需要重连
    }

    return pThis;
}