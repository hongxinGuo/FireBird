
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include"afxinet.h"

#include "OutputWnd.h"

#include"SetDayLine.h"
#include"SetRealTimeData.h"
#include"SetChoicedStock.h"
#include"SetStockCode.h"
#include"SetOption.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 特性
public:

// 操作
public:

private:
  bool CreateTodayActiveStockRTInquiringStr(CString& str);
  bool GetSinaStockRTData(void);

  bool CreateTotalStockContainer(void);
  bool CreateTodayActiveStockDayLineInquiringStr(CString& str, CString& strStartDay);
  bool GetNetEaseStockDayLineData(void);

  bool SchedulingTask(void);

  bool CompileTodayStocks(void);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  UINT                    m_uIdTimer;

  // 数据库
  CString									m_strDatabaseDir;

  bool                    m_fCheckTodayActiveStock; // 是否查询今日活跃股票代码

  bool										m_fGetRTStockData;
  bool										m_fGetDayLineData;
  bool                    m_fCountDownRT;
  int                     m_iCountDownDayLine;        // 日线数据读取延时计数。


protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd        m_wndOutput;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnSaveRtdata();
  afx_msg void OnDownloadDayline();
  afx_msg void OnSaveDaylineData();
  afx_msg void OnUpdateSaveDaylineData(CCmdUI *pCmdUI);
  afx_msg void OnCompileTodayStock();
  afx_msg void OnUpdateCompileTodayStock(CCmdUI *pCmdUI);
  afx_msg void OnCalculateRelativeStrong();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnRebuildDaylineDatabase();
  afx_msg void OnUpdateCalculateRelativeStrong(CCmdUI *pCmdUI);
};


