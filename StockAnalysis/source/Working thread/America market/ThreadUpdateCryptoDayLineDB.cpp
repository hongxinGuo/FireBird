//////////////////////////////////////////////////////////////////////////////////////
//
// �洢������ʷ���ݿ�ʱ�����ƻ���ͬ�����⣬���µ����ݿ��д��ھ�����ʱ�����µ���;�ͻᷢ�����ݿ�ͬ�����Ļ��⣬
// �����Ҷ�MySQL���ݿⲻ̫��Ϥ���޷��ҵ�����������ʶ����������ź������ֱ����������ݿ�Ϊ�գ�ϵͳ����)ʱ,
// ʹ�ö��̷߳�ʽ�洢���ݣ��������ݿ��д��ھ�����ʱ��ʹ�õ�һ�̴߳洢��
// ���ַ�ʽ����Ӱ���ٶȣ���Ϊ��������ʱ���������Ǻ�С�ġ�
//
// ���յĽ������������Ҫ�о��������ͬ�����⡣
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"FinnhubCryptoSymbol.h"

UINT ThreadUpdateCryptoDayLineDB(not_null<CFinnhubCryptoSymbol*> pSymbol) {
	CString str;

	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pSymbol->UpdateDayLineDB();
	pSymbol->UpdateDayLineStartEndDate();
	pSymbol->SetUpdateProfileDB(true);
	pSymbol->UnloadDayLine();
	str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
	gl_systemMessage.PushDayLineInfoMessage(str);
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 53;
}