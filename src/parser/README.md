# Parser Module - README

## Overview

The parser module transforms the token stream from the lexer into an Abstract Syntax Tree (AST) that represents the hierarchical structure of shell commands. It implements a recursive descent parser that handles operator precedence, redirections, and complex command structures including pipelines and logical operators.

## Core Components

### AST Node Types
The parser creates different types of AST nodes:
- `AST_SIMPLE_COMMAND` - Basic command with arguments and redirections
- `AST_PIPELINE` - Commands connected by pipes (`|`)
- `AST_LOGICAL_AND` - Commands connected by logical AND (`&&`)
- `AST_LOGICAL_OR` - Commands connected by logical OR (`||`) 
- `AST_SUBSHELL` - Commands grouped in parentheses `(cmd)`

### Parser State
The parser maintains state through `t_parser` structure:
- `current_token` - Pointer to current token being processed
- `error` - Error flag (0 = success, nonzero = syntax error)
- `errmsg` - Error message for detailed feedback

### AST Structure
Each AST node contains:
- `type` - Node type identifier
- Union of different node structures:
  - Simple command: `argv[]` and redirections list
  - Binary nodes: left and right child pointers
  - Subshell: child node and redirections

## Parsing Strategy

### Operator Precedence (Highest to Lowest)
1. **Parentheses/Subshells** - `(command)`
2. **Pipelines** - `cmd1 | cmd2 | cmd3`
3. **Logical AND** - `cmd1 && cmd2`
4. **Logical OR** - `cmd1 || cmd2`

### Grammar Structure
```
input_line     → logical_line
logical_line   → pipeline (('&&' | '||') pipeline)*
pipeline       → command ('|' command)*
command        → simple_command | group
group          → '(' logical_line ')' [redirections]
simple_command → [redirections] [arguments] [redirections]
```

## Key Functions

### Main Parsing Functions
- `parse_input_line()` - Entry point that parses complete input
- `parse_logical_line()` - Handles logical operators (&&, ||)
- `parse_pipeline()` - Handles pipe operators
- `parse_command()` - Dispatches to appropriate command parser
- `parse_simple_command()` - Parses basic commands with args/redirections
- `parse_group()` - Handles parenthesized command groups

### Core Helpers
- `advance()` - Moves to next token
- `match()` - Checks if current token matches expected type
- `expect()` - Consumes token or sets error
- `set_parse_error()` - Sets error state with message

### Node Constructors
- `ast_create_simple()` - Creates simple command nodes
- `ast_create_binary()` - Creates binary operator nodes
- `ast_create_subshell()` - Creates subshell grouping nodes

### Memory Management
- `free_ast()` - Recursively frees AST nodes
- `free_redirs()` - Frees redirection lists
- `free_argv()` - Frees argument arrays
- `cleanup_simple_cmd()` - Cleanup on parsing failures

## Edge Cases and Error Handling

### Syntax Error Detection
1. **Missing Commands**: `|`, `&&`, `||` without commands
2. **Unexpected Tokens**: Invalid token sequences
3. **Unclosed Groups**: Missing closing parentheses
4. **Empty Commands**: Pipes or logical operators with no command

### Redirection Parsing
1. **Multiple Redirections**: `cmd < file1 < file2 > output`
2. **Mixed Positioning**: `< input cmd > output`
3. **Heredoc Handling**: `cmd << EOF` with delimiter parsing
4. **Missing Targets**: `cmd >` without filename

### Memory Safety
1. **Partial Parse Cleanup**: Frees allocated memory on parse errors
2. **Argument Array Management**: Handles dynamic array growth
3. **Nested Structure Cleanup**: Properly frees complex AST structures

### Special Cases
1. **Empty Input**: Returns NULL AST
2. **Whitespace Only**: Handled at lexer level
3. **Comment Handling**: Not implemented (bash extension)
4. **Here Document Quotes**: Preserves quote state for heredoc delimiters

## File Structure

- `parser.c` - Main parsing logic and entry points
- `parse_simple_cmd.c` - Simple command and argument parsing
- `parse_command_dispatcher.c` - Command type dispatching
- `parser_core_helpers.c` - Core parser helper functions
- `parser_helpers.c` - Additional parsing utilities
- `ast_node_constructors.c` - AST node creation functions
- `memory_cleanup.c` - Memory management and cleanup
- `parse_redirs_utils.c` - Redirection parsing utilities

## Usage Example

Input: `echo hello | grep h && ls -la`

AST Structure:
```
AST_LOGICAL_AND
├── left: AST_PIPELINE
│   ├── left: AST_SIMPLE_COMMAND ["echo", "hello"]
│   └── right: AST_SIMPLE_COMMAND ["grep", "h"]
└── right: AST_SIMPLE_COMMAND ["ls", "-la"]
```

## Integration

The parser receives tokens from the lexer and produces an AST that is consumed by the execution module. The AST preserves the semantic structure needed for proper command execution order and handles operator precedence correctly. Redirections are attached to appropriate command nodes for processing during execution.
