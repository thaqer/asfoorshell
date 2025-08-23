# Builtin Module

## Overview
The builtin module implements shell built-in commands that must be executed within the shell process itself rather than as external programs. These commands can modify the shell's internal state, such as changing directories, setting environment variables, or exiting the shell.

## Core Data Structures

### Builtin Command Structure (`t_builtin`)
```c
typedef struct s_builtin
{
    char *name;                                    // Command name
    int  (*func)(char **args, t_shell *shell);     // Function pointer
} t_builtin;
```

### Builtin Command Table
```c
static t_builtin g_builtins[] = {
    {"echo", ft_echo_argv},
    {"cd", ft_cd_argv},
    {"pwd", ft_pwd},
    {"export", ft_export_argv},
    {"unset", ft_unset_argv},
    {"env", print_env},
    {"exit", ft_exit_argv},
    {NULL, NULL}
};
```

## Main Interface Functions

### `is_builtin_command(char *cmd)`
**Purpose**: Determines if a command is a builtin
**Process**:
1. Compare command name against builtin table
2. Return 1 if found, 0 otherwise
**Usage**: Called by execution engine before external command lookup

### `execute_builtin(char **argv, t_shell *shell)`
**Purpose**: Executes the appropriate builtin command
**Process**:
1. Look up command in builtin table
2. Call corresponding function with argv and shell
3. Return exit status from builtin function

## Built-in Commands

### `ft_echo_argv(char **argv, t_shell *shell)`
**Purpose**: Implements the `echo` command
**Options**:
- `-n`: Suppress trailing newline
**Process**:
1. Check for `-n` flag
2. Print arguments separated by spaces
3. Add newline unless `-n` specified
4. Return 0 on success

**Example**: `echo -n "Hello World"` → `Hello World` (no newline)

### `ft_cd_argv(char **argv, t_shell *shell)`
**Purpose**: Implements the `cd` command (change directory)
**Features**:
- No arguments: Change to HOME directory
- Single argument: Change to specified directory
- Updates PWD and OLDPWD environment variables
**Process**:
1. Determine target directory
2. Save current directory as OLDPWD
3. Call `chdir()` system call
4. Update PWD environment variable
5. Handle errors (permission denied, directory not found)

**Helper Function**: `end_cd_process(t_shell *shell, char *oldpwd, char *pwd)`
- Updates environment variables after successful directory change

### `ft_pwd(char **args, t_shell *shell)`
**Purpose**: Implements the `pwd` command (print working directory)
**Process**:
1. Call `getcwd()` to get current directory
2. Print directory path to stdout
3. Return 0 on success, 1 on error

### `ft_export_argv(char **argv, t_shell *shell)`
**Purpose**: Implements the `export` command (set environment variables)
**Modes**:
- No arguments: Display all exported variables
- With arguments: Set/modify environment variables

**Process for setting variables**:
1. Parse each argument for `KEY=VALUE` format
2. Validate variable name (alphanumeric + underscore, starts with letter)
3. Add/update in shell environment
4. Mark as exported

**Helper Functions**:
- `parse_export_arg(char *arg, char **key, char **value)`: Parse KEY=VALUE
- `print_export_list(t_env *env)`: Display exported variables
- `set_env(t_shell *shell, const char *key, const char *value)`: Set environment variable
- `new_env_node(const char *key, const char *value)`: Create new environment node
- `find_env(char *key, t_env *env)`: Find existing environment variable

**Export Display Format**: `declare -x KEY="VALUE"`

### `ft_unset_argv(char **argv, t_shell *shell)`
**Purpose**: Implements the `unset` command (remove environment variables)
**Process**:
1. Iterate through variable names in argv
2. Find each variable in environment list
3. Remove from environment
4. Free associated memory
5. Return 0 on success

