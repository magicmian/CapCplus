/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* TODO: add self define begin*/
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h>  //����������char *inet_ntoa (struct in_addr);
#include <netdb.h>      //gethostbyname
#include <time.h>       //struct timeval
//#ifdef _MY_OPENSSL_
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"
#include "openssl/crypto.h"
#include <ossl_typ.h>
//#endif
using namespace std;

//#ifdef _MY_OPENSSL_
SSL     *g_pSSL;
SSL_CTX *g_pSSLContex;
//#endif
int g_iSocketClient;

void ClearEnv();
void SslGetCipherAndCertification();
bool ConnectToServer(const string &astrServerUrl, const int aiPort = 443);
bool InteractiveByPost(const string &astrSendData, const string &astrServiceName, string &astrRecvData);
/* TODO: add self define end*/

/* Header for class neusoft_www_capcplus_NativeHelper */

#ifndef _Included_neusoft_www_capcplus_NativeHelper
#define _Included_neusoft_www_capcplus_NativeHelper
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     neusoft_www_capcplus_NativeHelper
 * Method:    getAppKey
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getAppKey
  (JNIEnv *, jclass);

/*
 * Class:     neusoft_www_capcplus_NativeHelper
 * Method:    getSha1
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getSha1
  (JNIEnv *, jclass, jstring);

/*
 * Class:     neusoft_www_capcplus_NativeHelper
 * Method:    getSha2
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getSha2
  (JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif
