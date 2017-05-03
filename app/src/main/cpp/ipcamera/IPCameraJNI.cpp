#include <string.h>
#include <jni.h>
#include <stdio.h>
#include "IPCamera.h"

static IPCamera *s_pIPCameraA = NULL;
static IPCamera *s_pIPCameraB = NULL;

std::string getJStringToChar( JNIEnv *env, jstring str );

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_init
    ( JNIEnv *env, jobject thiz, jstring tmpPath, jstring ip, jshort port, jstring userName, jstring password, jint courtArea )
{
    IPCamera *pIPCamera = NULL;
    switch(courtArea)
    {
        case COURTAREA_A:
        {
            if ( s_pIPCameraA == NULL )
            {
                s_pIPCameraA = new IPCamera();
                if ( s_pIPCameraA == NULL )
                    return (jboolean)false;
            }

            pIPCamera = s_pIPCameraA;
        }
        break;
        case COURTAREA_B:
        {
            if ( s_pIPCameraB == NULL )
            {
                s_pIPCameraB = new IPCamera();
                if ( s_pIPCameraB == NULL )
                    return (jboolean)false;
            }

            pIPCamera = s_pIPCameraB;
        }
        break;
        default:
            return false;
    }


    std::string strTmpPath = getJStringToChar( env, tmpPath );
    std::string strIP = getJStringToChar( env, ip );
    std::string strUserName = getJStringToChar( env, userName );
    std::string strPassword = getJStringToChar( env, password );
    return (jboolean)pIPCamera->init( strTmpPath.c_str(), strIP.c_str(), port, strUserName.c_str(), strPassword.c_str() );
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zealens_face_core_internal_IPCamera_dispose
    ( JNIEnv *env, jobject thiz, jint courtArea )
{
    switch(courtArea)
    {
        case COURTAREA_A:
        {
            if ( s_pIPCameraA != NULL )
            {
                s_pIPCameraA->dispose();
                s_pIPCameraA = NULL;
            }
        }
        break;
        case COURTAREA_B:
        {
            if ( s_pIPCameraB != NULL )
            {
                s_pIPCameraB->dispose();
                s_pIPCameraB = NULL;
            }
        }
        break;
        default:
            break;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_startRecord
    ( JNIEnv *env, jobject thiz, jstring savePath, jint courtArea  )
{
    std::string strSavePath = getJStringToChar( env, savePath );
    switch(courtArea)
    {
        case COURTAREA_A:
            return (jboolean)s_pIPCameraA->startRecord( strSavePath.c_str() );
        case COURTAREA_B:
            return (jboolean)s_pIPCameraB->startRecord( strSavePath.c_str() );
        default:
            break;
    }

    return false;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_stopRecord
    ( JNIEnv *env, jobject thiz, jint courtArea  )
{
    switch(courtArea)
    {
        case COURTAREA_A:
            return (jboolean)s_pIPCameraA->stopRecord();
        case COURTAREA_B:
            return (jboolean)s_pIPCameraB->stopRecord();
        default:
            break;
    }

    return false;
}

std::string getJStringToChar( JNIEnv *env, jstring str )
{
    const char  *buffer = env->GetStringUTFChars( str, NULL );
    std::string str2( buffer );
    env->ReleaseStringUTFChars( str, buffer );

    return str2;
}