#include"pch.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(const not_null<CChinaStockPtr>& pStockInput) {
	const auto pStock = pStockInput;
	pStock->UnloadDayLine();
	// װ����������
	pStock->LoadDayLine(pStock->GetSymbol());
	// ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
	pStock->CalculateDayLineRSIndex();
	pStock->SetDayLineLoaded(true);

	return 16;
}
