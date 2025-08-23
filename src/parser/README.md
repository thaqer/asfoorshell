# Parser Module

## Overview
The parser module converts the token stream from the lexer into an Abstract Syntax Tree (AST) that represents the hierarchical structure of commands, pipelines, logical operators, and redirections. It implements a recursive descent parser following shell grammar rules.

## Core Data Structures

### AST Node (`t_ast`)
```c
typedef struct s_ast
{
    t_ast_node_type type;
    union {
        struct {
            char    **argv;     // null-terminated argument array
            t_redir *redirs;    // linked list of redirections
        } s_simple_command;
        struct {
            struct s_ast *left;
            struct s_ast *right;
        } s_binary;             // for pipelines, logical ops
        struct {
            struct s_ast *child;
            t_redir      *redirs; // redirections for subshell
        } s_subshell;
    };
} t_ast;
```

### AST Node Types (`t_ast_node_type`)
- `AST_SIMPLE_COMMAND` - Basic command with args and redirections
- `AST_PIPELINE` - Commands connected by pipes (`|`)
- `AST_LOGICAL_AND` - Commands connected by `&&`
- `AST_LOGICAL_OR` - Commands connected by `||`
- `AST_SUBSHELL` - Commands grouped in parentheses

### Parser State (`t_parser`)
```c
typedef struct s_parser
{
    t_token *current_token;  // Current position in token stream
    int      error;          // Error flag (0 = ok, nonzero = error)
    char    *errmsg;         // Error message string
} t_parser;
```

## Main Parsing Functions

### `parse_input_line(t_parser *parser)`
**Purpose**: Entry point for parsing complete command line
**Returns**: Root AST node or NULL on error
**Process**:
1. Call `parse_logical_line()` for main parsing
2. Handle syntax errors and cleanup
3. Display error messages to stderr

### `parse_logical_line(t_parser *parser)`
**Purpose**: Parses logical operators (`&&`, `||`) - lowest precedence
**Grammar**: `pipeline (('&&' | '||') pipeline)*`
**Process**:
1. Parse left-side pipeline
2. While logical operators found:
   - Save operator type
   - Parse right-side pipeline
   - Create binary AST node

### `parse_pipeline(t_parser *parser)`
**Purpose**: Parses pipe operators (`|`) - higher precedence than logical
**Grammar**: `command ('|' command)*`
**Process**:
1. Parse left-side command
2. While pipe tokens found:
   - Parse right-side command
   - Create pipeline AST node
3. Left-associative parsing (builds right-heavy tree)

### `parse_command(t_parser *parser)`
**Purpose**: Parses individual commands or grouped commands
**Process**:
1. Check for subshell `(` token
2. If subshell: parse grouped commands
3. Otherwise: parse simple command
4. Handle redirections for both cases

### `parse_group(t_parser *parser)`
**Purpose**: Parses parenthesized command groups `(...)`
**Process**:
1. Expect and consume `(` token
2. Parse inner logical line recursively
3. Expect and consume `)` token
4. Parse any redirections after group
5. Create subshell AST node

### `parse_simple_command(t_parser *parser)`
**Purpose**: Parses basic commands with arguments and redirections
**Process**:
1. Parse leading redirections
2. Collect command name and arguments
3. Parse trailing redirections
4. Create simple command AST node

## Helper Functions

### Core Parser Helpers

#### `advance(t_parser *parser)`
**Purpose**: Moves to next token in stream
**Behavior**: Updates `current_token` pointer

#### `match(t_parser *parser, t_token_type type)`
**Purpose**: Checks if current token matches expected type
**Returns**: 1 if match, 0 otherwise
**Behavior**: Does not consume token

#### `expect(t_parser *parser, t_token_type type)`
**Purpose**: Consumes token if it matches expected type
**Returns**: Token pointer on success, sets error on failure
**Error Handling**: Sets parser error state on mismatch

#### `set_parse_error(t_parser *p, char *msg)`
**Purpose**: Sets error state and message
**Behavior**: Sets error flag and stores message

### AST Node Constructors

#### `ast_create_simple(char **argv, t_redir *redirs)`
**Purpose**: Creates simple command AST node
**Parameters**: 
- `argv`: Null-terminated argument array
- `redirs`: Linked list of redirections

#### `ast_create_binary(t_ast_node_type type, t_ast *left, t_ast *right)`
**Purpose**: Creates binary operation AST node (pipeline, logical ops)
**Parameters**:
- `type`: `AST_PIPELINE`, `AST_LOGICAL_AND`, or `AST_LOGICAL_OR`
- `left`/`right`: Child AST nodes

