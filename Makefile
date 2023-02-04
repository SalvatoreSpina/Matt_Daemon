NAME =		matt_daemon

COMP =		c++
CPPFLAGS =	-Wall -Werror -Wextra -std=c++20

SRCS =		srcs/main.cpp

OBJS =		$(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME):	$(OBJS)
	$(COMP) $(CPPFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:	
	@make fclean
	@make all

cleanfile:
	@sudo rm -rf /var/log/matt_daemon/matt_daemon.log

find:
	@ps aux | grep matt_daemon

findlock:
	@ls -ali /var/lock/ | grep matt