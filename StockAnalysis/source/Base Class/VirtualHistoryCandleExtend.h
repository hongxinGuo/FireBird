////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й���Ʊʵ�����ݣ����ߡ����ߵȣ��Ļ��ࡣ
// �����࣬����������ʵ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualHistoryCandleBasic.h"
#include"VirtualSetHistoryCandleExtend.h"
#include"VirtualSetHistoryCandle.h"

using namespace std;
#include<memory>

class CVirtualHistoryCandleExtend;
typedef shared_ptr<CVirtualHistoryCandleExtend> CVirtualHistoryCandleExtendPtr;

class CVirtualHistoryCandleExtend : public CVirtualHistoryCandleBasic {
public:
	CVirtualHistoryCandleExtend();
	~CVirtualHistoryCandleExtend();
	void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��
	virtual int GetRatio(void) const override final { return 1000; };

	virtual bool SaveHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend); // �洢��չ����
	virtual bool AppendHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend);
	virtual bool LoadHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend);

	virtual bool SaveHistoryCandle(CVirtualSetHistoryCandle* pSet); // �洢���е�����
	virtual bool AppendHistoryCandle(CVirtualSetHistoryCandle* pSet);
	virtual bool LoadHistoryCandle(CVirtualSetHistoryCandle* pSet);

