# Lexer Module - README

## Overview

The lexer module is responsible for tokenizing input strings into a stream of tokens that represent different shell components. It handles quote parsing, separators detection, and special character recognition. The lexer produces a linked list of tokens that serves as input to the parser.

## Core Components

### Token Types
The lexer recognizes the following token types:
- `TOKEN_WORD` - Regular words/commands/arguments
- `TOKEN_PIPE` - Pipe operator (`|`)
- `TOKEN_REDIRECT_IN` - Input redirection (`<`)
- `TOKEN_REDIRECT_OUT` - Output redirection (`>`)
- `TOKEN_APPEND` - Append redirection (`>>`)
- `TOKEN_HEREDOC` - Here document (`<<`)
- `TOKEN_AND` - Logical AND (`&&`) - Bonus feature
- `TOKEN_OR` - Logical OR (`||`) - Bonus feature
- `TOKEN_OPEN_PAREN` - Opening parenthesis (`(`) - Bonus feature
- `TOKEN_CLOSE_PAREN` - Closing parenthesis (`)`) - Bonus feature

### Quote Flags (qflags)
Each token contains quote flags that track quote usage:
- `QF_NONE` - No quotes
- `QF_HAS_SQ` - Contains single quotes
- `QF_HAS_DQ` - Contains double quotes
- `QF_ALL_SQ` - Entirely single-quoted
- `QF_ALL_DQ` - Entirely double-quoted
- `QF_MIXED` - Mixed quoting (quoted + unquoted or both quote types)
- `QF_HD_QUOTED` - Heredoc delimiter is quoted
- `QF_EXPANDED` - Token has been expanded

### State Machine
The lexer uses a state machine to handle quote parsing:
- `STATE_NORMAL` - Outside quotes
- `STATE_SQUOTE` - Inside single quotes
- `STATE_DQUOTE` - Inside double quotes

## Key Functions

### Main Tokenization
- `tokenize(char *input)` - Main entry point that tokenizes input string
- `state_determinant()` - Updates state based on quote characters
- `normal_state_handler()` - Processes tokens when in normal state
- `is_separator()` - Identifies separator characters
- `handle_double_char_sep()` - Handles double-character operators (&&, ||, <<, >>)

### Token Management
- `create_token()` - Creates a new token with specified properties
- `append()` - Appends token to the linked list
- `free_tok_list()` - Frees the entire token list
- `compute_qflags_from_raw()` - Computes quote flags from raw string

## Edge Cases and Special Handling

### Quote Handling
1. **Empty Quotes**: `""` and `''` are preserved as empty tokens
2. **Nested Quotes**: Single quotes inside double quotes are treated literally
3. **Unclosed Quotes**: Returns error state if quotes are not properly closed
4. **Adjacent Quotes**: `"hello"'world'` is treated as a single word token

### Separator Recognition
1. **Multi-character Operators**: `<<`, `>>`, `&&`, `||` are recognized as single tokens
2. **Whitespace**: Multiple consecutive spaces are treated as single separator
3. **Quote Context**: Separators inside quotes are treated as literal characters

### Special Cases
1. **Empty Input**: Returns NULL
2. **Whitespace Only**: Returns NULL after trimming
3. **Quote State Errors**: Handles unclosed quotes gracefully
4. **Memory Allocation**: Proper cleanup on allocation failures

## File Structure

- `tokenizer.c` - Main tokenization logic and state management
- `tokenizer_utils.c` - Utility functions for tokenization process
- `lexer_utils.c` - Token creation, manipulation, and memory management
- `token_operations.c` - Token list operations and transformations
- `lexer_final_utils.c` - Final processing and cleanup utilities

## Usage Example

```c
t_token *tokens = tokenize("echo \"hello world\" | grep test");
// This produces tokens: [WORD:"echo"], [WORD:"hello world"], [PIPE], [WORD:"grep"], [WORD:"test"]
```

The lexer properly handles the quoted string as a single word token while recognizing the pipe as a separate operator token.

## Integration

The lexer output is consumed by the parser module, which uses the token stream to build an Abstract Syntax Tree (AST). The quote flags are particularly important for the expander module to determine how to handle variable expansion and quote removal.
