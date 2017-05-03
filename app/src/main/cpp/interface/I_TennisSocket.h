#ifndef __INTERFACE_TENNISSOCKET_H__
#define __INTERFACE_TENNISSOCKET_H__

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>
#include <assert.h>

#include <pthread.h>

#include <I_TennisBout.h>
#include <I_TennisDrill.h>

#define DEFAULT_HOST_PORT 9080
#define DEFAULT_HOST_IP_STR "192.168.1.4"
#define DEFAULT_HOST_ADDR_STR "192.168.1.4:9080"

enum ClientCommand {
    CLIENT_INIT,
    CLIENT_START,
    CLIENT_PAUSE,
    CLIENT_RESUME,
    CLIENT_STOP,
    CLIENT_RUNONCE,
    CLIENT_RESTART,
    CLIENT_BALLPOWER,
};

enum ServerBoutCommand {
    BOUT_STATE,
    BOUT_ON_BALL,
    BOUT_ON_BALLACE,
    BOUT_ON_SERVE,
    BOUT_ON_SERVE_FAILED,
    BOUT_ON_SERVE_SUCCESSFUL,
    BOUT_ON_SERVE_TOUCH_DOWN_POSITION,
    BOUT_ON_HIT_POSITION,
    BOUT_ON_ADD_SCORE,
    BOUT_ON_ERROR,
    BOUT_ON_THREAD,
};

enum ServerDrillCommand {
    DRILL_STATE,
    DRILL_ON_BEGIN,
    DRILL_ON_DATA_CHANGE,
    DRILL_ON_SPEED_CHANGE,
    DRILL_ON_END,
    DRILL_ON_DRILL_NEXT,
    DRILL_ON_ERROR,
    DRILL_ON_THREAD,
};

#pragma pack(push, 4)
struct SocketInitParam {
    PlayModel playModel;                //Referrence: InitParam
    CourtArea servePosition;            //Referrence: InitParam
    CourtSize courtSize;                //Referrence: InitParam

    char szAppPath[512];                //Referrence: InitParam
    char szCfgPath[512];                //Referrence: InitParam
    char szTmpPath[512];                //Referrence: InitParam
};

struct SocketClientPackage {
    ClientCommand clientCommand;                   //command flag: start, stop, resume...
    SocketInitParam    socketInitParam;

    union {
        struct {
            DrillDifficulty difficulty; //Drill mode Difficulty
        } Drill;

        struct {
            CourtArea courtArea;        //Drill restart() Param
        } Restart;

        struct {
            BallPower ballPower;        //Bout BallPower
        } BallPower;
    };
};

struct BoutPackage {
    ServerBoutCommand serverBoutCommand;              //command type

    union
    {
        struct {
            Ball ball;
        } OnBall;

        struct {
            Player player;
        } OnBallAce;

        struct {
            Ball ball;
        } OnServe;

        struct {
            Player player;
            PlayerServe serveType;
            Ball ball;
        } OnServeTouchDownPosition;

        struct {
            Ball   ball;
            Player player;
        } OnHitPosition;

        struct {
            Player player;
            MatchBoxScore matchBoxScoreA;
            MatchBoxScore matchBoxScoreB;
            Ball ball;
        } OnAddScore;

        struct {
            int  code;
            char errorMessage[512];
        } OnError;

        struct {
            bool isExit;
        } OnThread;
    };

    bool isSuccess;
};

struct DrillPackage {
    ServerDrillCommand serverDrillCommand;
    DrillBoxScore drillBoxScore;

    union
    {
        struct {
            BallPoint ballPointHit[128];
            BallPoint ballPointDown[128];
        } OnDataChange;

		// TODO ����������ݿ��ܻ�Ƚ�Ƶ�����������OnDataChange���ݣ����ܻ�������ƿ��
        struct {
            int speed;
        } OnSpeedChange;

        struct {
            int  code;
            char errorMessage[512];
        } OnError;

        struct {
            bool isExit;
        } OnThread;
    };

	bool isSuccess;
};

#pragma pack(pop)

#endif //__INTERFACE_TENNISSOCKET_H__