#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SinaRTWebData.h"
//#include"TengxunRTWebData.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

extern CSinaRTWebData     gl_SinaRTWebData;     // ����ʵʱ���ݲɼ�
//extern CTengxunRTWebData  gl_TengxunRTWebData;  // ��Ѷʵʱ���ݲɼ�

extern CSystemMessage     gl_systemMessage;     // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime        gl_systemTime;        // ϵͳʱ����ܡ�
extern CQueueRTData       gl_QueueRTData;       // ϵͳʵʱ���ݶ��С�
extern CThreadStatus      gl_ThreadStatus;      // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern deque<CString> gl_dequeRTStockInquire;   // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

typedef struct stDayLine {    // �������ݽṹ
  stDayLine() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }

  CString strInquire;       // ������ȡ���ݵ��ַ���
  char buffer[2048 * 1024]; // ��ȡ����������ʷ���ݻ�����
  long lByteRead;           // ���ζ������ֽ���
  bool fError;              // ���δ������������Ƿ�����ı�ʶ

protected:
} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire;      // ����������ʷ���ݻ���

typedef struct stRTData {
  stRTData() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }
  CString strInquire;
  char buffer[2048 * 1024];
  long lByteRead;
  bool fError;
} RTDataInquire;

//extern RTDataInquire gl_stSinaRTDataInquire;    // ����ʵʱ�������ݻ���
extern RTDataInquire gl_stTengxunRTDataInquire; // ��Ѷʵʱ�������ݻ���

extern bool gl_fExiting;                        //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS;           // ����֪ͨ�����߳��˳����ź�
extern bool gl_fResetSystem;                    // ϵͳ������ʶ

extern bool gl_fNormalMode;                     // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
extern bool gl_fTestMode;                       // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

class CMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket;        // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

#endif
