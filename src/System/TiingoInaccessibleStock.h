//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// tiingo��վ���ݷ�Ϊ��Ѻ��շ����֣�����˻�ֻ����ȡ�������ݡ��ʶ���Ҫ��������ȷ���Ƿ��ܹ����յ���Ч���ݡ�
// �˽ṹ�д洢�����ܲ�������յ�֤ȯ���ƣ����������ų��ýṹ�е�֤ȯ�������룩��
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

#include "Inaccessible.h"

using CInaccessibleStocksPtr = shared_ptr<CInaccessible>;

class CTiingoInaccessibleStock {
public:
	CTiingoInaccessibleStock();
	// ֻ����һ��ʵ��,������ֵ������
	CTiingoInaccessibleStock(const CTiingoInaccessibleStock&) = delete;
	CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&) = delete;
	CTiingoInaccessibleStock(const CTiingoInaccessibleStock&&) noexcept = delete;
	CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&&) noexcept = delete;
	virtual ~CTiingoInaccessibleStock();
	void UpdateDB();

	bool LoadDB();
	bool LoadDB(const string& strFileDirectory);
	void SaveDB() const;
	void Update();
	void UpdateJson();

	void Clear() noexcept {
		m_mapStock.clear();
		m_finnhubInaccessibleStock.clear();
	}

	void SetDefaultFileName(const string& fileName) noexcept { m_strFileName = fileName; }
	string GetDefaultFileName() { return m_strFileName; }

	void SetUpdateDate(const long lDate) noexcept { m_lUpdateDate = lDate; }
	long GetUpdateDate() const { return m_lUpdateDate; }

	static int GetTiingoInquiryIndex(const string& sString) { return gl_FinnhubInquiryType.GetInquiryType(sString); }
	CInaccessibleStocksPtr GetStock(int iInquireType) { return m_mapStock.at(iInquireType); }
	void SetStock(const int iInquireType, const CInaccessibleStocksPtr& pStock) { m_mapStock[iInquireType] = pStock; }
	void DeleteStock(int iInquireType, const string& strStock);
	bool HaveStock(int iInquireType, const string& strStockCode) const;
	size_t GetItemSize() const noexcept { return m_mapStock.size(); }

	bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
	void SetUpdateDB(const bool fUpdate) noexcept { m_fUpdateDB = fUpdate; }

protected:
	string m_strFileName{ _T("TiingoInaccessibleStock.json") }; // �����ļ�����

	long m_lUpdateDate{ 19800101 }; // ���ļ���������
	map<int, CInaccessibleStocksPtr> m_mapStock; //

	bool m_fInitialized{ false };
	bool m_fUpdateDB{ false };

	json m_finnhubInaccessibleStock;
};

using CTiingoInaccessibleStockPtr = shared_ptr<CTiingoInaccessibleStock>;

extern CTiingoInaccessibleStock gl_tiingoInaccessibleStock;

extern std::string Test_gl_sTiingoInaccessibleStock; // finnhub inaccessible exchange test data
