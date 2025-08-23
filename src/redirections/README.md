# Redirections Module

## Overview
The redirections module handles all input/output redirection operations in the shell, including file redirections (`<`, `>`, `>>`), and here-documents (`<<`). It manages file descriptor manipulation, heredoc processing, and coordinates with the execution engine to ensure proper I/O flow.

## Core Data Structures

### Redirection Node (`t_redir`)
```c
typedef struct s_redir
{
    t_redir_type     type;              // Redirection type
    char            *target;            // Target file or delimiter
    int             heredoc_fd;         // File descriptor for heredoc
    bool            s_delim_quoted;     // Heredoc delimiter quoted flag
    struct s_redir  *next;             // Next redirection in list
} t_redir;
```

### Redirection Types (`t_redir_type`)
```c
typedef enum e_redir_type
{
    REDIR_IN,       // < input redirection
    REDIR_OUT,      // > output redirection (truncate)
    REDIR_APPEND,   // >> output redirection (append)
    REDIR_HEREDOC   // << here-document
} t_redir_type;
```

## Main Functions

### `handle_redirections(t_redir *redirs, t_shell *shell)`
**Purpose**: Main entry point for processing all redirections
**Process**:
1. Save current stdin/stdout file descriptors
2. Process each redirection in the linked list
3. Apply redirections in order (left to right)
4. Handle different redirection types appropriately
5. Return success/error status

**Redirection Processing Order**:
- Input redirections can be mixed with output redirections
- Last redirection of each type takes precedence
- Example: `cmd < file1 > file2 < file3` → reads from file3, writes to file2

### `handle_input_redirection(t_redir *redir, t_shell *shell)`
**Purpose**: Handles `<` input redirection
**Process**:
1. Open target file for reading
2. Check file exists and is readable
3. Duplicate file descriptor to stdin (fd 0)
4. Close original file descriptor
5. Handle errors (file not found, permission denied)

### `handle_output_redirection(t_redir *redir, t_shell *shell)`
**Purpose**: Handles `>` output redirection (truncate)
**Process**:
1. Open/create target file for writing
2. Truncate file if it exists
3. Set permissions (typically 0644)
4. Duplicate file descriptor to stdout (fd 1)
5. Close original file descriptor
6. Handle errors (permission denied, disk full)

### `handle_append_redirection(t_redir *redir, t_shell *shell)`
**Purpose**: Handles `>>` append redirection
**Process**:
1. Open/create target file for appending
2. Seek to end of file (O_APPEND flag)
3. Set permissions if creating new file
4. Duplicate file descriptor to stdout (fd 1)
5. Close original file descriptor

### `handle_heredoc_redirection(t_redir *redir, t_shell *shell)`
**Purpose**: Handles `<<` here-document redirection
**Process**:
1. Check if heredoc was preprocessed (heredoc_fd set)
2. If preprocessed: duplicate heredoc_fd to stdin
3. If not preprocessed: call interactive heredoc handler
4. Handle quoted vs unquoted delimiters
5. Apply variable expansion if delimiter not quoted

## Here-document Processing

### `handle_heredoc_redir(char *delimiter, t_shell *shell)`
**Purpose**: Interactive heredoc input processing
**Process**:
1. Set up special signal handling for heredoc mode
2. Create temporary file or pipe for heredoc content
3. Read lines from user until delimiter found
4. Process variable expansion (if delimiter not quoted)
5. Write processed content to temporary storage
6. Return file descriptor for reading

### `preprocess_all_heredocs(t_ast *ast, t_shell *shell)`
**Purpose**: Process all heredocs before command execution
**Why Needed**: Heredocs must be processed before forking to ensure proper user interaction
**Process**:
1. Recursively traverse AST tree
2. Find all nodes with heredoc redirections
3. Process each heredoc immediately
4. Store file descriptors for later use
5. Handle nested heredocs in complex commands

**Traversal Pattern**:
- Simple commands: process direct heredocs
- Pipelines: process left side first, then right side
- Logical operators: process left side only (right may not execute)
- Subshells: process inner command heredocs

### `cleanup_heredoc_fds(t_ast *ast)`
**Purpose**: Clean up heredoc file descriptors after execution
**Process**:
1. Traverse AST tree
2. Close all heredoc file descriptors
3. Remove temporary files if used
4. Reset heredoc_fd values to -1

### Heredoc Variable Expansion
**Quoted Delimiter**: `<< "EOF"` or `<< 'EOF'`
- No variable expansion
- All text literal
- Preserve exactly as typed

**Unquoted Delimiter**: `<< EOF`
- Variable expansion enabled
- `$VAR` and `${VAR}` expanded
- `$?` expanded to exit status
- Backslash escaping supported

## File Descriptor Management

