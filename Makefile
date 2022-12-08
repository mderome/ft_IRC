# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mderome <mderome@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/30 13:35:26 by esafar            #+#    #+#              #
#    Updated: 2022/12/07 18:18:49 by mderome          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS =	srcs/main.cpp \
		srcs/parsing.cpp \
		srcs/server.cpp	\
		srcs/ServerCmd.cpp	\
		srcs/user.cpp \
		srcs/returncode.cpp \
		srcs/timestamp.cpp \
		srcs/Channel.cpp	\
		srcs/splitov.cpp

GCC = c++
FLAGS = -g -Wextra -Wall -Werror -std=c++98 
OBJS = ${SRCS:.cpp=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${GCC} ${FLAGS} ${OBJS} -I includes -o ${NAME}

clean:
	rm -f ${OBJS}

fclean:
	rm -f ${OBJS} ${NAME}

re:	fclean all

%.o: %.cpp ${INCL}
	${GCC} ${FLAGS} -c $< -o ${<:.cpp=.o}

.PHONY: all clean fclean re