// WatchdogDoc.h : interface of the CWatchdogDoc class
//

module;
#include"pch.h"
#include <afxwin.h>
export module Watchdog.Document;

export {
	class CWatchdogDoc : public CDocument {
	protected: // create from serialization only
		CWatchdogDoc() noexcept;
		DECLARE_DYNCREATE(CWatchdogDoc)

		// Attributes
	public:
		// Operations
	public:
		// Overrides
	public:
		BOOL OnNewDocument() override;
		void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
		void InitializeSearchContent()override;
		void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)override;
#endif // SHARED_HANDLERS

		// Implementation
	public:
		~CWatchdogDoc() override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif

		// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
		// Helper function that sets search content for a Search Handler
		void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	};
}
