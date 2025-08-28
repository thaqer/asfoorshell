# Execution Module - README

## Overview

The execution module is responsible for executing the Abstract Syntax Tree (AST) produced by the parser. It handles different types of commands including simple commands, pipelines, logical operators, and subshells. The module manages process creation, I/O redirection, built-in command execution, and proper signal handling during execution.

## Core Components

### AST Execution Types
The execution module handles all AST node types:
- `AST_SIMPLE_COMMAND` - Execute single command with redirections
- `AST_PIPELINE` - Execute commands connected by pipes
- `AST_LOGICAL_AND` - Execute second command only if first succeeds
- `AST_LOGICAL_OR` - Execute second command only if first fails
- `AST_SUBSHELL` - Execute commands in a separate shell process

### Execution Context
The execution state is maintained through:
```c
typedef struct s_execution {
    int saved_stdin;     // Backup of original stdin
    int saved_stdout;    // Backup of original stdout
} t_execution;
```

### Signal Modes
Different execution contexts require different signal handling:
- `SIGNAL_PARENT` - Main shell signal handling
- `SIGNAL_CHILD` - Child process signal handling  
- `SIGNAL_HEREDOC` - Special handling during heredoc input
- `SIGNAL_NONE` - Minimal signal handling during waiting

## Key Functions

### Main Execution Dispatcher
- `execute_ast()` - Main entry point that dispatches based on AST type
- `execute_simple_command()` - Executes basic commands
- `execute_pipeline()` - Handles pipe execution
- `execute_logical_and()` - Implements && logic
- `execute_logical_or()` - Implements || logic  
- `execute_subshell()` - Executes commands in subshell

### Command Execution
- `execute_external_command()` - Handles external program execution
- `find_command_in_path()` - Locates executables in PATH
- `fork_and_execute()` - Creates child process and executes command
- `is_directory()` - Checks if command path is a directory

### I/O Management
- `handle_redirections()` - Sets up file redirections
- `save_and_redirect()` - Saves original fds and applies redirections
- `restore_std_fds()` - Restores original stdin/stdout
- `restore_and_clear_std_fds()` - Restoration with cleanup

### Pipeline Handling
- `handle_left_pipeline()` - Manages left side of pipe
- `handle_right_pipeline()` - Manages right side of pipe
- `preprocess_all_heredocs()` - Processes heredocs before execution
- `cleanup_heredoc_fds()` - Cleans up heredoc file descriptors

## Execution Flow

### Simple Command Execution
1. **Save Standard FDs**: Backup stdin/stdout for restoration
2. **Apply Redirections**: Set up input/output redirections
3. **Check Built-in**: Determine if command is built-in
4. **Execute**: Call appropriate execution function
5. **Restore FDs**: Restore original stdin/stdout

### Pipeline Execution  
1. **Create Pipe**: Set up pipe between commands
2. **Fork Left Process**: Execute left command with stdout to pipe
3. **Fork Right Process**: Execute right command with stdin from pipe
4. **Close Pipe FDs**: Close pipe descriptors in parent
5. **Wait for Children**: Wait for both processes to complete
6. **Return Status**: Return exit status of rightmost command

### Logical Operator Execution
1. **Execute Left**: Run first command
2. **Check Status**: Evaluate exit status
3. **Conditional Execute**: Run second command based on operator and status
   - `&&`: Execute right only if left succeeded (status 0)
   - `||`: Execute right only if left failed (status ≠ 0)

### Subshell Execution
1. **Fork Process**: Create child process for subshell
2. **Set Child Signals**: Configure signal handling for child
3. **Apply Redirections**: Handle subshell redirections
4. **Execute Child**: Execute AST in child process
5. **Wait for Child**: Parent waits for subshell completion

## Redirection Handling

### Redirection Types
- `REDIR_IN` - Input redirection `<`
- `REDIR_OUT` - Output redirection `>`
- `REDIR_APPEND` - Append redirection `>>`
- `REDIR_HEREDOC` - Here document `<<`

### Heredoc Processing
1. **Preprocessing**: Process all heredocs before command execution
2. **Temporary Files**: Create temporary files for heredoc content
3. **Signal Handling**: Special SIGINT handling during heredoc input
4. **Expansion**: Apply variable expansion to heredoc content (unless quoted)
5. **Cleanup**: Remove temporary files after execution

### File Descriptor Management
- Proper saving and restoration of stdin/stdout
- Cleanup of unused file descriptors
- Handling of file descriptor limits
- Error handling for file operations

## Error Handling and Edge Cases

### Command Not Found
1. **PATH Search**: Search through PATH directories
2. **Executable Check**: Verify file is executable
3. **Directory Check**: Handle case where command is a directory
4. **Error Reporting**: Proper error messages for different failure types

### Permission and Access Errors
1. **File Access**: Handle permission denied errors
2. **Directory Traversal**: Check directory access permissions
3. **Redirection Errors**: Handle file creation/access failures
4. **Pipe Errors**: Handle pipe creation failures

### Signal Handling During Execution
1. **SIGINT Handling**: Interrupt handling in different contexts
2. **SIGPIPE Handling**: Broken pipe handling in pipelines
3. **Child Signal Setup**: Proper signal configuration for child processes
4. **Signal Restoration**: Restore signal handlers after execution

### Memory and Resource Management
1. **Process Cleanup**: Proper child process cleanup
2. **File Descriptor Leaks**: Prevention of fd leaks
3. **Memory Allocation**: Error handling for memory failures
4. **Resource Limits**: Handling of system resource limits

## File Structure

- `execute_ast.c` - Main AST execution dispatcher
- `execute_ast_utils.c` - AST execution utilities
- `execution.c` - Core execution logic
- `execution_utils.c` - Execution utility functions
- `logical_operators.c` - Logical operator (&&, ||) handling
- `preprocess_heredoc.c` - Heredoc preprocessing logic

## Integration with Other Modules

### Built-in Commands
- Integrates with builtin module for built-in command execution
- Handles built-in vs external command distinction
- Proper exit status handling for built-ins

### Environment Variables
- Uses environment module for variable access
- Passes environment to external commands
- Handles environment modifications

### Signal System
- Coordinates with signal module for proper signal handling
- Different signal contexts for different execution phases
- Proper signal restoration after execution

## Usage Example

For command: `echo hello | grep h && ls -la > output.txt`

Execution Flow:
1. **Pipeline**: Execute `echo hello | grep h`
   - Fork for `echo hello` with stdout to pipe
   - Fork for `grep h` with stdin from pipe
   - Wait for both, get exit status
2. **Logical AND**: If pipeline succeeded, execute `ls -la > output.txt`
   - Set up output redirection to `output.txt`
   - Execute `ls -la`
   - Restore redirections

The module ensures proper process management, signal handling, and resource cleanup throughout the execution process.
