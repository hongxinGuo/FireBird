#include"pch.h"

////#include"gtest/gtest.h"

#include"afxinet.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"
import FireBird.WebProduct.Dummy;

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // 为了方便查找出错位置,将mock变量声明为全局的。
	static CVirtualProductWebDataPtr m_pProduct;

	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pVirtualDataSource = nullptr;
		}

		static void TearDownTestSuite() {
			m_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			m_pVirtualDataSource = make_shared<CMockVirtualDataSource>();
			m_pProduct = make_shared<CProductDummy>();
			m_pVirtualDataSource->SetCurrentInquiry(m_pProduct);
		}

		void TearDown() override {
			// clearUp

			m_pVirtualDataSource->SetInquiringString(_T(""));
			m_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};
}
