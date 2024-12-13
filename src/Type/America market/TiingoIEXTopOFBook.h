#pragma once

#include<memory>
#include<vector>

class CTiingoIEXTopOfBook final {
public:
	CTiingoIEXTopOfBook();
	// �������ƺ͸�ֵ��
	CTiingoIEXTopOfBook(const CTiingoIEXTopOfBook&) = delete;
	CTiingoIEXTopOfBook& operator=(const CTiingoIEXTopOfBook&) = delete;
	CTiingoIEXTopOfBook(const CTiingoIEXTopOfBook&&) noexcept = delete;
	CTiingoIEXTopOfBook& operator=(const CTiingoIEXTopOfBook&&) noexcept = delete;
	~CTiingoIEXTopOfBook() = default;

public:
	CString m_strTicker{ _T("") };
	chrono::sys_seconds m_timeStamp;
	chrono::sys_seconds m_lastSale;
	chrono::sys_seconds m_quote;
	long m_lLastClose{ 0 }; // ǰ���̡���λ��0.001Ԫ
	long m_lOpen{ 0 }; // ���̼�
	long m_lHigh{ 0 }; // ��߼�
	long m_lLow{ 0 }; // ��ͼ�
	long m_lNew{ 0 }; // ���¼�
	INT64 m_llVolume{ 0 };
};

using CTiingoIEXTopOfBookPtr = shared_ptr<CTiingoIEXTopOfBook>;
using CTiingoIEXTopOfBooksPtr = shared_ptr<vector<CTiingoIEXTopOfBookPtr>>;
