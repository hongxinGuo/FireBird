#include"globedef.h"

#include"StockID.h"
#include "Stock.h"
//#include "RTData.h"
#include "Market.h"
#include "CurrentTime.h"
#include "MutualLock.h"

CMarket gl_sMarket;

CCurrentTime gl_sTime;

CMutualLock gl_sMutualLock;

CString gl_sCurrentBaseDir = _T("c:\\SmartStockDataBase\\"); ;
CString gl_sDataBaseDir = _T("C:\\SmartStockDataBase\\DataBase\\");

CString gl_strRTStockSource = _T("http://hq.sinajs.cn/list=");
DWORD		gl_dwRTDataHttpFileStatus = 0;

CString gl_strDayLineStockSource = _T("http://quotes.money.163.com/service/chddata.html?code=");
CString gl_strDayLinePostfix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
DWORD		gl_dwDayLineHttpFileStatus = 0;

deque<CStockRTDataPtr> gl_dequeRTStockData;

vector<StockIDPtr>		gl_vTotalStock;
map<CString, long>		gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��


CString		gl_strCurrentStockDownLoading;

long gl_lToday;           // �������ڡ���ʽΪ��19990102
long gl_lLastTradeDay;  // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
long gl_lTime;				  // ����ʱ�䡣��ʽΪ��hhmmss
tm gl_tm;						    // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
time_t gl_ttime;        // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
bool gl_fMarketOpened = false; // �г��Ƿ��ڽ���ʱ��

long gl_lRelativeStrongStartDay;
long gl_lRelativeStrongEndDay;


CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ�

CSystemMessage gl_systemMessage;       // ϵͳ��Ϣ������

deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�



DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�

long gl_lOption_CalculatingRelativeStrongStartDay = 20190620;
bool gl_fExitingCalculatingRelativeStrong = false;



