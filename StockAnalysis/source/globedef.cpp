#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSystemMessage    gl_systemMessage;         // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime       gl_systemTime;            // ϵͳʱ����ܡ�
CSystemDequeData  gl_systemDequeData;       // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
CSystemStatus     gl_systemStatus;          // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��

CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ���Զ��״̬������ͬʱ�������ݿ�ʱ�ٶ�������


deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRelativeStrong = false;
bool gl_fResetSystem = false; // ϵͳ������ʶ

long gl_lShowGuaDanTransaction = __ORDINARY_BUY__; // ��ʾ�ҵ��ɽ��������

CMarket gl_ChinaStockMarket; // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�

clock_t gl_RTReadingTime;
clock_t gl_DayLineReadingTime;

bool gl_fExitCalculatingRTData = false;
