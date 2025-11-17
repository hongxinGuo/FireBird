// FireBirdDoc.h: CFireBirdDoc 类的接口
//

#pragma once

class CFireBirdDoc : public CDocument {
protected: // 仅从序列化创建
	CFireBirdDoc();
	DECLARE_DYNCREATE(CFireBirdDoc)

	// 特性
public:
	bool IsEmpty() const noexcept { return m_pCurrentStock == nullptr; }
	bool IsDayLineLoaded() const { return m_pCurrentStock->IsDayLineLoaded(); }
	bool IsChinaStock() const { return strcmp(typeid(*m_pCurrentStock).name(), _T("class CChinaStock")) == 0; }
	bool IsTiingoStock() const { return strcmp(typeid(*m_pCurrentStock).name(), _T("class CTiingoStock")) == 0; }

	// 操作
public:
	CVirtualStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CVirtualStockPtr& pStock) {
		m_pCurrentStock = pStock;
		if (m_pCurrentStock != nullptr) {
			ASSERT(pStock->IsDayLineLoaded());
			CalculateDayLineMA();
		}
	}

	void CalculateDayLineMA();
	void CalculateWeekLineMA();

	vector<long>* GetDayLine5MA() { return &m_DayLine5MA; }

	// 重写
public:
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDayLineERS
  virtual void InitializeSearchContent();
  virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDayLineERS

	// 实现
	~CFireBirdDoc() override = default;

protected:
	CVirtualStockPtr m_pCurrentStock{ nullptr }; // 当前股票

	vector<long> m_DayLine5MA{};
	vector<long> m_DayLine10MA{};
	vector<long> m_DayLine30MA{};
	vector<long> m_DayLine50MA{};
	vector<long> m_DayLine120MA{};
	vector<long> m_DayLine250MA{};

	vector<long> m_WeekLine5MA{};
	vector<long> m_WeekLine10MA{};
	vector<long> m_WeekLine30MA{};
	vector<long> m_WeekLine60MA{};
	vector<long> m_WeekLine120MA{};
	vector<long> m_WeekLine240MA{};

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDayLineERS
  // 用于为搜索处理程序设置搜索内容的 Helper 函数
  void SetSearchContent(const CString& value);
#endif // SHARED_HANDayLineERS
};
