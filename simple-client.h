#pragma once

struct SimpleClient {
	
	SimpleClient(const char* ip, const char* port);
	~SimpleClient();	

	bool connect();
	void send(const char* msg, int length);
	
private:
	const char* ip_addr {nullptr};
	const char* port 	{nullptr};
};
