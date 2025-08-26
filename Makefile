NAME        := minishell

# Compiler and flags
CC          = cc
CFLAGS      = -Wall -Werror -Wextra -g -fPIE
INCLUDES    = -I includes -I libft
LDFLAGS     = -lreadline -L./libft -lft

# Directories
SRC_DIR     = src
OBJ_DIR     = obj
LIBFT_DIR   = libft
LIBFT       := $(LIBFT_DIR)/libft.a
#INC_DIR = includes/


# Header (wrapper header includes everything)
HEADERS     = includes/minishell.h includes/lexer.h includes/parser.h

# **************************************************************************** #
#                                SOURCE FILES                                  #
# **************************************************************************** #
# Source files
SRC_FILES = \
    main.c \
    lexer/tokenizer.c \
    lexer/tokenizer_utils.c \
    lexer/lexer_utils.c \
    lexer/token_operations.c \
    lexer/lexer_final_utils.c \
    parser/ast_node_constructors.c \
    parser/memory_cleanup.c \
    parser/parse_command_dispatcher.c \
    parser/parse_redirs_utils.c \
    parser/parse_simple_cmd.c \
    parser/parser_core_helpers.c \
    parser/parser.c \
    parser/parser_helpers.c \
    enviroment/enviroment.c \
    enviroment/enviroment_utils.c \
    enviroment/enviroment_utils_extra.c \
    execution/execution.c \
    execution/execute_ast.c \
    execution/execution_utils.c \
    execution/execute_ast_utils.c \
    execution/logical_operators.c \
    execution/preprocess_heredoc.c \
    expander/core_expand.c \
    expander/core_expand_utils.c \
    expander/core_expand_helpers.c \
    expander/dispatcher_expander.c \
    expander/expand.c \
    expander/expander_utils.c \
    expander/mixed_expand.c \
    expander/segment_utils.c \
    expander/quote_start_end.c \
    expander/quote_processing.c \
    expander/parse_segments.c \
    expander/parse_segments_utils.c \
    expander/parse_segments_more_utils.c \
    expander/token_utils.c \
    expander/segment_processing.c \
    expander/type_utils.c \
    redirections/redirections.c \
    redirections/redirections_utils.c \
    redirections/heredoc.c \
    builtin/builtin.c \
    builtin/ft_cd.c \
    builtin/ft_echo.c \
    builtin/ft_exit.c \
    builtin/ft_export.c \
    builtin/ft_export_utils.c \
    builtin/ft_export_print.c \
    builtin/ft_pwd.c \
    builtin/ft_unset.c \
    signals.c \
    signals_utils.c \
    cleanup.c \
    shell_utils.c \



# prepend src/ to source files
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
# # Replace srcs/ with obj/ and .c with .o in one rule
# # SRC must already contain full paths (like srcs/...).
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# or in two rules => Generate corresponding object paths under obj/
# OBJ_FILES = $(SRC_FILES:%=$(OBJ_DIR)/%.o)
# OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
# Assumption = > SRC_FILES contains file paths without the leading srcs/.
#				 (relative paths)
# two-step, works on bare file paths and then prepends obj/.
# First, it transforms only the file names in SRC_FILES from .c to .o.
# (Here, SRC_FILES usually contains relative paths without srcs/, e.g. lexer/tokenizer.c)
# Then it prepends obj/ to every .o.

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

# Compile .c to .o (create directories as needed)
#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# := and = both assign values to variables
# but they work differently in terms of when the value is expanded.
# = = delayed expansion (recalculates each time).
# := = immediate expansion (locks in the value once).0.
