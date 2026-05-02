#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerFinnhubEconomicCalendar.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerFinnhubEconomicCalendarTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerFinnhubEconomicCalendar m_containerEconomicCalendar;
	};

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestIsUpdateDB) {
		EXPECT_FALSE(m_containerEconomicCalendar.IsUpdateDB()) << "皆为空，无需更新";

		vector<CEconomicCalendar> vEconomicCalendar;
		const CEconomicCalendar p1;
		const CEconomicCalendar p2;
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsUpdateDB());
	}

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestUpdate1) {
		vector<CEconomicCalendar> vEconomicCalendar;
		CEconomicCalendar p1;
		CEconomicCalendar p2;
		p1.m_strCountry = "US";
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsUpdateDB());
	}

	TEST_F(CContainerFinnhubEconomicCalendarTest, TestUpdate2) {
		EXPECT_FALSE(m_containerEconomicCalendar.IsUpdateDB()) << "皆为空，无需更新";

		vector<CEconomicCalendar> vEconomicCalendar;
		const CEconomicCalendar p1;
		const CEconomicCalendar p2;
		vEconomicCalendar.push_back(p1);
		vEconomicCalendar.push_back(p2);

		m_containerEconomicCalendar.Update(vEconomicCalendar);

		EXPECT_TRUE(m_containerEconomicCalendar.IsUpdateDB());
	}
}
