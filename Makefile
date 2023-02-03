NAME =		matt_daemon

COMP =		clang++
CFLAGS =	-Wall -Werror -Wextra -std=c++11

SRCS =		srcs/main.cpp

OBJS =		$(SRCS:%.cpp=%.o)

$(NAME):	$(SRCS)
	@$(COMP) $(CFLAGS) $(SRCS) -o $(NAME)

all:
	@$(NAME)

clean:
	@$(RM) $(OBJS)

fclean:		clean
	@$(RM) $(NAME)

re:	
	@make fclean
	@make $(NAME)

cleanfile:
	@sudo rm -rf /var/log/matt_daemon/matt_daemon.log

find:
	@ps aux | grep matt_daemon

findlock:
	@ls -ali /var/lock/ | grep matt