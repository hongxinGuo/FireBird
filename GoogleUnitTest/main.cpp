////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

// 排除外部代码
#include<CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*"); // 排除GoogleTest
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*"); // 排除扩展程序包
ExcludeSourceFromCodeCoverage(Exclude3, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除数据库类
ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\*.*"); // 排除VS系统库
// 排除无需测试的单个文件
ExcludeSourceFromCodeCoverage(Exclude11, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.cpp");
ExcludeSourceFromCodeCoverage(Exclude12, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude13, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysisDoc.cpp");
ExcludeSourceFromCodeCoverage(Exclude14, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude15, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.cpp");
ExcludeSourceFromCodeCoverage(Exclude16, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.h");
ExcludeSourceFromCodeCoverage(Exclude17, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.cpp");
ExcludeSourceFromCodeCoverage(Exclude18, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.h");
//  排除单个类

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