#ifndef __TENNISDRILL_IMPL_H__
#define __TENNISDRILL_IMPL_H__

#include <TennisBaseImpl.h>

class TennisDrillImpl
    : public TennisBaseImpl {
public:
    TennisDrillImpl();

    virtual ~TennisDrillImpl();

public:
    virtual bool TENNIS_STDCALL init( const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty );
    virtual void TENNIS_STDCALL readcb( struct bufferevent *bev, void *user_data );
    virtual void TENNIS_STDCALL onThread(bool isExit);

private:
    I_DrillCallback *m_pDrillCallback;

    //read data
    struct DrillPackage *m_pRecPackage;
};

#endif

