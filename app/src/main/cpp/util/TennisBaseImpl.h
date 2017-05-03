#ifndef __TENNISBASE_IMPL_H__
#define __TENNISBASE_IMPL_H__

#include <I_TennisSocket.h>

class TennisBaseImpl
    : public I_TennisBase {
public:
    TennisBaseImpl();

    virtual ~TennisBaseImpl();

public:
    virtual bool TENNIS_STDCALL init( const InitParam &initParam );

    virtual bool TENNIS_STDCALL start();

    virtual bool TENNIS_STDCALL pause();

    virtual bool TENNIS_STDCALL resume();

    virtual bool TENNIS_STDCALL stop();

    virtual bool TENNIS_STDCALL runOnce();

    virtual bool TENNIS_STDCALL restart( CourtArea serveArea );

    virtual void TENNIS_STDCALL readcb( struct bufferevent *bev, void *user_data ) = 0;

    virtual void TENNIS_STDCALL onThread(bool isExit) = 0;

protected:
    void dispose();
    bool connectServer();
    timespec* init_timeout();

private:
    static void  conn_readcb( struct bufferevent *bev, void *user_data );
    static void  conn_eventcb( struct bufferevent *bev, short events, void *user_data );
    static void* conn_server( void *user_data );

public:
    virtual unsigned int TENNIS_STDCALL addRef()
    {
        return ++m_uRefCount;
    }

    virtual unsigned int TENNIS_STDCALL releaseRef()
    {
        if ( --m_uRefCount == 0 )
        {
            delete this;
            return 0;
        }

        return m_uRefCount;
    }

protected:
    enum ConnectStat{
        CONNECTSTAT_INVALID,
        CONNECTSTAT_DISPOSE,
        CONNECTSTAT_CONNECTED,
        CONNECTSTAT_RECONNECTING,
    };

protected:
    unsigned int m_uRefCount;

    bool m_isSuccess;
    ConnectStat m_connectStat;

    //relate to libevent
    struct event_base  *m_pEventBase;
    struct sockaddr_in m_sockAddr;
    struct bufferevent *m_pBufferEvent;

    //write data
    struct SocketClientPackage *m_pSocketPackage;

    //pthread
    pthread_t m_uThreadId;
    pthread_cond_t  m_pthread_con;
    pthread_mutex_t m_pthread_mutex;

    size_t m_packageOffset;
};

#endif

