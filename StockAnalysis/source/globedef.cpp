#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSystemMessage    gl_systemMessage;         // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime       gl_systemTime;            // ϵͳʱ����ܡ�
CSystemDequeData  gl_systemDequeData;       // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
CSystemStatus     gl_systemStatus;          // ϵͳ�еĸ���״̬�������������߳���ʹ�á�


DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRelativeStrong = false; // ����֪ͨ�����߳��˳����ź�
bool gl_fResetSystem = false; // ϵͳ������ʶ

long gl_lShowGuaDanTransaction = __ORDINARY_BUY__; // ��ʾ�ҵ��ɽ��������

CMarket gl_ChinaStockMarket; // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
