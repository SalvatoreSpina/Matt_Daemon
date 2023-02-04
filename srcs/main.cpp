#include "../includes/Daemon.hpp"
#include "../includes/Tintin_reporter.hpp"

// Function to handle signals
void HandleSignal(int signum) {
	(void)signum;
	Tintin_reporter::WriteLogs(Tintin_reporter::kInfo, "Signal handler.");
	throw Daemon::QuitRequested();
}

// Creates log/lock directories and files
void CreateEnvironment(int& lock_file_descriptor)
{
	int ret;

	if (getuid() != 0) {
		throw Daemon::NotRootError();
	}
	ret = mkdir(LOG_DIRECTORY, 0777);
	if (ret == -1 && errno != EEXIST) {
		perror(LOG_DIRECTORY);
		throw Daemon::FunctionCallError();
	}
	ret = mkdir(LOCK_DIRECTORY, 0777);
	if (ret == -1 && errno != EEXIST) {
		perror(LOCK_DIRECTORY);
		throw Daemon::FunctionCallError();
	}
	lock_file_descriptor = open(LOCK_FILE, O_CREAT, 0644);
	if (lock_file_descriptor == -1) {
		perror(LOCK_FILE);
		throw Daemon::FunctionCallError();
	}
}

int main()
{
	int pid, lock_file_descriptor;
	int ret = kOk;

	// Register signals to handle
	signal(SIGINT, HandleSignal);
	signal(SIGQUIT, HandleSignal);
	signal(SIGTERM, HandleSignal);

	try {
		CreateEnvironment(lock_file_descriptor);
		pid = fork();
		if (pid == 0) {
			Daemon daemon(lock_file_descriptor);
			daemon.LockFile();
			daemon.InitializeServer();
			daemon.Start();
		} else if (pid < 0) {
			perror("Daemon: fork");
			throw Daemon::FunctionCallError();
		}
	} catch(const Daemon::QuitRequested& e) {
		// Do nothing
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		ret = kFailed;
	}
	return ret;
}
