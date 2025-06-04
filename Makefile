# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 18:04:05 by reriebsc          #+#    #+#              #
#    Updated: 2025/06/04 18:14:23 by reriebsc         ###   ########.fr        #
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


##                   Quellen
##*************************************************************/

SRC = 


# Objekt-Dateien
OBJS = $(SRCS:.c=.o)

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)✅ Executable $@ created successfully!$(RESET)"

# Objektdateien-Regel
%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -c $< -o $@

#@mkdir -p $(OBJ_PATH)

# libft bauen
$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	@$(MAKE) -C $(LIBFT_PATH) clean
	@echo "$(RED)🧹 Object files removed$(RESET)"

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@echo "$(RED)🧹 Executables removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus