


#include "neusoft_www_capcplus_NativeHelper.h"


void ClearEnv()
{
    /*����򿪵ľ��*/
#ifdef _MY_OPENSSL_
    if(NULL != g_pSSL)
    {
        SSL_shutdown(g_pSSL);
        close(g_iSocketClient);
        SSL_free(g_pSSL);
        g_pSSL = NULL;
    }
    if(NULL != g_pSSLContex)
    {
        SSL_CTX_free(g_pSSLContex);
        g_pSSLContex = NULL;
    }
#endif
    if(g_iSocketClient > 0)
    {
        ;//close(g_iSocketClient);
    }
}

void SslGetCipherAndCertification()
{
#ifdef _MY_OPENSSL_
    X509 *cert;
    char *line;
    char *line2;
    char sTemp[512] = {0};

    cert = SSL_get_peer_certificate(g_pSSL);
    //cert = SSL_get_certificate(g_pSSL);//�������������֤��
    if (cert != NULL)
    {
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        memset(sTemp, 0, sizeof(sTemp));
        sprintf(sTemp, "֤��: %s", line);
        #ifdef _MY_DEBUG_
            cout << sTemp << endl;
        #endif
        line2 = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        memset(sTemp, 0, sizeof(sTemp));
        sprintf(sTemp, "�䷢��: %s", line2);
        #ifdef _MY_DEBUG_
            cout << sTemp << endl;
        #endif
        X509_free(cert);
    }
    else
    {
        ;
        #ifdef _MY_DEBUG_
            cout << "��֤����Ϣ��" << endl;
        #endif
    }
#endif
}


/*����https������������ֵ>0��ʾ�ͻ���socket -1��ʾʧ��*/
bool ConnectToServer(const string &astrServerUrl, const int aiPort)
{
    /*1. ����socket����*/
    g_iSocketClient = -1;
    struct sockaddr_in server;

    struct hostent *pkHostEnt;
    pkHostEnt = gethostbyname(astrServerUrl.c_str());
    if (NULL == pkHostEnt)
    {
        return false;
    }
    #ifdef _MY_DEBUG_
        cout << "hostname=[" << pkHostEnt->h_name << "]" << endl;
        cout << "IpAddress=[" << inet_ntoa(*(struct in_addr *)*pkHostEnt->h_addr_list) << "]" << endl;
    #endif

    g_iSocketClient = socket(AF_INET, SOCK_STREAM, 0);
    if(g_iSocketClient == -1)
    {
        return false;
    }

    /*����sockaddr_in*/
    server.sin_family = AF_INET;
    server.sin_addr = *(struct in_addr *)*pkHostEnt->h_addr_list;
    server.sin_port = htons((unsigned short)aiPort);
    memset(server.sin_zero, 0, sizeof(server.sin_zero));

    /*2. SSL����*/
#ifdef _OPEN_SSL_
    OpenSSL_add_all_algorithms();   /*��SSL���г�ʼ��,��ʵ����SSL_library_init(),����һ����*/
    SSL_load_error_strings();       /*����SSL������Ϣ,Ϊ��ӡ������Ϣ��׼��*/

    const SSL_METHOD *pSSLMethod = SSLv23_client_method();  /*�������λỰ������ʹ�õ�Э��*/
    if(NULL == pSSLMethod)
    {
        return false;
    }

    g_pSSLContex = SSL_CTX_new(pSSLMethod);                 /*����SSL������, ����SSL�Ự�Ļ���*/
    if(NULL == g_pSSLContex)
    {
        ClearEnv();
        return false;
    }

    g_pSSL = SSL_new(g_pSSLContex);
    if(NULL == g_pSSL)
    {
        ClearEnv();
        return false;
    }

    /*SSL��ԭ��socket*/
    if(SSL_set_fd(g_pSSL, g_iSocketClient) == 0)
    {
        ClearEnv();
        return false;
    }
    /*���ӷ�����*/
    if(SSL_connect(g_pSSL) < 0)
    {
        ClearEnv();
        return false;
    }
    else
    {
        const char *sCipher = SSL_get_cipher(g_pSSL);
        SslGetCipherAndCertification();
        return true;
    }
#endif
    return true;
}


#ifdef _MY_OPENSSL_
jstring charTojstring(JNIEnv* env, const char* pat)
{
    //����java String�� strClass
    jclass strClass = (env)->FindClass("Ljava/lang/String;");
    //��ȡString(byte[],String)�Ĺ�����,���ڽ�����byte[]����ת��Ϊһ����String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //����byte����
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //��char* ת��Ϊbyte����
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // ����String, ������������,����byte����ת����Stringʱ�Ĳ���
    jstring encoding = (env)->NewStringUTF("GB2312");
    //��byte����ת��Ϊjava String,�����
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}


char *jstringTostring(JNIEnv* env, jstring jstr)
{
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}
#endif


