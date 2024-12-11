#include"pch.h"
export module FireBird.DataSource.TengxunRT;

import FireBird.DataSource.VirtualBase;

export {
	class CTengxunRTDataSource : public CVirtualDataSource {
	public:
		CTengxunRTDataSource();
		~CTengxunRTDataSource() override = default;

		bool Reset() override;

		bool GenerateInquiryMessage(long lCurrentTime) override;

		void ConfigureInternetOption() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
		void CreateCurrentInquireString() override;

		static bool IsInvalidTengxunRTData(const CWebData& WebDataReceived);
	};

	using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;

	extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
}