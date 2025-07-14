#pragma once
\
#include "Server.hpp"

class Daemon
{
	private:
		// Delete default constructor, copy constructor, and assignment operator
		Daemon() = delete;
		Daemon(const Daemon&) = delete;
		Daemon& operator=(const Daemon&) = delete;

		int _lock_file_descriptor;
		bool _locked;
		Server _server;

		void LogClientsInputs();

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

		class NotRootError : public std::exception {
		public:
			const char* what() const throw() {
				return "Matt daemon must be run as root.";
			}
		};
};
