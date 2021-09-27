# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/13 14:58:45 by sgoffaux          #+#    #+#              #
#    Updated: 2021/09/27 12:06:33 by sgoffaux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFT_A		=	libft.a
LIBFT_DIR	=	libft/
LIBFT		=	$(addprefix $(LIBFT_DIR), $(LIBFT_A))

CC			=	gcc
INCLUDE		=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE) -I/Users/$(USER)/.brew/opt/readline/include
CPPFLAGS 	= $(CFLAGS)
RM			=	rm -f

EXEC_DIR	=	srcs/execution
PARSING_DIR	=	srcs/parsing

SRCS		=	main.c \
				$(PARSING_DIR)/parsing.c \
				$(PARSING_DIR)/utils.c \
				$(PARSING_DIR)/replace_env.c \
				$(PARSING_DIR)/tokenizer.c \
				$(PARSING_DIR)/tokenizer_utils.c \
				$(EXEC_DIR)/exec_pipes_forks.c \
				$(EXEC_DIR)/exec_pipes_utils.c \
				$(EXEC_DIR)/exec_pipes.c \
				$(EXEC_DIR)/exec.c \
				$(EXEC_DIR)/heredoc.c \
				$(EXEC_DIR)/path_handling.c \
				srcs/free.c \
				srcs/builtins.c \
				srcs/signal.c \

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -ltermcap $(OBJS) -o $(NAME)
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