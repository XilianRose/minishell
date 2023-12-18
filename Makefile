# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mstegema <mstegema@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/08/01 18:15:02 by cschabra      #+#    #+#                  #
#    Updated: 2023/12/18 18:56:19 by cschabra      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CFLAGS = -Wall -Werror -Wextra ${HEADERS} #-Wunreachable-code -Ofast

LIBFT = 42lib

# READLINE_DIR = $(shell brew --prefix readline)
# READLINE_LIB = -lreadline -lhistory -L $(READLINE_DIR)/lib
READLINE_LIB = -lreadline -lhistory

HEADERS	= -I include -I ${LIBFT}/include -I $(READLINE_DIR)/include
LIBS	= ${LIBFT}/libft.a

SRCS	= srcs/execution/builtins/builtin.c \
srcs/execution/builtins/cd.c \
srcs/execution/builtins/env.c \
srcs/execution/builtins/export_append.c \
srcs/execution/builtins/export_builtin.c \
srcs/execution/builtins/export_utils.c \
srcs/execution/builtins/pwd_utils.c \
srcs/execution/builtins/unset.c \
srcs/execution/childprocess.c \
srcs/execution/errors.c \
srcs/execution/executor.c \
srcs/execution/fds.c \
srcs/execution/heredoc.c \
srcs/execution/pathfinder.c \
srcs/execution/pipes.c \
srcs/execution/signals.c \
srcs/expander/expander_ppid.c \
srcs/expander/expander_utils.c \
srcs/expander/expander.c \
srcs/lexer/lexer_split.c \
srcs/lexer/lexer_utils.c \
srcs/lexer/lexer.c \
srcs/parser/parser_utils.c \
srcs/parser/parser.c \
srcs/parser/prelim_parser_utils.c \
srcs/parser/prelim_parser.c \
srcs/utilities/atollong.c \
srcs/utilities/bubblesort.c \
srcs/utilities/exec_utils.c \
srcs/utilities/free.c \
srcs/utilities/free2.c \
srcs/utilities/list_utilities.c \
srcs/utilities/node_mem_alloc.c \
srcs/utilities/string_utilities.c \
srcs/main.c

OBJS	= ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	@${MAKE} -C ${LIBFT}
	@${CC} -o ${NAME} ${CFLAGS} ${SRCS} ${LIBS} ${READLINE_LIB}

clean:
	@rm -f ${OBJS}
	@${MAKE} -C ${LIBFT} clean

fclean: clean
	@rm -f ${NAME}
	@${MAKE} -C ${LIBFT} fclean

re: fclean all

debug: CFLAGS += -g #-fsanitize=address
debug: re

.PHONY: all, clean, fclean, re, debug
