/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

UINT ThreadLoadWeekLine(const not_null<CChinaStockPtr>& pStockInput) {
	const CChinaStockPtr pStock = pStockInput;
	pStock->UnloadWeekLine();
	// װ����������
	pStock->LoadWeekLine();
	// ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
	pStock->CalculateWeekLineRSIndex();
	pStock->SetWeekLineLoaded(true);

	return 29;
}
