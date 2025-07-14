NAME =		Matt_daemon

COMP =		c++
CPPFLAGS =	-Wall -Werror -Wextra -std=c++20

SRCS =		srcs/main.cpp

OBJS =		$(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME):	$(OBJS)
	$(COMP) $(CPPFLAGS) $^ -o $@

clean:
	@echo "→ Killing any running Matt_daemon…"
	-@sudo pkill -SIGTERM -f Matt_daemon 2>/dev/null
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:	
	@make fclean
	@make all

cleanfile:
	@sudo rm -rf /var/log/matt_daemon/matt_daemon.log

find:
	@ps aux | grep Matt_daemon

findlock:
	@ls -ali /var/lock/ | grep matt