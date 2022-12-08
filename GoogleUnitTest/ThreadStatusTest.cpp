#include"pch.h"

#include"GeneralCheck.h"

#include"ThreadStatus.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class ThreadStatusTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearUp
		}
	};

	TEST_F(ThreadStatusTest, TestInitialize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfBackGroundWorkingThread(), 0);
		int iCreatingThread = gl_ThreadStatus.GetNumberOfSavingThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);

		size_t l = gl_systemMessage.InformationSize();
		CThreadStatus threadStatus; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
		EXPECT_EQ(gl_systemMessage.InformationSize(), l + 1); // ϵͳ��������
		for (int i = 0; i < l + 1; i++) {
			CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
		}
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
	}

	TEST_F(ThreadStatusTest, TestIsCalculatingRS) {
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		for (int i = 0; i < gl_systemConfigeration.GetBackgroundThreadPermittedNumber(); i++) {  // Ŀǰ�������8���߳�
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
		}
		for (int i = 0; i < gl_systemConfigeration.GetBackgroundThreadPermittedNumber() - 1; i++) {
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			EXPECT_TRUE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		}
		gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
	}

	TEST_F(ThreadStatusTest, TestIsSavThreadRunning) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfSavingThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
		gl_ThreadStatus.IncreaseSavingThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread + 1);
		EXPECT_TRUE(gl_ThreadStatus.IsSavingThreadRunning());
		gl_ThreadStatus.DecreaseSavingThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
		gl_ThreadStatus.DecreaseSavingThread();
		if (iCreatingThread <= 0) {
			EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
			EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
		}
		else {
			EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread - 1);
		}
	}

	TEST_F(ThreadStatusTest, TestIsWebInquiringThreadRunning) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
		gl_ThreadStatus.IncreaseWebInquiringThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread + 1);
		EXPECT_TRUE(gl_ThreadStatus.IsWebInquiringThreadRunning());
		gl_ThreadStatus.DecreaseWebInquiringThread();
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
		gl_ThreadStatus.DecreaseWebInquiringThread();
		if (iCreatingThread <= 0) {
			EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
			EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
		}
		else {
			EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread - 1);
		}
	}
}