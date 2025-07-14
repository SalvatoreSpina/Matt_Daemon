#pragma once
#include "utilities.hpp"

enum ConnectStatus { kOk, kFailed, kRefused };
enum InputStatus { kReceived, kDisconnected };

class Server {
	
	private:
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;
		int _socket;
		struct sockaddr_in _address;
		fd_set _file_descriptors;
		int _address_len;
		std::vector<int> _client_sockets;
	
	public:
		Server();
		~Server();

		int Init();
		int NewClient();
		int GetClientInput(int i, std::string& input);
		int GetClientSocket(int i) const;
		void ResetFds();
		fd_set& GetFds();
		int GetMaxFd() const;
		int GetClientCount() const;
		int IsNewClient() const;
};