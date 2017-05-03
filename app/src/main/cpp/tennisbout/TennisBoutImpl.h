#ifndef __TENNISBOUT_IMPL_H__
#define __TENNISBOUT_IMPL_H__

#include <TennisBaseImpl.h>

class TennisBoutImpl
    : public TennisBaseImpl {
public:
    TennisBoutImpl();

    virtual ~TennisBoutImpl();

public:
    virtual bool TENNIS_STDCALL init( const InitParam &initParam, I_BoutCallback *pCallback );

    virtual bool TENNIS_STDCALL ballPower( BallPower ballPower );

    virtual void TENNIS_STDCALL readcb( struct bufferevent *bev, void *user_data );

    virtual void TENNIS_STDCALL onThread(bool isExit);

private:
    I_BoutCallback *m_pBoutCallback;

    //read data
    struct BoutPackage *m_pRecPackage;
};

#endif