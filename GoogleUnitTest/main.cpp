////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////

// GoogleMock使用方法：
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
/// 可以使用Virtual leak detector(vld)查找内存泄漏。
/// 由于vld官方源码支持只到vs2015，故而需要手工修改源代码，才能使用VS2022编译。
/// 且dbghelp.dll需要使用目录C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\Extensions\\TestPlatform\Extensions\Cpp\X64中的那个版本
/// 需要将vld\VS2022文件夹中的文件dbghelp.dll Microsoft.DTfW.DHL.manifest vld_x64.dll三个文件拷贝至执行文件夹(X64\Release)中
///
/// 目前VS本身的内存泄露检测也跟上来了，也许无需使用vld了。vld对VS2022的支持有限。
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include"vld.h" // 需要检测内存泄漏时包括此头文件。 此文件需要位于pch.h之前。

#include"pch.h"

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifndef __GOOGLEMOCK__
#error _T("本系统必须使用GOOGLE MOCK")
#endif

// 目前使用.runsettings文件来排除外部代码，不再使用ExcludeSourceFromCodeCoverage的模式。且ExcludeSourceFromCodeCoverage模式目前在C20标准下无法编译。

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
