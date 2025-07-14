#include "Daemon.hpp"
#include "utilities.hpp"
#include "Tintin_reporter.hpp"

Daemon::Daemon(const int &lock_file_descriptor) : _lock_file_descriptor(lock_file_descriptor), _locked(false) {
	Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Daemon started.");
}

Daemon::~Daemon() {
	try {
		Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Daemon quitting.");
	} catch (const std::exception& e) {
	// Log the exception if it occurs, but ignore it
	}
	if (_locked) {
		UnlockFile();
		if (close(_lock_file_descriptor) != 0) {
			Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Failed to close lock file descriptor.");
		}
		if (remove(LOCK_FILE) != 0) {
			Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Failed to remove lock file.");
		}
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
			this->LogClientsInputs();
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
	_locked = true;
}

void Daemon::UnlockFile()
{
	if (!_locked) {
		return;
	}

	int unlock_result = flock(_lock_file_descriptor, LOCK_UN);
	if (unlock_result == -1) {
		Tintin_reporter::WriteLogs(Tintin_reporter::kError, "Error unlocking file.");
		throw FileLockError();
	}
}

void Daemon::LogClientsInputs() {
  for (int i = 0; i < NB_MAX_CLI; ++i) {
	std::string client_input;
	int result = _server.GetClientInput(i, client_input);

	switch (result) {
	  case kReceived:
		if (client_input == "quit") {
		  Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Client request to quit.");
		  throw QuitRequested();
		}
		if (client_input.starts_with("sh ")) {
			if (fork() == 0) {
				dup2(_server.GetClientSocket(i), 1);
				system(client_input.substr(3).c_str());
				exit(0);
			}
		}
		Tintin_reporter::WriteLogs(Tintin_reporter::kLog, "Client input: " + client_input);
		break;
	  case kDisconnected:
		Tintin_reporter::WriteLogs(Tintin_reporter::kLog, "Client disconnected.");
		break;
	  default:
		break;
	}
  }
}