### `print_env(char **args, t_shell *shell)`
**Purpose**: Implements the `env` command (display environment)
**Process**:
1. Iterate through shell environment list
2. Print each variable in `KEY=VALUE` format
3. Only print exported variables
4. Return 0 on success

### `ft_exit_argv(char **argv, t_shell *shell)`
**Purpose**: Implements the `exit` command
**Features**:
- No arguments: Exit with status 0
- Single numeric argument: Exit with that status
- Non-numeric argument: Print error, exit with status 2
- Multiple arguments: Print error, don't exit

**Process**:
1. Parse exit code from first argument
2. Validate numeric format
3. Handle edge cases (overflow, non-numeric)
4. Clean up shell resources
5. Call `exit()` with specified code

**Error Handling**:
- Invalid numbers: "numeric argument required"
- Too many arguments: "too many arguments"

## Environment Management

### Environment Structure (`t_env`)
```c
typedef struct s_env
{
    char         *key;      // Variable name
    char         *value;    // Variable value
    int          exported;  // Export flag
    struct s_env *next;     // Next in linked list
} t_env;
```

### Environment Functions

#### `set_env(t_shell *shell, const char *key, const char *value)`
**Purpose**: Set or update environment variable
**Process**:
1. Search for existing variable
2. If found: update value
3. If not found: create new node and add to list
4. Mark as exported if setting through export command

#### `find_env(char *key, t_env *env)`
**Purpose**: Find environment variable by key
**Returns**: Pointer to environment node or NULL

#### `new_env_node(const char *key, const char *value)`
**Purpose**: Create new environment variable node
**Process**:
1. Allocate memory for node
2. Duplicate key and value strings
3. Initialize exported flag
4. Return new node

## Utility Functions

### Memory Management

#### `cleanup(t_shell *shell)`
**Purpose**: Free all shell resources before exit
**Process**:
1. Free environment list
2. Free execution context
3. Free any remaining AST nodes
4. Clean up other allocated resources

#### `free_env(t_env *env)`
**Purpose**: Free environment variable list
**Process**:
1. Iterate through linked list
2. Free key and value strings
3. Free node structure
4. Continue to next node

#### `ft_free_array(char **array)`
**Purpose**: Free null-terminated string array
**Process**:
1. Iterate through array
2. Free each string
3. Free array pointer

### Error Handling

#### `shell_error_message(char *message)`
**Purpose**: Print error message to stderr
**Format**: `minishell: command: error message`
**Returns**: Error exit code (usually 1)

### Validation Functions

#### Variable Name Validation
- Must start with letter or underscore
- Can contain letters, numbers, underscores
- Case sensitive
- Cannot be empty

#### Numeric Validation (for exit command)
- Must be valid integer
- Handle overflow/underflow
- No leading/trailing non-numeric characters

## Command Behavior Details

### `cd` Command
- `cd` (no args) → Go to HOME directory
- `cd -` → Go to previous directory (OLDPWD)
- `cd /path` → Go to specified path
- Updates PWD and OLDPWD automatically
- Handles relative and absolute paths

### `export` Command
- `export` → List all exported variables
- `export VAR` → Export existing variable (mark for export)
- `export VAR=value` → Set and export variable
- Multiple variables: `export A=1 B=2 C=3`
- Invalid names cause error but continue processing

### `unset` Command
- `unset VAR1 VAR2` → Remove multiple variables
- Removes from environment completely
- No error if variable doesn't exist
- Cannot unset certain variables (implementation dependent)

### `exit` Command
- `exit` → Exit with status 0
- `exit 42` → Exit with status 42
- `exit abc` → Error, exit with status 2
- `exit 1 2` → Error, don't exit
- Status range: 0-255 (modulo 256)

## Integration Points
- **Execution Engine**: Called when builtin command detected
- **Environment System**: Modify shell environment variables
- **Error Handling**: Report errors to user appropriately
- **Shell State**: Can modify shell's internal state (PWD, environment)
- **Process Management**: Execute within shell process, not external process
