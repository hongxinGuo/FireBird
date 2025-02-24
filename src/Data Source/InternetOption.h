#pragma once

struct InternetOption {
public:
	int option_connect_retries{ 1 };
	int option_connect_timeout{ 5 };
	int option_receive_timeout{ 5000 };
	int option_data_receive_timeout{ 12000 };
	int option_send_timeout{ 2000 };
};
