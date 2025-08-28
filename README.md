# Minishell - A Bash-like Shell Implementation

## Overview

**Minishell** (asfoorshell🐥) is a Unix shell implementation that mimics the behavior of bash. It provides a command-line interface for executing commands, managing processes, handling I/O redirection, and supporting advanced features like pipelines and logical operators. The project is built with careful attention to memory management, signal handling, and adherence to shell standards.

## Project Structure

```
minishell/
├── Makefile                    # Build configuration
├── includes/                   # Header files
│   ├── minishell.h            # Main header with all includes
│   ├── lexer.h                # Lexer/tokenizer definitions
│   ├── parser.h               # Parser and AST definitions  
│   ├── expander.h             # Variable expansion definitions
│   ├── execution.h            # Execution engine definitions
│   ├── enviroment.h           # Environment management definitions
│   ├── builtin.h              # Built-in commands definitions
│   └── redirection.h          # I/O redirection definitions
├── src/                       # Source code organized by modules
│   ├── lexer/                 # Tokenization and lexical analysis
│   ├── parser/                # Syntax analysis and AST building
│   ├── expander/              # Variable expansion and quote processing
│   ├── execution/             # Command execution engine
│   ├── enviroment/            # Environment variable management
│   ├── builtin/               # Built-in command implementations
│   ├── redirections/          # I/O redirection handling
│   ├── main.c                 # Program entry point
│   ├── signals.c              # Signal handling implementation
│   ├── signals_utils.c        # Signal utilities
│   ├── cleanup.c              # Memory cleanup functions
│   └── shell_utils.c          # General shell utilities
├── libft/                     # Custom C library
└── obj/                       # Object files (generated)
```

## Core Architecture

### Processing Pipeline
The shell processes input through a multi-stage pipeline:

1. **Input Reading** → Readline library for command input with history
2. **Lexical Analysis** → Tokenization with quote and operator recognition
3. **Variable Expansion** → Parameter expansion and quote removal
4. **Syntax Analysis** → Parse tokens into Abstract Syntax Tree (AST)
5. **Execution** → Execute AST with proper process and I/O management

### Main Shell Loop
```c
while (1) {
    setup_signals(SIGNAL_PARENT);
    line = readline("asfoorshell🐥:~ ");
    if (line == NULL) exit_shell(&shell);
    if (g_sig_value != 0) sig_status(&shell, &expander_creds);
    process_line(line, &shell, &parser, &expander_creds);
}
```

## Signal Handling System

### Signal Architecture
The shell implements sophisticated signal handling with different modes for different execution contexts:

```c
typedef enum e_signal_mode {
    SIGNAL_PARENT,    // Main shell signal handling
    SIGNAL_CHILD,     // Child process signal handling  
    SIGNAL_HEREDOC,   // Special handling during heredoc input
    SIGNAL_NONE       // Minimal signal handling during waiting
} t_signal_mode;
```

### Global Signal State
```c
volatile sig_atomic_t g_sig_value = 0;  // Global signal state
```

### Signal Modes in Detail

#### SIGNAL_PARENT Mode
**Used**: Main shell interactive loop
**Signals Handled**:
- `SIGINT` (Ctrl+C): 
  - Prints newline
  - Clears current input line
  - Redisplays prompt
  - Sets `g_sig_value = SIGINT`
  - Does NOT exit the shell
- `SIGQUIT` (Ctrl+\): Ignored (`SIG_IGN`)
- `SIGPIPE`: Custom handler to set global flag

**Implementation**:
```c
void handle_signal(int sig) {
    if (sig == SIGINT) {
        ft_putstr_fd("\n", 1);
        rl_on_new_line();           // Inform readline of new line
        rl_replace_line("", 0);     // Clear current input
        rl_redisplay();             // Redisplay prompt
        g_sig_value = SIGINT;
    }
}
```

#### SIGNAL_CHILD Mode  
**Used**: Child processes executing external commands
**Signals Handled**:
- `SIGINT`: Sets global flag and prints newline
- `SIGQUIT`: Default behavior (`SIG_DFL`) - allows core dump
- `SIGPIPE`: Default behavior (`SIG_DFL`)

**Behavior**: Allows child processes to handle signals naturally while updating shell state

#### SIGNAL_HEREDOC Mode
**Used**: During heredoc input processing
**Signals Handled**:
- `SIGINT`: 
  - Prints newline
  - Closes `STDIN_FILENO` to break out of heredoc input
  - Sets `g_sig_value = SIGINT`
- `SIGQUIT`: Ignored
- `SIGPIPE`: Custom handler

**Special Behavior**: SIGINT during heredoc input terminates the heredoc and the entire command

#### SIGNAL_NONE Mode
**Used**: While waiting for child processes
**Signals Handled**:
- `SIGINT`: Simple flag setting without readline interference
- `SIGQUIT`: Ignored

**Purpose**: Prevents readline interference while waiting for children

### Signal Handling Flow

#### Interactive Command Input
1. **Setup**: `setup_signals(SIGNAL_PARENT)`
2. **Readline**: User types command with signal handling active  
3. **Signal Response**: SIGINT clears line and redisplays prompt
4. **Processing**: Continue with command processing

#### Command Execution
1. **Child Creation**: Fork child process
2. **Child Signals**: `setup_signals(SIGNAL_CHILD)` in child
3. **Parent Waiting**: `setup_signals(SIGNAL_NONE)` while waiting
4. **Cleanup**: Restore parent signals after child completion

#### Heredoc Processing
1. **Setup**: `setup_signals(SIGNAL_HEREDOC)`
2. **Input Loop**: Read heredoc content with special SIGINT handling
3. **Interruption**: SIGINT closes stdin and terminates heredoc
4. **Cleanup**: Process interruption and cleanup

