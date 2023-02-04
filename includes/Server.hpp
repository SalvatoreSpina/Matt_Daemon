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

Server::Server() : _client_sockets(NB_MAX_CLI, 0)
{
	this->_address.sin_family = AF_INET;  
	this->_address.sin_addr.s_addr = INADDR_ANY;  
	this->_address.sin_port = htons(PORT);
	this->_address_len = sizeof(_address);
	FD_ZERO(&this->_file_descriptors);
}

Server::~Server()
{
	for (unsigned int i = 0; i < this->_client_sockets.size(); i++) {
		close(this->_client_sockets[i]);
	}
}

int Server::Init()
{
	int result = kOk;
	_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (_socket  == 0) {
		result = kFailed;
	} else {
		const int enable = 1;
		if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
			perror("Error: setsockopt");
			result = kFailed;
		} else if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
			perror("Error: bind");
			result = kFailed;
		} else if (listen(_socket, NB_MAX_CLI) < 0) {
			perror("Error: listen");
			result = kFailed;	
		}
	}
	return result;
}

int Server::NewClient()
{
	int new_socket, result;
	result = kOk;
	new_socket = accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&_address_len);
	if (new_socket < 0) {
		result = kFailed;
	} else if (GetClientCount() == NB_MAX_CLI) {
		close(new_socket);
		result = kRefused;
	} else {
		for (unsigned int i = 0; i < _client_sockets.size(); ++i) {
			if (_client_sockets[i] == 0) {
				_client_sockets[i] = new_socket;
				break;
			}
		}
	}
	return result;
}

// This function retrieves the input received from a particular client and stores it in the string "input".
// client_index - The index of the client in the _client_sockets vector
// input - The string where the input received from the client will be stored
// Returns the status of the client - kReceived, kDisconnected or -1 if no input was received.
int Server::GetClientInput(int client_index, std::string &input)
{
	// bytes_read - The number of bytes read in each iteration
	// total_bytes - The total number of bytes read from the client
	size_t bytes_read, total_bytes;
	// buffer - Temporary storage for the input received from the client
	char buffer[BUFFER_SIZE + 1];
	// status - The status of the client (kReceived, kDisconnected or -1 if no input was received)
	int status = -1;

	// Clear the input string
	input.clear();

	// Check if input is available from the client with the given index
	if (FD_ISSET(_client_sockets[client_index], &_file_descriptors)) {
		// Set the status to kReceived
		status = kReceived;

		// Read the input from the client into the buffer
		bytes_read = read(_client_sockets[client_index], buffer, BUFFER_SIZE);
		total_bytes = bytes_read;

		// Keep reading from the client until the number of bytes read is less than BUFFER_SIZE
		while (bytes_read == BUFFER_SIZE) {
			// Null-terminate the buffer
			buffer[bytes_read] = '\0';  
			// Add the buffer to the input string
			input += buffer;
			// Read more input from the client
			bytes_read = read(_client_sockets[client_index], buffer, BUFFER_SIZE);
			total_bytes += bytes_read;
		}

		// Check if the client has disconnected
		if (total_bytes == 0) {
			// Close the socket for the disconnected client
			close(_client_sockets[client_index]);  
			// Set the socket for the disconnected client to 0
			_client_sockets[client_index] = 0;
			// Set the status to kDisconnected
			status = kDisconnected;
		} else {
			// Null-terminate the buffer
			buffer[bytes_read] = '\0';  
			// Add the buffer to the input string
			input += buffer;

			// Find the first occurrence of "\r" or "\n" in the input string and extract the part before it
			size_t pos = input.find("\r");
			if (pos != std::string::npos) {
				input = input.substr(0, pos);
			} else {
				pos = input.find("\n");
				if (pos != std::string::npos) {
					input = input.substr(0, pos);
				}
			}
		}
	}

	// Return the status of the client
	return status;
}

int Server::GetClientSocket(int client_index) const
{
	return _client_sockets[client_index];
}

void Server::ResetFds()
{
	FD_ZERO(&_file_descriptors);
	FD_SET(_socket, &_file_descriptors);  
	for (const auto &socket : _client_sockets) {
		if (socket > 0) {
			FD_SET(socket, &_file_descriptors);
		}
	}
}

fd_set &Server::GetFds()
{
	return _file_descriptors;
}

int Server::GetMaxFd() const
{
	int maxfd = _socket;
	for (const auto &socket : _client_sockets) {
		maxfd = std::max(maxfd, socket);
	}
	return maxfd;
}

int Server::GetClientCount() const
{
	int count = 0;
	for (unsigned int i = 0; i < this->_client_sockets.size(); i++) {
		if (this->_client_sockets[i] != 0) {
			count++;
		}
	}
	return count;
}

int Server::IsNewClient() const
{
	return FD_ISSET(this->_socket, &this->_file_descriptors);
}
