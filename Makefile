NAME		=	minishell

LIBFT_A		=	libft.a
LIBFT_DIR	=	libft/
LIBFT		=	$(addprefix $(LIBFT_DIR), $(LIBFT_A))

CC			=	gcc
INCLUDE		=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE) -fsanitize=address -g
CPPFLAGS 	= $(CFLAGS)
RM			=	rm -f

SRCS		=	srcs/builtins.c		\
				srcs/exec.c 		\
				srcs/parsing.c 		\
				srcs/signal.c		\
				main.c 				\

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/Users/$(USER)/.brew/opt/readline/lib $(OBJS) -I/Users/$(USER)/.brew/opt/readline/include -o $(NAME)
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