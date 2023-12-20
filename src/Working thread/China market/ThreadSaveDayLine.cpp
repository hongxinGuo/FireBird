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

#include "ChinaMarket.h"
#include"ThreadStatus.h"
#include"ChinaStock.h"

UINT ThreadSaveDayLineBasicInfo(const not_null<CChinaStockPtr>& pStock) {
	gl_UpdateChinaMarketDB.acquire();
	if (!gl_systemConfiguration.IsExitingSystem()) {
		const bool fDataSaved = pStock->SaveDayLineBasicInfo();
		pStock->UpdateDayLineStartEndDate();
		if (fDataSaved) {
			const CString str = pStock->GetSymbol() + _T("�������ϴ洢���");
			gl_systemMessage.PushDayLineInfoMessage(str);
		}
		pStock->UnloadDayLine();// Ϊ��ֹ����ͬ�����⣬ж��������ʷ���ݵ�����Ҳ�ɱ��߳�ִ�С�
	}
	gl_UpdateChinaMarketDB.release();

	return 15;
}
