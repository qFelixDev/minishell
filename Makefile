# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 18:04:05 by reriebsc          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/07/23 18:40:48 by ghodges          ###   ########.fr        #
=======
#    Updated: 2025/07/23 18:36:04 by reriebsc         ###   ########.fr        #
>>>>>>> 25af65b0084e6176ff61ebf2dbc25b5ca2b7cd88
#                                                                              #
# **************************************************************************** #

##*************************************************************/
##                   Program Name 
##*************************************************************/
NAME := minishell



##                   Compiler 
##*************************************************************/
CC := cc
CFLAGS := -Wall -Werror -Wextra -g -fsanitize=address


##                   Pfade
##*************************************************************/
PATH_ENVIROMENT := env
PATH_LIBFT := libft_extend
PATH_BUILTINS := builtins
LIBFT := $(PATH_LIBFT)/libft.a
OBJ_DIR := obj


##                   Quellen
##*************************************************************/

SRCS := main.c\
		minishell.c\
		utils/utils_v1.c\
		user_commands/commants.c\
		signals/signals_v1.c\
		parser/command.c\
		parser/debug.c\
		parser/ms_allocate_command.c\
		parser/ms_check_syntax.c\
		parser/ms_expand_precedence.c\
		parser/ms_expand_wildcards.c\
		parser/ms_expand_wildcards_2.c\
		parser/ms_parse.c\
		parser/ms_resolve_strings.c\
		parser/ms_should_ignore.c\
		parser/ms_tokenize.c\
		parser/ms_traverse_delims.c\
		parser/ms_traverse_delims_2.c\
		parser/sequence.c\
		parser/token.c\
		gc_collector/gc_part1.c\
		gc_collector/gc_part2.c\
		gc_collector/gc_part3.c\
		execution/exe_1.c\
		execution/exe_2.c\
		execution/exe_2_1.c\
		execution/exe_2_2.c\
		execution/exe_v3.c\
		error/errorhandling.c\
		env/env_1.c\
		env/env_2.c\
		env/env_3.c\
		builtins/unset.c\
		builtins/cd.c\
		builtins/clear.c\
		builtins/export.c\
		builtins/echo.c\
		builtins/env.c\
		builtins/exit.c\
		builtins/pwd.c\
		ast_tree/main_pipes.c\
		ast_tree/pipes.c\

# Objekt-Dateien
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) qInterceptor.o -fsanitize=address,undefined -lreadline -o $@
	@echo "$(GREEN)✅ Executable $@ created successfully!$(RESET)"

# Objektdateien-Regel
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(PATH_LIBFT) -c $< -o $@

#@mkdir -p $(OBJ_PATH)

# libft bauen
$(LIBFT):
	@$(MAKE) -C $(PATH_LIBFT)

clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	@$(MAKE) -C $(PATH_LIBFT) clean
	$(RM) -r $(OBJ_DIR)
	@echo "$(RED)🧹 Object files removed$(RESET)"

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(PATH_LIBFT) fclean
	@echo "$(RED)🧹 Executables removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus