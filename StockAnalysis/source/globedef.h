#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SystemMessage.h"
#include"SystemDequeData.h"
#include"SystemStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

class CMarket;


extern vector<StockIDPtr>	gl_vTotalStock;             // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
extern map<CString, long>	gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

extern vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��


extern CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ���Զ��״̬������ͬʱ�������ݿ�ʱ�ٶ�������

extern CSystemTime        gl_systemTime;         // ϵͳʱ����ܡ�
extern CSystemMessage     gl_systemMessage;      // ϵͳ��Ϣ�����࣬�����������߳���ʹ��
extern CSystemDequeData   gl_systemDequeData;    // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
extern CSystemStatus      gl_systemStatus;       // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

typedef struct stDayLine {    // �������ݽṹ
  stDayLine() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }

  CString strInquire;   // ������ȡ���ݵ��ַ���
  char buffer[2048 * 1024]; // ��ȡ����������ʷ���ݻ�����
  long lByteRead;           // ���ζ������ֽ���
  bool fError;              // ���δ������������Ƿ�����ı�ʶ

protected:

} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire; 

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

extern RTDataInquire gl_stRTDataInquire;

extern bool gl_fExiting;
extern bool gl_fExitingCalculatingRelativeStrong;

extern long gl_lShowGuaDanTransaction; // ��ʾ�ҵ��ɽ��������

// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket;        // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

#endif
