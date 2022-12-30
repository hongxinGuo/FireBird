#pragma once

#include"SetFinnhubStockExchange.h"

#include<memory>

class CFinnhubStockExchange final {
public:
	CFinnhubStockExchange();
	virtual ~CFinnhubStockExchange() = default;

	void Append(CSetFinnhubStockExchange& setFinnhubExchange);

	void Load(CSetFinnhubStockExchange& setFinnhubExchange);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
	CString m_strCode; // ��λ�Ľ��������롣�Ϻ��г�ΪSS�� �����г�ΪUS��������
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	bool m_fUpdated;
};

using CFinnhubStockExchangePtr = shared_ptr<CFinnhubStockExchange>;
