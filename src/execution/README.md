# Execution Module

## Overview
The execution module traverses the AST created by the parser and executes commands, handling pipes, redirections, logical operators, and process management. It coordinates with the builtin module for internal commands and manages external process execution.

## Core Data Structures

### Shell Execution Context (`t_execution`)
```c
typedef struct s_execution
{
    int saved_stdin;     // Backup of original stdin
    int saved_stdout;    // Backup of original stdout
} t_execution;
```

### Signal Modes (`t_signal_mode`)
```c
typedef enum e_signal_mode
{
    SIGNAL_PARENT,    // Interactive parent shell
    SIGNAL_CHILD,     // Child processes
    SIGNAL_HEREDOC,   // During heredoc input
    SIGNAL_NONE       // No signal handling
} t_signal_mode;
```

### Global Signal Variable
```c
extern volatile sig_atomic_t g_sig_value;  // Current signal number
```

## Main Execution Functions

### `execute_ast(t_ast *ast, t_shell *shell)`
**Purpose**: Main dispatcher for AST node execution
**Process**:
1. Check for NULL AST node
2. Switch on AST node type
3. Call appropriate execution function
4. Return exit status

**Supported Node Types**:
- `AST_SIMPLE_COMMAND` → `execute_simple_command()`
- `AST_PIPELINE` → `execute_pipeline()`
- `AST_LOGICAL_AND` → `execute_logical_and()`
- `AST_LOGICAL_OR` → `execute_logical_or()`
- `AST_SUBSHELL` → `execute_subshell()`

### `execute_simple_command(t_ast *ast, t_shell *shell)`
**Purpose**: Executes basic commands with arguments and redirections
**Process**:
1. Handle redirections first
2. Check if command is builtin
3. If builtin: execute in current process
4. If external: fork and execute
5. Restore redirections
6. Return exit status

**Key Features**:
- Builtin detection and execution
- Redirection setup and cleanup
- External command path resolution
- Error handling and status codes

### `execute_pipeline(t_ast *ast, t_shell *shell)`
**Purpose**: Executes commands connected by pipes
**Process**:
1. Create pipe with `pipe()` system call
2. Fork for left command:
   - Redirect stdout to pipe write end
   - Close pipe read end
   - Execute left command
3. Fork for right command:
   - Redirect stdin from pipe read end
   - Close pipe write end
   - Execute right command
4. Close both pipe ends in parent
5. Wait for both children
6. Return exit status of rightmost command

**Pipeline Flow**:
```
cmd1 | cmd2 | cmd3
└─stdout─┐  ┌─stdin──┐
         pipe      pipe
         └─stdin───┘  └─stdout─→
```

### `execute_logical_and(t_ast *ast, t_shell *shell)`
**Purpose**: Executes `&&` logical AND operations
**Logic**:
1. Execute left command
2. If left succeeds (exit status 0):
   - Execute right command
   - Return right command's status
3. If left fails:
   - Skip right command
   - Return left command's status

### `execute_logical_or(t_ast *ast, t_shell *shell)`
**Purpose**: Executes `||` logical OR operations
**Logic**:
1. Execute left command
2. If left fails (non-zero exit status):
   - Execute right command
   - Return right command's status
3. If left succeeds:
   - Skip right command
   - Return left command's status

### `execute_subshell(t_ast *ast, t_shell *shell)`
**Purpose**: Executes commands in parentheses `(...)`
**Process**:
1. Handle subshell redirections
2. Fork new process for subshell
3. In child: execute inner command
4. In parent: wait for child completion
5. Restore redirections
6. Return child exit status

## Helper Functions

### External Command Execution

#### `execute_external_command(char **argv, t_shell *shell)`
**Purpose**: Executes external programs
**Process**:
1. Find command in PATH
2. Fork child process
3. In child: `execve()` the command
4. In parent: wait for completion
5. Return child exit status

#### `find_command_in_path(char *cmd, t_shell *shell)`
**Purpose**: Locates executable in PATH directories
**Process**:
1. If command contains '/', use as absolute path
2. Get PATH environment variable
3. Split PATH into directories
4. Search each directory for executable
5. Return full path or NULL

#### `search_in_path_dirs(char **paths, char *cmd)`
**Purpose**: Helper to search PATH directories
**Process**:
1. Iterate through path directories
2. Construct full path: `directory/command`
3. Check if file exists and is executable
4. Return first match found

#### `fork_and_execute(char *command_path, char **argv, t_shell *shell)`
**Purpose**: Forks and executes external command
**Process**:
1. Fork child process
2. In child:
   - Set up signal handling
   - Convert environment to char**
   - `execve(command_path, argv, envp)`
3. In parent:
   - Set up signal handling
   - Wait for child completion
   - Handle interrupted system calls

### Redirection Management

#### `handle_redirections(t_redir *redirs, t_shell *shell)`
**Purpose**: Sets up all redirections for a command
**Process**:
1. Save current stdin/stdout
2. Process each redirection in list
3. Handle different redirection types
4. Return success/failure status