bool InteractiveByPost(const string &astrSendData, const string &astrServiceName, string &astrRecvData)
{
    /*��֯������*/
    //string strTemp = "{\"SERVICE_ID\":\"18641166797\",\"PRODUCT_NAME\":\"�ſ���������\",\"PRODUCT_PRICE\":\"6Ԫ/��\",\"CLIENT_ID\":\"YfDJIu0k1K3DuDTSuGhb\",\"CLIENT\":\"1\",\"VERSION\":\"1.0.0\",\"CLIENT_SECRET\":\"12cb7a4c095b48afb21db3fbe0561415\",\"AUTH_NO\":\"152023151744232\"}";
    //string strSendParamUTF = GBKToUTF8(strTemp);
    string strSendParamUTF = astrSendData;
    string strSendParamLen;
    char sLen[6] = {0};
    sprintf(sLen, "%d", strSendParamUTF.length());
    strSendParamLen = sLen;
    bool bRet = false;
    //strSendData = "POST /cap/sms/";
    string strSendData = "POST " + astrServiceName;
    strSendData += " HTTP/1.1\r\n";
    strSendData += "Content-Type: application/json; charset=utf-8\r\n";
    strSendData += "Host: cap.chinaunicom.cn\r\n";
    strSendData += "Content-Length: " + strSendParamLen;
    strSendData += "\r\n\r\n";      /*HTTPͷ����,����"\r\n"*/
    strSendData += strSendParamUTF;
    /*����http������*/
    int iRet = 0;
#ifdef _OPEN_SSL_
    iRet = SSL_write(g_pSSL, (void *)strSendData.c_str(), strSendData.length());
#endif
    if(-1 == iRet)
    {
        return false;
    }

    /*����httpӦ����*/
    struct timeval waittime;
    waittime.tv_sec = 5;    /*��ʱʱ��5s*/
    waittime.tv_usec = 0;
    //while(1)
    //{
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(g_iSocketClient, &fds);

        char sRecvData[2048] = {0};
        int nRecvLen;

        /*iRet = select(FD_SETSIZE, &fds, NULL, NULL, &waittime);
        //select����=0-��ʱ <0����
        if(iRet == 0)
        {
            return false;//��ʱ
        }
        else if(iRet == 0)
        {
            return false;//����
        }
        else
        {
            ;
        }*/


        if(FD_ISSET(g_iSocketClient, &fds))
        {
#ifdef _OPEN_SSL_
            iRet = SSL_read(g_pSSL, sRecvData, sizeof(sRecvData));
#endif
            if(iRet <= 0)
            {
                return false;
            }
        }
        else
        {
            ;
            //break;
        }
        astrRecvData = sRecvData;
    //}//end while(1)
    return true;
}




JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getAppKey
  (JNIEnv * env, jclass){
    jstring result ;
    return result;
  }


JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getSha1
  (JNIEnv * env, jclass, jstring str){
    jstring result ;
    /*TODO: add impl begin*/
    //�����strת��Ϊutf8��ʽ�����str����json��
    //����˵���� const char *GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy);
    //����ָ���ַ�����UTF-8�ַ������ָ��
    const char *pReqParam = env->GetStringUTFChars(str, NULL);
    //��ת��Ϊutf8��ʽ��json������ֲ�����
    string strReqParam(pReqParam);
    string strResInfo = "";
    string strRetJSON = "";
    char sTemp[256] = {0};
    #ifdef _MY_DEBUG_
        cout << "��ʼ����https������https://cap.chinaunicom.cn" << endl;
    #endif
    if( !ConnectToServer("cap.chinaunicom.cn", 443) )  //��д�̶�ֵ
    {
        strcpy(sTemp, "{\"MSG\":\"���ӷ�����ʧ�ܣ�\",\"RSP\":{\"RSP_CODE\":\"1000\",\"RSP_DESC\":\"failure\"},\"STATUS\":\"0000\",\"TXID\":\"a6df^1527163592466^268771\"}");
        #ifdef _MY_DEBUG_
            cout << sTemp << endl;
        #endif
        #ifdef _MY_OPENSSL_
            result = charTojstring(env, sTemp);
        #endif
        return result;
    }
    #ifdef _MY_DEBUG_
        cout << "����https������https://cap.chinaunicom.cn�ɹ���" << endl;
        cout << "����/cap/sms/��������JSON��: " << pReqParam << endl;
    #endif

    strReqParam = pReqParam;
    if( !InteractiveByPost(strReqParam, "/cap/sms/", strResInfo) )
    {
        strcpy(sTemp, "{\"MSG\":\"�������ʧ�ܣ�\",\"RSP\":{\"RSP_CODE\":\"2000\",\"RSP_DESC\":\"failure\"},\"STATUS\":\"0000\",\"TXID\":\"a6df^1527163592466^268771\"}");
        #ifdef _MY_DEBUG_
            cout << sTemp << endl;
        #endif
        #ifdef _MY_OPENSSL_
            result = charTojstring(env, sTemp);
        #endif
        return result;
    }
    #ifdef _MY_DEBUG_
        cout << "����/cap/sms/���������������Ϣ: " << strResInfo << endl;
    #endif

    if(strResInfo.find("HTTP/1.1 200") != string::npos)
    {
        int iPos = strResInfo.find("{\"MSG\"");
        int iLen = strResInfo.length();
        strRetJSON = strResInfo.substr(iPos, iLen-iPos);
        #ifdef _MY_DEBUG_
            cout << "������óɹ���http����200Ӧ��JSON��: " << strRetJSON << endl;
        #endif
        memset(sTemp, 0, sizeof(sTemp));
        strcpy(sTemp, strRetJSON.c_str());
    }
    else
    {
        strcpy(sTemp, "{\"MSG\":\"������óɹ���http���ط�200��\",\"RSP\":{\"RSP_CODE\":\"3000\",\"RSP_DESC\":\"failure\"},\"STATUS\":\"0000\",\"TXID\":\"a6df^1527163592466^268771\"}");
    }
    #ifdef _MY_OPENSSL_
        result = charTojstring(env, sTemp);
    #endif
    ClearEnv();
    /*TODO: add impl end*/
    return result;
  }

JNIEXPORT jstring JNICALL Java_neusoft_www_capcplus_NativeHelper_getSha2
  (JNIEnv * env, jclass, jstring str){
    jstring result ;
    return result;
  }


