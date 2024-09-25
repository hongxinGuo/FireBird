#pragma once

#include"SetTiingoCrypto.h"

#include<memory>
#include<vector>

class CTiingoCryptoSymbol final {
public:
	CTiingoCryptoSymbol();
	~CTiingoCryptoSymbol() = default;

	void Reset();

	void Load(const CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);

	void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }
	bool IsDayLineNeedUpdate() const noexcept { return m_fUpdateDayLine; }

	CString m_strTicker;
	CString m_strName;
	CString m_strDescription; // �����ѷ��������ˡ�
	CString m_strBaseCurrency;
	CString m_strQuoteCurrency;

	// ����洢������
	bool m_fUpdateDayLine{ true };
};

using CTiingoCryptoSymbolPtr = shared_ptr<CTiingoCryptoSymbol>;
using CTiingoCryptosPtr = shared_ptr<vector<CTiingoCryptoSymbolPtr>>;
