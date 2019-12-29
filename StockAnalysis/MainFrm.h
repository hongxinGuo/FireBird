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
  CMainFrame();

  void Reset(void);

  // 特性
public:

  // 操作
public:

private:

  // 重置系统（恢复系统的初始态，准备第二天继续工作。第二天上午9时由SchedulingTask调用此函数，不允许其他函数调用。
  bool ResetSystem(void);

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
  long                    m_lCurrentPos;

  char m_aStockCodeTemp[30];

protected:  // 控件条嵌入成员
  CMFCMenuBar       m_wndMenuBar;
  CMFCToolBar       m_wndToolBar;
  CMFCStatusBar     m_wndStatusBar;
  CMFCToolBarImages m_UserImages;
  COutputWnd        m_wndOutput;
  COutputWnd        m_wndOutput2;

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
  afx_msg void OnCompileTodayStock();
  afx_msg void OnUpdateCompileTodayStock(CCmdUI* pCmdUI);
  afx_msg void OnCalculateRelativeStrong();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnUpdateCalculateRelativeStrong(CCmdUI* pCmdUI);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnRebuildDaylineRS();
  afx_msg void OnBuildResetSystem();
  afx_msg void OnUpdateRebuildDaylineRs(CCmdUI* pCmdUI);
};
