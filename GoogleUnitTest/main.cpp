////////////////////////////////////////////////////////////////////////////////////
//
//
// ������ҪԤ���趨ȫ�ֳ�ʼ���������ʶ���ʹ��ϵͳ�ṩ��main()�����������Լ����塣
// ��Ҫ����һ��AddGlobalTestEnvironment()�������ã������趨ȫ�ֻ������˺������Զ��ʹ�á�
//
/////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include<CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)
//ExcludeSourceFromCodeCoverage(Exclude1, L"\\SSD500\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.cpp");
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.cpp");
ExcludeSourceFromCodeCoverage(Exclude3, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.h");
ExcludeSourceFromCodeCoverage(Exclude4, L"std::bool::*");
//ExcludeSourceFromCodeCoverage(Exclude5, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\packages\\Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3\\build\\native\\include\\gtest\\*.h");
//ExcludeSourceFromCodeCoverage(Exclude6, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\packages\\Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3\\build\\native\\include\\gtest\\internal\\*.h");
//ExcludeSourceFromCodeCoverage(Exclude7, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\packages\\Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3\\build\\native\\include\\gtest\\internal\\custom\\*.h");
#pragma managed(pop)

#include"EnvironmentSetUp.h"

using namespace StockAnalysisTest;

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);

  EXPECT_FALSE(gl_fNormalMode);
  EXPECT_TRUE(gl_fTestMode);

  return RUN_ALL_TESTS();
}