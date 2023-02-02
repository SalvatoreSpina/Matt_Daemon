NAME =		matt_daemon

COMP =		clang++
CFLAGS =	-Wall -Werror -Wextra

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
	@fclean $(NAME)