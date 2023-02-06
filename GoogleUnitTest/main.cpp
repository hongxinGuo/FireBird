////////////////////////////////////////////////////////////////////////////////////
//
//
// ������ҪԤ���趨ȫ�ֳ�ʼ���������ʶ���ʹ��ϵͳ�ṩ��main()�����������Լ����塣
// ��Ҫ����һ��AddGlobalTestEnvironment()�������ã������趨ȫ�ֻ������˺������Զ��ʹ�á�
//
/////////////////////////////////////////////////////////////////////////////////////

// GoogleMockʹ�÷�����
//
//   ON_CALL(mock_object, method_name(matchers...))
//     .With(multi_argument_matcher)  // Can be used at most once
//     .WillByDefault(action);        // Required
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
// EXPECT_CALL(mock_object, method_name(matchers...))
//   .With(multi_argument_matcher)  // Can be used at most once
//   .Times(cardinality)            // Can be used at most once
//   .InSequence(sequences...)      // Can be used any number of times
//   .After(expectations...)        // Can be used any number of times
//   .WillOnce(action)              // Can be used any number of times
//   .WillRepeatedly(action)        // Can be used at most once
//   .RetiresOnSaturation();        // Can be used at most once
//
// where all clauses are optional and WillOnce() can be repeated.
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ����ʹ��Virtual leak detector(vld)�����ڴ�й©��
/// ����vld�ٷ�Դ��֧��ֻ��vs2015���ʶ���Ҫ�ֹ��޸�Դ���룬����ʹ��VS2022���롣
/// ��dbghelp.dll��Ҫʹ��Ŀ¼C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\Extensions\\TestPlatform\Extensions\Cpp\X64�е��Ǹ��汾
/// ��Ҫ��vld\VS2022�ļ����е��ļ�dbghelp.dll Microsoft.DTfW.DHL.manifest vld_x64.dll�����ļ�������ִ���ļ���(X64\Release)��
///
/// ĿǰVS������ڴ�й¶���Ҳ�������ˣ�Ҳ������ʹ��vld�ˡ�vld��VS2022��֧�����ޡ�
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include"vld.h" // ��Ҫ����ڴ�й©ʱ������ͷ�ļ��� ���ļ���Ҫλ��pch.h֮ǰ��

#include"pch.h"

#ifndef _MBCS
#error _T("��ϵͳʹ�ö��ֽ��ַ���")
#endif

#ifndef __GOOGLEMOCK__
#error _T("��ϵͳ����ʹ��GOOGLE MOCK")
#endif

// Ŀǰʹ��.runsettings�ļ����ų��ⲿ���룬����ʹ��ExcludeSourceFromCodeCoverage��ģʽ����ExcludeSourceFromCodeCoverageģʽĿǰ��C20��׼���޷����롣

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include"EnvironmentSetUp.h"
using namespace FireBirdTest;

int main(int argc, char* argv[]) {
	InitGoogleTest(&argc, argv);
	// gtest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(!gl_systemStatus.IsWorkingMode());

	return RUN_ALL_TESTS();
}
