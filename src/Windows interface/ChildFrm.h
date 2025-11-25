// ChildFrm.h: CChildFrame 类的接口
//

#pragma once

class CChildFrame : public CMDIChildWndEx {
	DECLARE_DYNCREATE(CChildFrame)

public:
	CChildFrame() {}
	~CChildFrame() override = default;

	// 特性
protected:
	CSplitterWndEx m_wndSplitter;

public:
	// 操作
public:
	// 重写
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	// 实现
public:
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
};
