#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ContainerFinnhubEconomicCalendar.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerFinnhubEconomicCalendarTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CContainerFinnhubEconomicCalendar m_containerEconomicCalendar;
	};

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestIsNeedUpdate) {
		EXPECT_FALSE(m_containerEconomicCalendar.IsNeedUpdate()) << "皆为空，无需更新";

		vector<CEconomicCalendarPtr> vEconomicCalendar;
		const auto p1 = make_shared<CEconomicCalendar>();
		const auto p2 = make_shared<CEconomicCalendar>();
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsNeedUpdate());
	}

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestUpdate1) {
		vector<CEconomicCalendarPtr> vEconomicCalendar;
		const auto p1 = make_shared<CEconomicCalendar>();
		p1->m_strCountry = _T("US");
		const auto p2 = make_shared<CEconomicCalendar>();
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsNeedUpdate());
	}

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestUpdate2) {
		EXPECT_FALSE(m_containerEconomicCalendar.IsNeedUpdate()) << "皆为空，无需更新";

		vector<CEconomicCalendarPtr> vEconomicCalendar;
		const auto p1 = make_shared<CEconomicCalendar>();
		const auto p2 = make_shared<CEconomicCalendar>();
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsNeedUpdate());
	}
}