### Descriptor Backup and Restoration
```c
// Save original descriptors
int saved_stdin = dup(STDIN_FILENO);
int saved_stdout = dup(STDOUT_FILENO);

// Apply redirections
// ... redirection operations ...

// Restore original descriptors
dup2(saved_stdin, STDIN_FILENO);
dup2(saved_stdout, STDOUT_FILENO);
close(saved_stdin);
close(saved_stdout);
```

### File Opening Flags
- **Input (`<`)**: `O_RDONLY`
- **Output (`>`)**: `O_WRONLY | O_CREAT | O_TRUNC`
- **Append (`>>`)**: `O_WRONLY | O_CREAT | O_APPEND`
- **Default permissions**: `0644` (rw-r--r--)

## Error Handling

### File Access Errors
- **File not found** (`ENOENT`): "No such file or directory"
- **Permission denied** (`EACCES`): "Permission denied"
- **Is directory** (`EISDIR`): "Is a directory"
- **Too many open files** (`EMFILE`): "Too many open files"

### Error Messages Format
```c
// Format: "minishell: filename: error description"
fprintf(stderr, "minishell: %s: %s\n", filename, strerror(errno));
```

### Heredoc Error Handling
- **Interrupted input**: Handle Ctrl-C gracefully
- **EOF before delimiter**: Warning about missing delimiter
- **Disk space**: Handle write failures during heredoc storage

## Integration with Execution

### Execution Flow Integration
1. **Before Command**: `handle_redirections()` sets up I/O
2. **During Command**: Command reads/writes using redirected I/O
3. **After Command**: Restore original file descriptors

### Pipeline Integration
- **Left Command**: May have output redirections
- **Right Command**: May have input redirections
- **Pipe + Redirect**: `cmd1 | cmd2 > file` (pipe to cmd2, cmd2 output to file)

### Builtin Integration
- **Builtin Commands**: Also use redirections
- **Same Interface**: Builtins see redirected I/O
- **Example**: `echo "hello" > file` works with builtin echo

## Advanced Features

### Multiple Redirections
```bash
# Multiple inputs - last one wins
cmd < file1 < file2  # reads from file2

# Multiple outputs - last one wins  
cmd > file1 > file2  # writes to file2

# Mixed redirections
cmd < input.txt > output.txt 2> errors.txt
```

### Redirection with Heredocs
```bash
# Heredoc with output redirection
cmd << EOF > output.txt
heredoc content
EOF
```

### Subshell Redirections
```bash
# Redirections apply to entire subshell
(cmd1; cmd2) > file.txt
(cmd1 | cmd2) < input.txt
```

## Utility Functions

### `save_and_redirect(t_redir *redirs, t_shell *shell, int *saved_stdin, int *saved_stdout)`
**Purpose**: Apply redirections and save original descriptors
**Usage**: Combined save and redirect operation for efficiency

### `restore_std_fds(int saved_stdin, int saved_stdout)`
**Purpose**: Restore original stdin/stdout
**Usage**: Cleanup after command execution

### File Descriptor Utilities
- Check if file descriptor is valid
- Safely close file descriptors
- Handle dup2() failures
- Manage temporary files

## Signal Handling During Heredocs

### Special Heredoc Mode
- **SIGINT (Ctrl-C)**: Interrupt heredoc input, return to prompt
- **SIGQUIT (Ctrl-\\)**: Ignore during heredoc input
- **EOF (Ctrl-D)**: Terminate heredoc input

### Signal Setup
```c
setup_signals(SIGNAL_HEREDOC);  // Special mode for heredoc
```

## Processing Flow

### Simple Redirection Flow
1. **Parse**: Parser creates redirection nodes
2. **Setup**: Save original file descriptors
3. **Apply**: Open files and dup2() to std descriptors
4. **Execute**: Command runs with redirected I/O
5. **Restore**: Restore original file descriptors
6. **Cleanup**: Close opened files

### Heredoc Processing Flow
1. **Detection**: Parser identifies heredoc redirections
2. **Preprocessing**: Process all heredocs before execution
3. **Storage**: Store heredoc content in temporary files/pipes
4. **Execution**: Redirect stdin from stored content
5. **Cleanup**: Remove temporary storage after execution

### Complex Command Flow
```bash
# Example: (cmd1; cmd2 << EOF) | cmd3 > file
# 1. Preprocess heredoc for cmd2
# 2. Setup pipe for (subshell) | cmd3
# 3. Setup output redirection for cmd3
# 4. Execute subshell with heredoc
# 5. Execute cmd3 with pipe input and file output
```

## Integration Points
- **Parser**: Creates redirection nodes during parsing
- **Execution**: Calls redirection functions before command execution
- **Environment**: Access environment for variable expansion in heredocs
- **Signals**: Special signal handling during heredoc input
- **Error System**: Reports file access errors to user
