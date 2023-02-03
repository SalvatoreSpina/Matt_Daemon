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