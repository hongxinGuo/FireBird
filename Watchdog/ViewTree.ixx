module;
#include"pch.h"
export module Watchdog.ViewTree;
import std;
export {
	class CViewTree : public CTreeCtrl {
		// Construction
	public:
		CViewTree() noexcept;

		// Overrides
	protected:
		BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;

		// Implementation
	public:
		~CViewTree() override;

	protected:
		DECLARE_MESSAGE_MAP()
	};
}
