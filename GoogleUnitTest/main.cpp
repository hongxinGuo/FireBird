////////////////////////////////////////////////////////////////////////////////////
//
//
// ������ҪԤ���趨ȫ�ֳ�ʼ���������ʶ���ʹ��ϵͳ�ṩ��main()�����������Լ����塣
// ��Ҫ����һ��AddGlobalTestEnvironment()�������ã������趨ȫ�ֻ������˺������Զ��ʹ�á�
//
/////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"MarketTest.h"

using namespace StockAnalysisTest;

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);

  return RUN_ALL_TESTS();
}