### Signal Status Handling
```c
void sig_status(t_shell *shell, t_expander_creds *expander_creds) {
    shell->status = 130;                    // Set exit status to 130
    expander_creds->last_exit_status = 130; // Update for $? expansion  
    g_sig_value = 0;                        // Reset signal flag
}
```

**Exit Status 130**: Standard Unix exit status for SIGINT (128 + 2)

## Program Flow

### Startup Process
1. **Initialize Shell State**:
   ```c
   initialize_shell(&shell, &expander_creds, argv, environment);
   ```
   - Allocate execution structure
   - Initialize environment from system environment
   - Set up expander credentials

2. **Setup Parser State**:
   ```c
   shell.parser = &parser;
   ```

3. **Check Interactive Mode**:
   ```c
   if (isatty(STDIN_FILENO))  // Interactive mode
   ```

### Command Processing Flow
1. **Setup Signals**: Configure parent signal handling
2. **Read Input**: Use readline for command input with history
3. **Handle Signals**: Process any pending signals
4. **Process Line**:
   - **Tokenize**: Break input into tokens
   - **Expand**: Perform variable expansion and quote removal
   - **Parse**: Build AST from tokens
   - **Execute**: Execute AST if no syntax errors
   - **Cleanup**: Free allocated memory

### Execution Process
1. **Preprocess Heredocs**: Handle all heredocs before execution
2. **Execute AST**: Recursive AST execution with proper signal handling
3. **Handle Results**: Update shell status and cleanup
4. **Memory Management**: Free AST and associated structures

### Cleanup and Exit
```c
void exit_shell(t_shell *shell) {
    cleanup(shell);      // Free all shell resources
    clear_history();     // Clear readline history
    printf("\rexit\n");  // Print exit message
    exit(0);             // Exit program
}
```

## Makefile Details

### Build Configuration
```makefile
NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Werror -Wextra
INCLUDES    = -I includes -I libft  
LDFLAGS     = -lreadline -L./libft -lft
```

### Directory Structure
```makefile
SRC_DIR     = src
OBJ_DIR     = obj  
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
```

### Source File Organization
The Makefile organizes source files by module:
- **Core**: `main.c`, `cleanup.c`, `shell_utils.c`, `signals.c`, `signals_utils.c`
- **Lexer**: All tokenization and lexical analysis files
- **Parser**: All parsing and AST construction files
- **Expander**: All variable expansion and quote processing files
- **Execution**: All command execution files
- **Environment**: All environment management files
- **Built-ins**: All built-in command implementations
- **Redirections**: All I/O redirection handling files

### Build Process
1. **Library Build**: 
   ```makefile
   $(LIBFT):
       @make -C $(LIBFT_DIR)
   ```

2. **Object Compilation**:
   ```makefile
   $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
       @mkdir -p $(dir $@)
       $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
   ```

3. **Final Linking**:
   ```makefile
   $(NAME): $(LIBFT) $(OBJ)
       $(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
   ```

### Make Targets
- `all`: Build the complete project
- `clean`: Remove object files
- `fclean`: Remove object files and executable  
- `re`: Rebuild everything from scratch

### Dependencies
- **libft**: Custom C library for utility functions
- **readline**: GNU readline library for command input and history
- **System Libraries**: Standard C library functions

## Features Implemented

### Core Shell Features
- ✅ Command execution with PATH resolution
- ✅ Built-in commands (echo, cd, pwd, export, unset, env, exit)
- ✅ I/O redirection (`<`, `>`, `>>`)
- ✅ Here documents (`<<`)
- ✅ Pipelines (`|`)
- ✅ Variable expansion (`$VAR`, `$?`)
- ✅ Quote handling (single and double quotes)
- ✅ Signal handling (SIGINT, SIGQUIT, SIGPIPE)
- ✅ History support via readline

### Bonus Features  
- ✅ Logical operators (`&&`, `||`)
- ✅ Subshells with parentheses `(cmd)`
- ✅ Advanced signal handling in different contexts

### Error Handling
- ✅ Syntax error detection and reporting
- ✅ Command not found handling
- ✅ Permission error handling  
- ✅ Memory allocation error handling
- ✅ Proper exit status propagation

## Memory Management

The shell implements comprehensive memory management:
- **Systematic Cleanup**: All allocated memory is properly freed
- **Error Recovery**: Cleanup on allocation failures
- **AST Management**: Recursive cleanup of complex AST structures
- **Token Lists**: Proper cleanup of linked token structures
- **Environment Variables**: Dynamic environment management with cleanup

## Testing and Reliability

### Signal Testing
The shell handles signals correctly in all contexts:
- Interactive prompt interruption
- Command execution interruption
- Heredoc interruption
- Pipeline signal propagation

### Memory Testing
- Valgrind clean under normal conditions
- Proper cleanup on exit
- Error condition memory handling

### Compatibility Testing
- Behavior matches bash in standard cases
- Proper exit status handling
- Compatible quote and expansion behavior

## Usage

### Compilation
```bash
make
```

### Execution
```bash
./minishell
```

### Example Commands
```bash
asfoorshell🐥:~ echo "Hello World"
asfoorshell🐥:~ ls -la | grep minishell
asfoorshell🐥:~ export USER=john && echo $USER
asfoorshell🐥:~ (cd /tmp && pwd) && pwd
asfoorshell🐥:~ cat << EOF > output.txt
Hello from heredoc
EOF
```

The shell provides a robust, feature-complete implementation of a Unix shell with careful attention to signal handling, memory management, and compatibility with standard shell behavior.
