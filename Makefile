NAME =		matt_daemon
NAME_GUI =	Ben_AFK

COMP =		clang++
CPPFLAGS =	-Wall -Werror -Wextra -std=c++20 $$(pkg-config --cflags gtk4)
LDFLAGS =	$$(pkg-config --libs gtk4)

SRCS =		srcs/main.cpp
SRCS_GUI =	srcs/ben_afk.cpp

OBJS =		$(SRCS:%.cpp=%.o)
OBJS_GUI =	$(SRCS_GUI:%.cpp=%.o)

all: $(NAME) $(NAME_GUI)

$(NAME):	$(OBJS)
	@$(COMP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

$(NAME_GUI):	$(OBJS_GUI)
	@$(COMP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@$(RM) $(OBJS) $(OBJS_GUI)

fclean:		clean
	@$(RM) $(NAME) $(NAME_GUI)

re:	
	@make fclean
	@make all

cleanfile:
	@sudo rm -rf /var/log/matt_daemon/matt_daemon.log

find:
	@ps aux | grep matt_daemon

findlock:
	@ls -ali /var/lock/ | grep matt