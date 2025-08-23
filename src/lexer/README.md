# Lexer Module

## Overview
The lexer is the first stage of the minishell processing pipeline. It transforms raw input strings into a sequence of tokens that can be understood by the parser. This module handles quote processing, operator recognition, and word separation.

## Core Data Structures

### Token (`t_token`)
```c
typedef struct s_token
{
    char            *value;        // Token content
    t_token_type    type;          // Token classification
    t_qflags        qflags;        // Quote/expansion flags
    struct s_token  *next;         // Linked list pointer
} t_token;
```

### Token Types (`t_token_type`)
- `TOKEN_WORD` - Regular words and arguments
- `TOKEN_PIPE` - `|` pipeline operator
- `TOKEN_REDIRECT_IN` - `<` input redirection
- `TOKEN_REDIRECT_OUT` - `>` output redirection
- `TOKEN_APPEND` - `>>` append redirection
- `TOKEN_HEREDOC` - `<<` here-document
- `TOKEN_AND` - `&&` logical AND (bonus)
- `TOKEN_OR` - `||` logical OR (bonus)
- `TOKEN_OPEN_PAREN` - `(` subshell start (bonus)
- `TOKEN_CLOSE_PAREN` - `)` subshell end (bonus)

### Quote Flags (`t_qflags`)
- `QF_NONE` - No quotes
- `QF_HAS_SQ` - Contains single quotes
- `QF_HAS_DQ` - Contains double quotes
- `QF_ALL_SQ` - Entirely single-quoted
- `QF_ALL_DQ` - Entirely double-quoted
- `QF_MIXED` - Mixed quoted/unquoted content
- `QF_HD_QUOTED` - Heredoc delimiter is quoted

## Main Functions

### `tokenize(char *input)`
**Purpose**: Main entry point that converts input string to token list
**Process**:
1. Initialize state and pointers
2. Iterate through input character by character
3. Handle different states (normal, single-quote, double-quote)
4. Create tokens when separators are found
5. Return linked list of tokens

**Flow**:
```
Input string → State machine → Separator detection → Token creation → Token list
```

### `is_separator(char *current_position)`
**Purpose**: Identifies separators and operators in input
**Returns**: `t_separator_type` enum value
**Logic**:
- Checks for double-character operators first (`>>`, `<<`, `&&`, `||`)
- Then single-character operators (`|`, `<`, `>`, `(`, `)`)
- Handles quotes as special separators
- Returns `SEP_NONE` if not a separator

### `state_determinant(t_state *state, char **s_ref)`
**Purpose**: Updates parsing state based on quote characters
**States**:
- `STATE_NORMAL` - Outside quotes
- `STATE_SQUOTE` - Inside single quotes
- `STATE_DQUOTE` - Inside double quotes

**Behavior**:
- Single quotes: Everything literal until matching quote
- Double quotes: Allow variable expansion, escape sequences
- Nested quotes: Handle properly based on current state

### `append_separator(t_token **head, t_separator_type type, char **input_ref, char **start_ref)`
**Purpose**: Creates and appends tokens when separators are found
**Process**:
1. Create word token from accumulated characters (if any)
2. Create operator token for the separator (if not space)
3. Update input position and reset word start pointer

## Utility Functions

### `create_token(char *value, t_token_type type, t_qflags flags)`
**Purpose**: Allocates and initializes a new token
**Returns**: Pointer to new token or NULL on failure

### `append(t_token **head, char *value, t_token_type type, t_qflags flags)`
**Purpose**: Appends a new token to the end of the token list
**Returns**: Pointer to the new token

### `compute_qflags_from_raw(char *s)`
**Purpose**: Analyzes a string to determine its quote flags
**Logic**:
- Scans string for quote characters
- Determines if entirely quoted, partially quoted, or mixed
- Sets appropriate flags for expansion behavior

### `ft_strndup(const char *s, size_t n)`
**Purpose**: Creates a null-terminated copy of first n characters
**Usage**: Extract token values from input string

### `free_tok_list(t_token *head)`
**Purpose**: Frees entire token list and associated memory
**Process**: Iterates through list, freeing value strings and token structures

## Processing Flow

1. **Input Reception**: Raw command line string
2. **Character Scanning**: Iterate through each character
3. **State Management**: Track quote state (normal/single/double)
4. **Separator Detection**: Identify operators and word boundaries
5. **Token Creation**: Build tokens with proper types and flags
6. **Quote Flag Computation**: Analyze quote patterns for expansion
7. **List Assembly**: Create linked list of tokens
8. **Return**: Complete token list ready for parsing

## Key Features

### Quote Handling
- **Single Quotes**: Preserve all characters literally
- **Double Quotes**: Allow variable expansion, handle escapes
- **Mixed Quotes**: Track complex quoting patterns
- **Quote Removal**: Flags indicate when quotes should be stripped

### Operator Recognition
- **Multi-character**: `>>`, `<<`, `&&`, `||` detected first
- **Single-character**: `|`, `<`, `>`, `(`, `)`
- **Context Awareness**: Different behavior inside/outside quotes

### Error Handling
- **Memory Allocation**: Check malloc failures
- **Invalid Input**: Handle malformed quotes gracefully
- **Cleanup**: Proper memory management on errors

## Usage Example

```c
char *input = "echo \"hello world\" > file.txt";
t_token *tokens = tokenize(input);

// Result: 4 tokens:
// 1. TOKEN_WORD: "echo" (QF_NONE)
// 2. TOKEN_WORD: "hello world" (QF_ALL_DQ)
// 3. TOKEN_REDIRECT_OUT: ">" (QF_NONE)
// 4. TOKEN_WORD: "file.txt" (QF_NONE)
```

## Integration Points
- **Input**: Raw command line from readline
- **Output**: Token list consumed by parser
- **Memory**: Tokens freed after parsing complete
- **Errors**: Malformed input handled gracefully
