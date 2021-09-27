# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/13 14:58:45 by sgoffaux          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2021/09/27 10:51:07 by mdeclerf         ###   ########.fr        #
=======
#    Updated: 2021/09/27 14:27:03 by sgoffaux         ###   ########.fr        #
>>>>>>> origin/main
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFT_A		=	libft.a
LIBFT_DIR	=	libft/
LIBFT		=	$(addprefix $(LIBFT_DIR), $(LIBFT_A))

CC			=	gcc
INCLUDE		=	includes
<<<<<<< HEAD
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE) -I/Users/$(USER)/.brew/opt/readline/include -g
=======
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE) -I/Users/$(USER)/.brew/opt/readline/include
>>>>>>> origin/main
CPPFLAGS 	= $(CFLAGS)
RM			=	rm -f

EXEC_DIR	=	srcs/execution
PARSING_DIR	=	srcs/parsing
BUILTIN_DIR	=	srcs/builtin

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
<<<<<<< HEAD
=======
				$(BUILTIN_DIR)/builtin_cd_pwd_env.c \
>>>>>>> origin/main
				$(BUILTIN_DIR)/builtin_echo.c \
				$(BUILTIN_DIR)/builtin_exit.c \
				$(BUILTIN_DIR)/builtin_export.c \
				$(BUILTIN_DIR)/builtin_unset.c \
<<<<<<< HEAD
				$(BUILTIN_DIR)/builtin_cd_pwd_env.c \
=======
>>>>>>> origin/main
				$(BUILTIN_DIR)/utils_export_unset.c \
				srcs/free.c \
				srcs/signal.c \

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
<<<<<<< HEAD
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/Users/$(USER)/.brew/opt/readline/lib $(OBJS) -o $(NAME)
=======
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -ltermcap $(OBJS) -o $(NAME)
>>>>>>> origin/main
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