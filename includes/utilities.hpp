#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <fstream>
#include <exception>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/file.h>
#include <errno.h>
#include <cstdlib>
#include <sstream>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <vector>

#define PORT 4242
#define NB_MAX_CLI 3
#define BUFFER_SIZE 1024
#define LOG_DIRECTORY "/var/log/matt_daemon"
#define LOCK_DIRECTORY "/var/lock"
#define LOG_FILE LOG_DIRECTORY "/matt_daemon.log"
#define LOCK_FILE LOCK_DIRECTORY "/matt_daemon.lock"