#### `ast_create_subshell(t_ast *child, t_redir *redirs)`
**Purpose**: Creates subshell AST node
**Parameters**:
- `child`: Inner command AST
- `redirs`: Redirections applied to entire subshell

### Argument Handling

#### `allocate_argv(t_parser copy)`
**Purpose**: Pre-allocates argv array based on token count
**Process**: Counts remaining WORD tokens to determine size

#### `add_args(t_parser *parser, char **argv, int *argc)`
**Purpose**: Collects consecutive WORD tokens into argv array
**Process**:
1. While current token is WORD:
2. Copy token value to argv
3. Advance to next token
4. Increment argument count

### Redirection Handling

#### `parse_redirections(t_parser *p, t_redir **list)`
**Purpose**: Parses and collects all redirection tokens
**Process**:
1. While current token is redirection:
2. Get redirection type from token
3. Expect target (filename/delimiter)
4. Add to redirection list

#### `is_redirection_tok(t_token *token)`
**Purpose**: Checks if token is a redirection operator
**Returns**: 1 for `<`, `>`, `>>`, `<<`; 0 otherwise

#### `token_to_redir_type(t_token_type type)`
**Purpose**: Converts token type to redirection type enum
**Mapping**:
- `TOKEN_REDIRECT_IN` → `REDIR_IN`
- `TOKEN_REDIRECT_OUT` → `REDIR_OUT`
- `TOKEN_APPEND` → `REDIR_APPEND`
- `TOKEN_HEREDOC` → `REDIR_HEREDOC`

#### `add_redirection(t_redir **head, t_token_type type, const char *target)`
**Purpose**: Creates and appends redirection to list
**Process**:
1. Allocate new redirection node
2. Set type and target
3. Append to linked list

## Memory Management

### `free_ast(t_ast *n)`
**Purpose**: Recursively frees entire AST
**Process**:
1. Switch on node type
2. Free type-specific data (argv arrays, child nodes)
3. Free redirection lists
4. Free node itself

### `free_redirs(t_redir *r)`
**Purpose**: Frees linked list of redirections
**Process**: Iterate and free target strings and nodes

### `free_argv(char **argv)`
**Purpose**: Frees null-terminated string array
**Process**: Free individual strings then array

### Cleanup Helpers

#### `cleanup_simple_cmd(char **argv, int argc, t_redir *redirs)`
**Purpose**: Cleanup on simple command parse failure
**Process**: Free partial argv array and redirections

#### `clean_group(t_ast *inner, t_redir *redirs)`
**Purpose**: Cleanup on subshell parse failure
**Process**: Free inner AST and redirections

## Error Handling

### Syntax Errors
- **Missing operators**: Commands without separators
- **Incomplete redirections**: Missing filenames/delimiters
- **Unmatched parentheses**: Missing opening/closing parens
- **Empty commands**: Operators without commands

### Error Recovery
- **Cleanup**: Free partial AST structures
- **Messages**: Specific error messages for different syntax errors
- **State**: Reset parser state for next input

## Grammar Rules

```
input_line     := logical_line
logical_line   := pipeline (('&&' | '||') pipeline)*
pipeline       := command ('|' command)*
command        := simple_command | '(' logical_line ')' redirection*
simple_command := redirection* word+ redirection*
redirection    := ('<' | '>' | '>>' | '<<') word
```

## Processing Flow

1. **Token Stream Input**: Receive tokens from lexer
2. **Recursive Descent**: Parse using grammar rules
3. **Precedence Handling**: Logical ops < Pipeline < Command
4. **AST Construction**: Build tree structure
5. **Error Checking**: Validate syntax at each step
6. **Memory Management**: Cleanup on errors
7. **Return**: Complete AST ready for execution

## Usage Example

```c
// Input tokens: ["echo", "hello", "|", "wc", "-l"]
t_parser parser;
parser.current_token = token_list;
t_ast *ast = parse_input_line(&parser);

// Result AST:
// AST_PIPELINE
// ├── left: AST_SIMPLE_COMMAND (argv=["echo", "hello"])
// └── right: AST_SIMPLE_COMMAND (argv=["wc", "-l"])
```

## Integration Points
- **Input**: Token stream from lexer
- **Output**: AST consumed by execution engine
- **Memory**: AST freed after execution
- **Errors**: Syntax errors reported to user
