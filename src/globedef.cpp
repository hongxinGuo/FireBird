//////////////////////////////////////////////////////////////////////////////////////
//
// Ϊ�˱�֤����ĳ�ʼ�������е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"
#include"NeteaseDayLineDataSource.h"
#include"NeteaseRTDataSource.h"
#include"QuandlDataSource.h"
#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"TengxunDayLineDataSource.h"
#include"TiingoDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoIEXWebSocket.h"

#include"ThreadStatus.h"

#include "ChinaMarket.h"

bool gl_bGlobeVariableInitialized = false;

// ��ΪΨһʵ��
CSystemConfiguration gl_systemConfiguration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����
CSystemStatus gl_systemStatus; // ϵͳ״̬��
CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��

CFinnhubInquiryType gl_FinnhubInquiryType;

CHighPerformanceCounter gl_counter;
time_t gl_tUTC = 0; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub��ֹ���ʽ�����������˻��޷����ʵĽ��������ݣ���Ψһʵ��

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
binary_semaphore gl_UpdateWorldMarketDB{1}; // ���ڸ���WorldMarket���ݿ�
counting_semaphore<4> gl_SaveDayLineThreadPermitted{1}; // �����ݿ���û����������ʱ�����Ӵ��ź������ֵ��4��ʹ���ĸ��߳��Լӿ��������ݴ洢�ٶ�
counting_semaphore<8> gl_BackGroundTaskThread{8}; // ��̨�����߳��������Ϊ8
binary_semaphore gl_ProcessChinaMarketRTData{1}; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
counting_semaphore<3> gl_WebSourceParseAndStoreData{3};//���ڽ���WebSource�е����ݡ���ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������

CSystemData gl_SystemData;

// Data source, ��ΪΨһʵ��
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource = nullptr;
CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource = nullptr;
CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource = nullptr;
CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;
CQuandlDataSourcePtr gl_pQuandlDataSource = nullptr;

// Web socket�� ��ΪΨһʵ����
CFinnhubWebSocket gl_finnhubWebSocket;
CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
CTiingoForexWebSocket gl_tiingoForexWebSocket;
CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;

// ���г�����ΪΨһʵ����Ϊ��������ı�����
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask
