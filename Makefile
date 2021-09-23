# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/13 14:58:45 by sgoffaux          #+#    #+#              #
#    Updated: 2021/09/23 13:46:20 by rvan-aud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFT_A		=	libft.a
LIBFT_DIR	=	libft/
LIBFT		=	$(addprefix $(LIBFT_DIR), $(LIBFT_A))

CC			=	gcc
INCLUDE		=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE) -I/Users/$(USER)/.brew/opt/readline/include -fsanitize=address -g
CPPFLAGS 	= $(CFLAGS)
RM			=	rm -f

SRCS		=	srcs/execution/exec_pipes_forks.c	\
				srcs/execution/exec_pipes_utils.c	\
				srcs/execution/exec_pipes.c			\
				srcs/execution/exec.c				\
				srcs/execution/here_doc.c			\
				srcs/execution/path_handling.c		\
				srcs/parsing/free.c					\
				srcs/parsing/parsing.c				\
				srcs/parsing/replace_env.c			\
				srcs/parsing/tokenizer_utils.c		\
				srcs/parsing/tokenizer.c			\
				srcs/parsing/utils.c				\
				srcs/builtins.c						\
				srcs/signal.c						\
				main.c 								\

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/Users/$(USER)/.brew/opt/readline/lib  $(OBJS) -o $(NAME)
			@echo "Linked into executable \033[0;32mminishell\033[0m."

$(LIBFT):
			@$(MAKE) -s -C $(LIBFT_DIR)
			@echo "Compiled libft.a"

.c.o:
			@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
			@echo "Compiling $<."

localclean:
			@$(RM) $(OBJS)
			@echo "Removed object files."

clean:		localclean
			@$(MAKE) clean -s -C $(LIBFT_DIR)
			@echo "Clean libft."

fclean:		localclean
			@$(MAKE) fclean -s -C $(LIBFT_DIR)
			@echo "Full clean libft."
			@$(RM) $(NAME)
			@echo "Removed executable."

re:			fclean all

.PHONY:		all clean fclean localclean re