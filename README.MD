# Matt Daemon

Matt Daemon is a custom daemon project that runs on the background of the system. It aims to provide a simple and efficient way to initialize and manage a server in a Linux environment.

The project uses the fork system call to create a new process and runs the daemon in the background. It also registers signals such as SIGINT, SIGQUIT, and SIGTERM to handle graceful exits.

## Main

This is the main file for a Daemon project.

### Function HandleSignal

`HandleSignal` function is a signal handler that is called when a signal is received. This function throws an exception of `Daemon::QuitRequested` to exit the program.

### Function CreateEnvironment

`CreateEnvironment` function creates the log/lock directories and files required for the program to run. If the user running the program is not root, a `Daemon::FilePermissionError` is thrown. If there is an error in creating the log/lock directories or files, a `Daemon::FunctionCallError` is thrown.

### Main Function

The main function is the entry point of the program.
It first registers signals to be handled and creates the environment using the `CreateEnvironment` function.
Then it forks a child process to run the `Daemon` class. If the fork is unsuccessful, a `Daemon::FunctionCallError` is thrown.
If there is any error in the program, a `std::exception` is caught and the program returns with a failed status.

## Class Server

A class that implements a server that can handle multiple clients.

### Members

`_socket`: An integer that stores the file descriptor of the server's socket.

- `_address`: A sockaddr_in struct that stores the address of the server.

- `_file_descriptors`: An fd_set that holds the file descriptors of all clients.

- `_address_len`: An integer that stores the length of the _address struct.

- `_client_sockets`: A vector of integers that holds the file descriptors of all clients.

### Methods

- `Server()`: A constructor that initializes the member variables.

- `~Server()`: A destructor that closes all client sockets.

- `Init()`: Initializes the server socket and starts listening for incoming connections.

- `NewClient()`: Accepts a new client connection.

- `GetClientInput(int client_index, std::string &input)`: Retrieves the input received from a particular client and stores it in the input string.

- `ResetFds()`: Resets the _file_descriptors set.

- `GetFds()`: Returns a reference to the _file_descriptors set.

- `GetMaxFd()`: Returns the highest file descriptor number in the _file_descriptors set.

- `GetClientCount()`: Returns the number of clients currently connected to the server.

- `IsNewClient()`: Checks if a new client has connected to the server.

### Enumerations

`ConnectStatus`: An enumeration with three values: `kOk`, `kFailed`, and `kRefused`. Used to indicate the status of the connection attempt.

`InputStatus`: An enumeration with two values: `kReceived`, and `kDisconnected`. Used to indicate the status of the client input.

## Class Daemon

The `Daemon` class is responsible for the daemon process that runs in the background and starts the server. 

### Members
- `int _lock_file_descriptor`: a file descriptor for the lock file that the daemon uses to ensure only one instance of the daemon is running at a time
- `Server _server`: the `Server` instance that handles client connections and communication.

### Methods

- `explicit Daemon(const int &lock_file_descriptor)`: the constructor takes in a lock file descriptor and initializes the lock file descriptor.

- `~Daemon()`: the destructor.

- `void Start()`: starts the daemon and runs it until a `QuitRequested` exception is thrown.

- `void InitializeServer()`: initializes the `Server` instance.

- `void LockFile()`: locks the lock file so that only one instance of the daemon can run at a time.

- `void UnlockFile()`: unlocks the lock file.

- `class FunctionCallError`: an exception class that is thrown if a function call fails.

- `class FileLockError`: an exception class that is thrown if there is a problem with the lock file.

- `class FilePermissionError`: an exception class that is thrown if there is a problem with file permissions.

- `class QuitRequested`: an exception class that is thrown if a signal is received to stop the daemon.

## Class Tintin_reporter

The `Tintin_reporter` class is used to log messages to a log file. 

### Private Members
- `Tintin_reporter(const Tintin_reporter&) = delete`: The copy constructor is deleted to make the class non-copyable. 
- `Tintin_reporter& operator=(const Tintin_reporter&) = delete`: The assignment operator is deleted to make the class non-copyable. 

### Public Members

- `Tintin_reporter() = default`: The default constructor is used to make the constructor explicit and non-copyable.

- `~Tintin_reporter() = default`: The default destructor is used to make the destructor explicit and non-copyable.

- `static constexpr const char* kInfo = "INFO"`: A compile-time constant for the info log code. 

- `static constexpr const char* kLog = "LOG"`: A compile-time constant for the log code. 

- `static constexpr const char* kError = "ERROR"`: A compile-time constant for the error log code. 

- `static void WriteLogs(const std::string &code, const std::string &msg)`: This method writes a log message to a file. The method takes two arguments, `code`, and `msg`. `code` represents the type of the log and `msg` is the information that needs to be logged. The method starts by getting the current timestamp and stores it in the timestamp variable. Then it opens the log file in append mode and writes the log message to the file. The log message format includes the timestamp, code, and the message. If the file was not successfully opened, the method throws an exception.
