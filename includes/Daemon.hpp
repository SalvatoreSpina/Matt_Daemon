#pragma once
#include "utilities.hpp"
#include "Tintin_reporter.hpp"
#include "Server.hpp"

class Daemon
{
	private:
		// Delete default constructor, copy constructor, and assignment operator
		Daemon() = delete;
		Daemon(const Daemon&) = delete;
		Daemon& operator=(const Daemon&) = delete;

		int _lock_file_descriptor;
		Server _server;

		void LogClientsInput();

	public:
		// Use a constructor with a meaningful parameter name
		explicit Daemon(const int &lock_file_descriptor);
		~Daemon();

		void Start();
		void InitializeServer();
		void LockFile();
		void UnlockFile();

		class FunctionCallError : public std::exception {
		public:
			const char* what() const throw() {
				return "Function call error.";
			}
		};

		class FileLockError : public std::exception {
		public:
			const char* what() const throw() {
				return "File lock error.";
			}
		};

		class FilePermissionError : public std::exception {
		public:
			const char* what() const throw() {
				return "File permission error.";
			}
		};

		class QuitRequested : public std::exception {
		public:
			const char* what() const throw() {
				return "Quit requested.";
			}
		};
};


Daemon::Daemon(const int &lock_file_descriptor) : _lock_file_descriptor(lock_file_descriptor) {
	Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Daemon started.");
}

Daemon::~Daemon() {
	try {
		Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Daemon quitting.");
	} catch (const std::exception& e) {
	// Log the exception if it occurs, but ignore it
	}
	if (_lock_file_descriptor != -1) {
		UnlockFile();
		if (close(_lock_file_descriptor) != 0) {
			Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Failed to close lock file descriptor.");
		}
	}
	if (remove(LOCK_FILE) != 0) {
		Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Failed to remove lock file.");
	}
}

void Daemon::Start()
{
	int ret;
	const std::string pid = "Started. PID: " + std::to_string(getpid());
	Tintin_reporter::WriteLogs(Tintin_reporter::kLog, "Entering daemon mode.");
	Tintin_reporter::WriteLogs(Tintin_reporter::kLog, pid);

	while (true) {
		this->_server.ResetFds();
		const int max_fd = this->_server.GetMaxFd();
		fd_set &fds = this->_server.GetFds();
		if (select(max_fd + 1, &fds, nullptr, nullptr, nullptr) >= 0) {
			if (this->_server.IsNewClient()) {
				ret = this->_server.NewClient();
				if (ret == 0) {
					Tintin_reporter::WriteLogs(Tintin_reporter::kLog, "New connection.");
				} else if (ret == 1) {
					Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Connection failed.");
				}
				else {
					Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Max clients.");
				}
			}
			this->LogClientsInput();
		}
	}
}


void Daemon::InitializeServer()
{
	int init_result = 0;
	Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Creating server.");
	init_result = _server.Init();
	if (init_result == kFailed) {
		Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Server failed to create.");
		throw FunctionCallError();
	}
	Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Server created.");
}

void Daemon::LockFile()
{
	int lock_result = flock(_lock_file_descriptor, LOCK_EX | LOCK_NB);
	if (lock_result == -1) {
		Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Error file locked.");
		throw FileLockError();
	}
}

void Daemon::UnlockFile()
{
	int unlock_result = flock(_lock_file_descriptor, LOCK_UN);
	if (unlock_result == -1) {
		Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Error unlocking file.");
		throw FileLockError();
	}
}
