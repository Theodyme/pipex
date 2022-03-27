# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: flplace <flplace@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/27 15:12:17 by flplace           #+#    #+#              #
#    Updated: 2022/03/27 15:28:28 by flplace          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	main.c \
			pathfinder.c \
			utils.c \
			builders.c \
			ft_split.c \

OBJ	= ${SRCS:.c=.o}


NAME	= pipex

CC	= gcc

CFLAGS	= -Wall -Wextra -Werror

%.o: 	%.c
	${CC} ${CFLAGS} -c $< -o $@

$(NAME):	${OBJ}
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)


all:	${NAME}

clean:
	rm -f ${OBJ}

fclean:	clean
	rm -f ${NAME}

re:	fclean all

.PHONY:	all clean fclean re
