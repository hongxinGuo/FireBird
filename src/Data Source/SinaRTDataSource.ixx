module;

export module FireBirdLib.DataSource.SinaRT;

import FireBirdLib.DataSource;
import FireBirdLib.WebData;

import std;
using std::chrono::local_seconds;
using std::shared_ptr;

export {
	class CSinaRTDataSource : public CVirtualDataSource {
	public:
		CSinaRTDataSource();
		CSinaRTDataSource(const CSinaRTDataSource& other) = delete;
		CSinaRTDataSource(CSinaRTDataSource&& other) noexcept = delete;
		CSinaRTDataSource& operator=(const CSinaRTDataSource& other) = delete;
		CSinaRTDataSource& operator=(CSinaRTDataSource&& other) noexcept = delete;
		~CSinaRTDataSource() override = default;

		bool Reset() override;

		bool GenerateInquiryMessage(const local_seconds& currentTime) override;

		void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
		void CreateCurrentInquireString() override;

		void CheckWebData(const CWebDataPtr& pWebData) override;
	};

	using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
	extern CSinaRTDataSourcePtr gl_pSinaRTDataSource; // 新浪实时数据引擎，唯一实例
}