#### `save_and_redirect(t_redir *redirs, t_shell *shell, int *saved_stdin, int *saved_stdout)`
**Purpose**: Apply redirections and save original file descriptors
**Redirection Types**:
- `REDIR_IN` (`<`): Redirect stdin from file
- `REDIR_OUT` (`>`): Redirect stdout to file (truncate)
- `REDIR_APPEND` (`>>`): Redirect stdout to file (append)
- `REDIR_HEREDOC` (`<<`): Redirect stdin from heredoc

#### `restore_std_fds(int saved_stdin, int saved_stdout)`
**Purpose**: Restore original stdin/stdout after command execution
**Process**:
1. `dup2()` saved descriptors back to 0/1
2. Close saved descriptors
3. Reset shell execution state

#### `restore_and_clear_std_fds(t_shell *shell, int saved_stdin, int saved_stdout)`
**Purpose**: Restore and clear shell's saved descriptors
**Process**: Restore descriptors and reset shell state

### Pipeline Helpers

#### `handle_left_pipeline(int *pipefd, t_ast *ast, t_shell *shell)`
**Purpose**: Execute left side of pipeline
**Process**:
1. Fork child process
2. In child:
   - Close pipe read end
   - Redirect stdout to pipe write end
   - Execute AST node
3. Return child PID

#### `handle_right_pipeline(int *pipefd, t_ast *ast, t_shell *shell)`
**Purpose**: Execute right side of pipeline
**Process**:
1. Fork child process
2. In child:
   - Close pipe write end
   - Redirect stdin from pipe read end
   - Execute AST node
3. Return child PID

### Heredoc Processing

#### `preprocess_all_heredocs(t_ast *ast, t_shell *shell)`
**Purpose**: Process all heredocs before execution
**Process**:
1. Recursively traverse AST
2. Find all heredoc redirections
3. Read heredoc content from user
4. Store in temporary files or memory
5. Update redirection targets

#### `preprocess_left_heredocs(t_ast *ast, t_shell *shell)`
**Purpose**: Process heredocs for left side of binary operations
**Usage**: Ensures heredocs are processed before forking

#### `cleanup_heredoc_fds(t_ast *ast)`
**Purpose**: Clean up heredoc file descriptors
**Process**: Close and remove temporary heredoc files

## Signal Handling

### `setup_signals(int mode)`
**Purpose**: Configure signal handlers based on context
**Modes**:
- `SIGNAL_PARENT`: Handle Ctrl-C, Ctrl-\\ in interactive mode
- `SIGNAL_CHILD`: Default signal handling for child processes
- `SIGNAL_HEREDOC`: Special handling during heredoc input
- `SIGNAL_NONE`: Ignore signals

### `handle_signal(int sig)`
**Purpose**: Signal handler function
**Process**:
1. Set global signal variable
2. Handle SIGINT (Ctrl-C) appropriately
3. Handle SIGQUIT (Ctrl-\\) appropriately
4. Update shell state if needed

## Process Management

### Fork and Wait Patterns
1. **Simple Commands**: Fork, exec, wait
2. **Pipelines**: Multiple forks, pipe setup, coordinate waits
3. **Subshells**: Fork for isolation, wait for completion
4. **Background**: Fork without waiting (bonus feature)

### Exit Status Handling
- **Success**: Exit status 0
- **Command not found**: Exit status 127
- **Permission denied**: Exit status 126
- **Signal termination**: Exit status 128 + signal number
- **Builtin errors**: Various status codes

### Error Handling
- **System call failures**: Handle EINTR, EAGAIN
- **Memory allocation**: Check malloc failures
- **File descriptor limits**: Handle EMFILE, ENFILE
- **Path resolution**: Handle command not found

## Execution Flow

### Simple Command Flow
1. **Setup**: Save stdin/stdout, setup redirections
2. **Decision**: Check if builtin or external
3. **Builtin Path**: Execute in current process
4. **External Path**: Fork, find in PATH, exec
5. **Cleanup**: Restore redirections, get exit status

### Pipeline Flow
1. **Preprocessing**: Handle heredocs for all commands
2. **Pipe Creation**: Create pipe file descriptors
3. **Left Fork**: Execute left command with stdout → pipe
4. **Right Fork**: Execute right command with stdin ← pipe
5. **Parent**: Close pipe, wait for both children
6. **Status**: Return rightmost command's exit status

### Logical Operation Flow
1. **Left Execution**: Execute left command, get status
2. **Decision**: Based on operator and left status
3. **Right Execution**: Execute right command if needed
4. **Status**: Return appropriate exit status

## Integration Points
- **Input**: AST from parser
- **Builtins**: Coordinate with builtin module
- **Environment**: Access shell environment variables
- **Redirections**: Coordinate with redirection module
- **Signals**: Handle user interrupts appropriately
