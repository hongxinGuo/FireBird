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
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*");
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*");
ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\*.*");
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