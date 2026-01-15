#include"pch.h"

#include"afxinet.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;

namespace {
	CMockVirtualDataSourcePtr s_pVirtualDataSource; // 为了方便查找出错位置,将mock变量声明为全局的。
	CVirtualProductWebDataPtr s_pProduct;
}

namespace FireBirdTest {
	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			s_pVirtualDataSource = nullptr;
		}

		static void TearDownTestSuite() {
			s_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pVirtualDataSource = make_shared<CMockVirtualDataSource>();
			s_pProduct = make_shared<CProductDummy>();
			s_pVirtualDataSource->SetCurrentInquiry(s_pProduct);
		}

		void TearDown() override {
			// clearUp

			s_pVirtualDataSource->SetInquiringString("");
			s_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};
}
