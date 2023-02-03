#pragma once
#include "utilities.hpp"

class Tintin_reporter
{
	private:
		// Delete copy constructor and assignment operator to make the class non-copyable
		Tintin_reporter(const Tintin_reporter&) = delete;
		Tintin_reporter& operator=(const Tintin_reporter&) = delete;

	public:
		// Use default to make constructor and destructor explicit and non-copyable
		Tintin_reporter() = default;
		~Tintin_reporter() = default;

		// Use constexpr for compile-time constants
		static constexpr const char* kInfo = "INFO";
		static constexpr const char* kLog = "LOG";
		static constexpr const char* kError = "ERROR";

		static void WriteLogs(const std::string &code, const std::string &msg)
		{
			char timestamp[100];
			std::time_t now = std::time(nullptr);

			// Get the current timestamp
			std::strftime(timestamp, sizeof(timestamp), "[%d/%m/%Y-%T]", std::localtime(&now));

			// Open the log file in append mode
			std::ofstream log_file(LOG_FILE, std::fstream::app);

			// Check if the file was successfully opened
			if (!log_file) {
				throw std::exception();
			}

			// Write the log message
			log_file << timestamp << " [ " << code << " ] - Matt_daemon: " << msg << '\n';
		}
};
