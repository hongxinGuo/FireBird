// ChildFrm.h : interface of the CWatchdogChildFrame class
//

#pragma once

class CWatchdogChildFrame : public CMDIChildWndEx {
	DECLARE_DYNCREATE(CWatchdogChildFrame)

public:
	CWatchdogChildFrame() noexcept;

	// Attributes
protected:
	CSplitterWndEx m_wndSplitter;

public:
	// Operations
public:
	// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CWatchdogChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
