#pragma once

#include<memory>
#include<vector>

#include "ProductTiingo.h"

class CTiingoIEXTopOFBook final {
public:
	CTiingoIEXTopOFBook();
	// �������ƺ͸�ֵ��
	CTiingoIEXTopOFBook(const CTiingoIEXTopOFBook&) = delete;
	CTiingoIEXTopOFBook& operator=(const CTiingoIEXTopOFBook&) = delete;
	CTiingoIEXTopOFBook(const CTiingoIEXTopOFBook&&) noexcept = delete;
	CTiingoIEXTopOFBook& operator=(const CTiingoIEXTopOFBook&&) noexcept = delete;
	~CTiingoIEXTopOFBook() = default;

public:
	CString m_strTicker{ _T("") };
	INT64 m_llTimestamp{ 0 };

	// 
	long m_lLastClose{ 0 }; // ǰ���̡���λ��0.001Ԫ
	long m_lOpen{ 0 }; // ���̼�
	long m_lHigh{ 0 }; // ��߼�
	long m_lLow{ 0 }; // ��ͼ�
	long m_lNew{ 0 }; // ���¼�

	INT64 m_llVolume{ 0 };
};

using CTiingoIEXTopOFBookPtr = shared_ptr<CTiingoIEXTopOFBook>;
using CTiingoIEXTopOFBooksPtr = shared_ptr<vector<CTiingoIEXTopOFBookPtr>>;
