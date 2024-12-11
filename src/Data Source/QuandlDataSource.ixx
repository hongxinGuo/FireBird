#include"pch.h"
export module FireBird.DataSource.Quandl;

import FireBird.DataSource.Virtual;
import FireBird.Factory.Quandl;

export {
	class CQuandlDataSource : public CVirtualDataSource {
	public:
		CQuandlDataSource();
		CQuandlDataSource(const CQuandlDataSource& other) = delete;
		CQuandlDataSource(CQuandlDataSource&& other) noexcept = delete;
		CQuandlDataSource& operator=(const CQuandlDataSource& other) = delete;
		CQuandlDataSource& operator=(CQuandlDataSource&& other) noexcept = delete;
		~CQuandlDataSource() override = default;

		bool Reset() override;

		bool GenerateInquiryMessage(long lCurrentTime) override;

		virtual bool Inquire();

		void ConfigureInternetOption() override;

	protected:
		CQuandlFactory m_QuandlFactory;

		bool m_fQuandlDataInquiryFinished{ false };
	};

	using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

	extern CQuandlDataSourcePtr gl_pQuandlDataSource;
}
