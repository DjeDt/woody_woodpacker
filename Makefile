#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddinaut <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/26 16:31:33 by ddinaut           #+#    #+#              #
#    Updated: 2019/04/22 16:25:12 by ddinaut          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Output file #
NAME	= woody_woodpacker

# Details #
CC			= gcc
FLAGS		= -Wall -Wextra -Werror
ADDFLAGS	= #-fsanitize=address -g3

# Directories
OBJ_DIR		= .obj
SRC_DIR		= srcs
INC_DIR		= includes/

INCLUDES	= -I $(INC_DIR)

# Sources #
SRCS =					\
	packer.c			\
	criteria.c			\
	elf_section.c		\
	elf_segment.c		\
	elf_header.c		\
	infection.c			\
	create_infected.c	\
	lib.c

SRCS_ASM = \
	rc4.s

SRC_FILES = $(addprefix $(SRC_DIR)/,$(SRCS))
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ASM_FILES = $(addprefix $(SRC_DIR)/,$(SRCS_ASM))
OBJ_FILE_ASM = $(ASM_FILES:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)

# Rules #
.PHONY: all clean fclean re

all: $(NAME)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(FLAGS) $(ADDFLAGS) $(INCLUDES) -c $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

$(NAME): $(OBJ_FILES) $(OBJ_FILE_ASM)
	$(CC) -o $(NAME) $(FLAGS) $(ADDFLAGS) $(OBJ_FILES) $(OBJ_FILE_ASM)

clean:
	@/bin/rm -f $(OBJ_FILES) $(OBJ_FILE_ASM)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