public:
	INT64 GetTransactionNumber(void) const noexcept { return m_lTransactionNumber; }
	void SetTransactionNumber(INT64 lValue) noexcept { m_lTransactionNumber = lValue; }

	INT64 GetTransactionNumberBelow5000(void) const noexcept { return m_lTransactionNumberBelow5000; }
	void SetTransactionNumberBelow5000(INT64 lValue) noexcept { m_lTransactionNumberBelow5000 = lValue; }
	INT64 GetTransactionNumberBelow50000(void) const noexcept { return m_lTransactionNumberBelow50000; }
	void SetTransactionNumberBelow50000(INT64 lValue) noexcept { m_lTransactionNumberBelow50000 = lValue; }
	INT64 GetTransactionNumberBelow200000(void) const noexcept { return m_lTransactionNumberBelow200000; }
	void SetTransactionNumberBelow200000(INT64 lValue) noexcept { m_lTransactionNumberBelow200000 = lValue; }
	INT64 GetTransactionNumberAbove200000(void) const noexcept { return m_lTransactionNumberAbove200000; }
	void SetTransactionNumberAbove200000(INT64 lValue) noexcept { m_lTransactionNumberAbove200000 = lValue; }

	INT64 GetAttackBuyVolume(void) const noexcept { return m_lAttackBuyVolume; }
	void SetAttackBuyVolume(INT64 lValue) noexcept { m_lAttackBuyVolume = lValue; }
	INT64 GetStrongBuyVolume(void) const noexcept { return m_lStrongBuyVolume; }
	void SetStrongBuyVolume(INT64 lValue) noexcept { m_lStrongBuyVolume = lValue; }
	INT64 GetAttackSellVolume(void) const noexcept { return m_lAttackSellVolume; }
	void SetAttackSellVolume(INT64 lValue) noexcept { m_lAttackSellVolume = lValue; }
	INT64 GetStrongSellVolume(void) const noexcept { return m_lStrongSellVolume; }
	void SetStrongSellVolume(INT64 lValue) noexcept { m_lStrongSellVolume = lValue; }
	INT64 GetUnknownVolume(void) const noexcept { return m_lUnknownVolume; }
	void SetUnknownVolume(INT64 lValue) noexcept { m_lUnknownVolume = lValue; }
	INT64 GetCanceledBuyVolume(void) const noexcept { return m_lCanceledBuyVolume; }
	void SetCanceledBuyVolume(INT64 lValue) noexcept { m_lCanceledBuyVolume = lValue; }
	INT64 GetCanceledSellVolume(void) const noexcept { return m_lCanceledSellVolume; }
	void SetCanceledSellVolume(INT64 lValue) noexcept { m_lCanceledSellVolume = lValue; }

	INT64 GetOrdinaryBuyVolume(void) const noexcept { return m_lOrdinaryBuyVolume; }
	void SetOrdinaryBuyVolume(INT64 lValue) noexcept { m_lOrdinaryBuyVolume = lValue; }
	INT64 GetAttackBuyBelow50000(void) const noexcept { return m_lAttackBuyBelow50000; }
	void SetAttackBuyBelow50000(INT64 lValue) noexcept { m_lAttackBuyBelow50000 = lValue; }
	INT64 GetAttackBuyBelow200000(void) const noexcept { return m_lAttackBuyBelow200000; }
	void SetAttackBuyBelow200000(INT64 lValue) noexcept { m_lAttackBuyBelow200000 = lValue; }
	INT64 GetAttackBuyAbove200000(void) const noexcept { return m_lAttackBuyAbove200000; }
	void SetAttackBuyAbove200000(INT64 lValue) noexcept { m_lAttackBuyAbove200000 = lValue; }
	INT64 GetOrdinarySellVolume(void) const noexcept { return m_lOrdinarySellVolume; }
	void SetOrdinarySellVolume(INT64 lValue) noexcept { m_lOrdinarySellVolume = lValue; }
	INT64 GetAttackSellBelow50000(void) const noexcept { return m_lAttackSellBelow50000; }
	void SetAttackSellBelow50000(INT64 lValue) noexcept { m_lAttackSellBelow50000 = lValue; }
	INT64 GetAttackSellBelow200000(void) const noexcept { return m_lAttackSellBelow200000; }
	void SetAttackSellBelow200000(INT64 lValue) noexcept { m_lAttackSellBelow200000 = lValue; }
	INT64 GetAttackSellAbove200000(void) const noexcept { return m_lAttackSellAbove200000; }
	void SetAttackSellAbove200000(INT64 lValue) noexcept { m_lAttackSellAbove200000 = lValue; }

	INT64 GetOrdinaryBuyNumberBelow5000(void) const noexcept { return m_lOrdinaryBuyNumberBelow5000; }
	INT64 GetOrdinaryBuyNumberBelow10000(void) const noexcept { return m_lOrdinaryBuyNumberBelow10000; }
	INT64 GetOrdinaryBuyNumberBelow20000(void) const noexcept { return m_lOrdinaryBuyNumberBelow20000; }
	INT64 GetOrdinaryBuyNumberBelow50000(void) const noexcept { return m_lOrdinaryBuyNumberBelow50000; }
	INT64 GetOrdinaryBuyNumberBelow100000(void) const noexcept { return m_lOrdinaryBuyNumberBelow100000; }
	INT64 GetOrdinaryBuyNumberBelow200000(void) const noexcept { return m_lOrdinaryBuyNumberBelow200000; }
	INT64 GetOrdinaryBuyNumberAbove200000(void) const noexcept { return m_lOrdinaryBuyNumberAbove200000; }
	INT64 GetOrdinarySellNumberBelow5000(void) const noexcept { return m_lOrdinarySellNumberBelow5000; }
	INT64 GetOrdinarySellNumberBelow10000(void) const noexcept { return m_lOrdinarySellNumberBelow10000; }
	INT64 GetOrdinarySellNumberBelow20000(void) const noexcept { return m_lOrdinarySellNumberBelow20000; }
	INT64 GetOrdinarySellNumberBelow50000(void) const noexcept { return m_lOrdinarySellNumberBelow50000; }
	INT64 GetOrdinarySellNumberBelow100000(void) const noexcept { return m_lOrdinarySellNumberBelow100000; }
	INT64 GetOrdinarySellNumberBelow200000(void) const noexcept { return m_lOrdinarySellNumberBelow200000; }
	INT64 GetOrdinarySellNumberAbove200000(void) const noexcept { return m_lOrdinarySellNumberAbove200000; }
	INT64 GetOrdinaryBuyVolumeBelow5000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow5000; }
	INT64 GetOrdinaryBuyVolumeBelow10000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow10000; }
	INT64 GetOrdinaryBuyVolumeBelow20000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow20000; }
	INT64 GetOrdinaryBuyVolumeBelow50000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow50000; }
	INT64 GetOrdinaryBuyVolumeBelow100000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow100000; }
	INT64 GetOrdinaryBuyVolumeBelow200000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow200000; }
	INT64 GetOrdinaryBuyVolumeAbove200000(void) const noexcept { return m_lOrdinaryBuyVolumeAbove200000; }
	INT64 GetOrdinarySellVolumeBelow5000(void) const noexcept { return m_lOrdinarySellVolumeBelow5000; }
	INT64 GetOrdinarySellVolumeBelow10000(void) const noexcept { return m_lOrdinarySellVolumeBelow10000; }
	INT64 GetOrdinarySellVolumeBelow20000(void) const noexcept { return m_lOrdinarySellVolumeBelow20000; }
	INT64 GetOrdinarySellVolumeBelow50000(void) const noexcept { return m_lOrdinarySellVolumeBelow50000; }
	INT64 GetOrdinarySellVolumeBelow100000(void) const noexcept { return m_lOrdinarySellVolumeBelow100000; }
	INT64 GetOrdinarySellVolumeBelow200000(void) const noexcept { return m_lOrdinarySellVolumeBelow200000; }
	INT64 GetOrdinarySellVolumeAbove200000(void) const noexcept { return m_lOrdinarySellVolumeAbove200000; }
	void SetOrdinaryBuyNumberBelow5000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow5000 = lValue; }
	void SetOrdinaryBuyNumberBelow10000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow10000 = lValue; }
	void SetOrdinaryBuyNumberBelow20000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow20000 = lValue; }
	void SetOrdinaryBuyNumberBelow50000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow50000 = lValue; }
	void SetOrdinaryBuyNumberBelow100000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow100000 = lValue; }
	void SetOrdinaryBuyNumberBelow200000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow200000 = lValue; }
	void SetOrdinaryBuyNumberAbove200000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberAbove200000 = lValue; }
	void SetOrdinarySellNumberBelow5000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow5000 = lValue; }
	void SetOrdinarySellNumberBelow10000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow10000 = lValue; }
	void SetOrdinarySellNumberBelow20000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow20000 = lValue; }
	void SetOrdinarySellNumberBelow50000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow50000 = lValue; }
	void SetOrdinarySellNumberBelow100000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow100000 = lValue; }
	void SetOrdinarySellNumberBelow200000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow200000 = lValue; }
	void SetOrdinarySellNumberAbove200000(INT64 lValue) noexcept { m_lOrdinarySellNumberAbove200000 = lValue; }
	void SetOrdinaryBuyVolumeBelow5000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow5000 = lValue; }
	void SetOrdinaryBuyVolumeBelow10000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow10000 = lValue; }
	void SetOrdinaryBuyVolumeBelow20000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow20000 = lValue; }
	void SetOrdinaryBuyVolumeBelow50000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow50000 = lValue; }
	void SetOrdinaryBuyVolumeBelow100000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow100000 = lValue; }
	void SetOrdinaryBuyVolumeBelow200000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow200000 = lValue; }
	void SetOrdinaryBuyVolumeAbove200000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeAbove200000 = lValue; }
	void SetOrdinarySellVolumeBelow5000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow5000 = lValue; }
	void SetOrdinarySellVolumeBelow10000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow10000 = lValue; }
	void SetOrdinarySellVolumeBelow20000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow20000 = lValue; }
	void SetOrdinarySellVolumeBelow50000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow50000 = lValue; }
	void SetOrdinarySellVolumeBelow100000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow100000 = lValue; }
	void SetOrdinarySellVolumeBelow200000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow200000 = lValue; }
	void SetOrdinarySellVolumeAbove200000(INT64 lValue) noexcept { m_lOrdinarySellVolumeAbove200000 = lValue; }

	INT64 GetCanceledBuyVolumeBelow5000(void) const noexcept { return m_lCanceledBuyVolumeBelow5000; }
	INT64 GetCanceledBuyVolumeBelow10000(void) const noexcept { return m_lCanceledBuyVolumeBelow10000; }
	INT64 GetCanceledBuyVolumeBelow20000(void) const noexcept { return m_lCanceledBuyVolumeBelow20000; }
	INT64 GetCanceledBuyVolumeBelow50000(void) const noexcept { return m_lCanceledBuyVolumeBelow50000; }
	INT64 GetCanceledBuyVolumeBelow100000(void) const noexcept { return m_lCanceledBuyVolumeBelow100000; }
	INT64 GetCanceledBuyVolumeBelow200000(void) const noexcept { return m_lCanceledBuyVolumeBelow200000; }
	INT64 GetCanceledBuyVolumeAbove200000(void) const noexcept { return m_lCanceledBuyVolumeAbove200000; }
	INT64 GetCanceledSellVolumeBelow5000(void) const noexcept { return m_lCanceledSellVolumeBelow5000; }
	INT64 GetCanceledSellVolumeBelow10000(void) const noexcept { return m_lCanceledSellVolumeBelow10000; }
	INT64 GetCanceledSellVolumeBelow20000(void) const noexcept { return m_lCanceledSellVolumeBelow20000; }
	INT64 GetCanceledSellVolumeBelow50000(void) const noexcept { return m_lCanceledSellVolumeBelow50000; }
	INT64 GetCanceledSellVolumeBelow100000(void) const noexcept { return m_lCanceledSellVolumeBelow100000; }
	INT64 GetCanceledSellVolumeBelow200000(void) const noexcept { return m_lCanceledSellVolumeBelow200000; }
	INT64 GetCanceledSellVolumeAbove200000(void) const noexcept { return m_lCanceledSellVolumeAbove200000; }
	void SetCanceledBuyVolumeBelow5000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow5000 = lValue; }
	void SetCanceledBuyVolumeBelow10000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow10000 = lValue; }
	void SetCanceledBuyVolumeBelow20000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow20000 = lValue; }
	void SetCanceledBuyVolumeBelow50000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow50000 = lValue; }
	void SetCanceledBuyVolumeBelow100000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow100000 = lValue; }
	void SetCanceledBuyVolumeBelow200000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow200000 = lValue; }
	void SetCanceledBuyVolumeAbove200000(INT64 lValue) noexcept { m_lCanceledBuyVolumeAbove200000 = lValue; }
	void SetCanceledSellVolumeBelow5000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow5000 = lValue; }
	void SetCanceledSellVolumeBelow10000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow10000 = lValue; }
	void SetCanceledSellVolumeBelow20000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow20000 = lValue; }
	void SetCanceledSellVolumeBelow50000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow50000 = lValue; }
	void SetCanceledSellVolumeBelow100000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow100000 = lValue; }
	void SetCanceledSellVolumeBelow200000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow200000 = lValue; }
	void SetCanceledSellVolumeAbove200000(INT64 lValue) noexcept { m_lCanceledSellVolumeAbove200000 = lValue; }

public:
	// ��Ҫ�洢������
	INT64 m_lOrdinaryBuyVolume; // �������롣�ɽ��۽ӽ��������һ��������������λ����
	INT64 m_lAttackBuyVolume; // ���Ͻ��������룬�ɽ��۳�����һ�۸񵫲����������ۡ�����ɽ�����������m_lStrongBuyVolume��
	INT64 m_lStrongBuyVolume; // ����ǿ������,�ɽ��۳���֮ǰ���������ۡ�
	INT64 m_lOrdinarySellVolume;
	INT64 m_lAttackSellVolume; // ����������������һ�۵����������
	INT64 m_lStrongSellVolume; // ����ǿ������,�ɽ��۵���֮ǰ���������
	INT64 m_lUnknownVolume;
	INT64 m_lCanceledBuyVolume; // �򵥳�����
	INT64 m_lCanceledSellVolume; // ����������
	INT64 m_lTransactionNumber;
	INT64 m_lTransactionNumberBelow5000;
	INT64 m_lTransactionNumberBelow50000;
	INT64 m_lTransactionNumberBelow200000;
	INT64 m_lTransactionNumberAbove200000;

	INT64 m_lAttackBuyBelow50000;
	INT64 m_lAttackBuyBelow200000;
	INT64 m_lAttackBuyAbove200000;
	INT64 m_lAttackSellBelow50000;
	INT64 m_lAttackSellBelow200000;
	INT64 m_lAttackSellAbove200000;

	INT64 m_lOrdinaryBuyNumberBelow5000; // �������ܵ���5000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyNumberBelow10000; // �������ܵ���10000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyNumberBelow20000; // �������ܵ���20000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyNumberBelow50000; //
	INT64 m_lOrdinaryBuyNumberBelow100000; // �������ܵ���100000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyNumberBelow200000; //
	INT64 m_lOrdinaryBuyNumberAbove200000; //
	INT64 m_lOrdinarySellNumberBelow5000; // �������ܵ���5000�ɵĳɽ�����
	INT64 m_lOrdinarySellNumberBelow10000; // �������ܵ���10000�ɵĳɽ�����
	INT64 m_lOrdinarySellNumberBelow20000; // �������ܵ���20000�ɵĳɽ�����
	INT64 m_lOrdinarySellNumberBelow50000; //
	INT64 m_lOrdinarySellNumberBelow100000; // �������ܵ���100000�ɵĳɽ�����
	INT64 m_lOrdinarySellNumberBelow200000; //
	INT64 m_lOrdinarySellNumberAbove200000; //
	INT64 m_lOrdinaryBuyVolumeBelow5000; // �������ܵ���5000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyVolumeBelow10000; // �������ܵ���10000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyVolumeBelow20000; // �������ܵ���20000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyVolumeBelow50000; //
	INT64 m_lOrdinaryBuyVolumeBelow100000; // �������ܵ���100000�ɵĳɽ�����
	INT64 m_lOrdinaryBuyVolumeBelow200000; //
	INT64 m_lOrdinaryBuyVolumeAbove200000; //
	INT64 m_lOrdinarySellVolumeBelow5000; // �������ܵ���5000�ɵĳɽ�����
	INT64 m_lOrdinarySellVolumeBelow10000; // �������ܵ���10000�ɵĳɽ�����
	INT64 m_lOrdinarySellVolumeBelow20000; // �������ܵ���20000�ɵĳɽ�����
	INT64 m_lOrdinarySellVolumeBelow50000; //
	INT64 m_lOrdinarySellVolumeBelow100000; // �������ܵ���100000�ɵĳɽ�����
	INT64 m_lOrdinarySellVolumeBelow200000; //
	INT64 m_lOrdinarySellVolumeAbove200000; //

	INT64 m_lCanceledBuyVolumeBelow5000; // �������ܵ���5000�ɵĳ�������
	INT64 m_lCanceledBuyVolumeBelow10000; // �������ܵ���10000�ɵĳ�������
	INT64 m_lCanceledBuyVolumeBelow20000; // �������ܵ���20000�ɵĳ�������
	INT64 m_lCanceledBuyVolumeBelow50000; //
	INT64 m_lCanceledBuyVolumeBelow100000; // �������ܵ���100000�ɵĳ�������
	INT64 m_lCanceledBuyVolumeBelow200000; //
	INT64 m_lCanceledBuyVolumeAbove200000; //
	INT64 m_lCanceledSellVolumeBelow5000; // �������ܵ���5000�ɵĳ�������
	INT64 m_lCanceledSellVolumeBelow10000; // �������ܵ���10000�ɵĳ�������
	INT64 m_lCanceledSellVolumeBelow20000; // �������ܵ���20000�ɵĳ�������
	INT64 m_lCanceledSellVolumeBelow50000; //
	INT64 m_lCanceledSellVolumeBelow100000; // �������ܵ���100000�ɵĳ�������
	INT64 m_lCanceledSellVolumeBelow200000; //
	INT64 m_lCanceledSellVolumeAbove200000; //

	// ����Ҫ�洢������
};