# Built-in Commands Module - README

## Overview

The built-in commands module implements essential shell built-in commands that must be executed within the shell process rather than as external programs. These commands have direct access to the shell's internal state and can modify the shell environment, working directory, and other shell properties.

## Implemented Built-in Commands

### Core Built-ins
- **echo** - Display arguments with optional formatting
- **cd** - Change current working directory
- **pwd** - Print current working directory
- **export** - Set and export environment variables
- **unset** - Remove environment variables
- **env** - Display environment variables
- **exit** - Exit the shell with optional status code

### Built-in Architecture
```c
typedef struct s_builtin {
    char *name;                              // Command name
    int (*func)(char **args, t_shell *shell); // Function pointer
} t_builtin;
```

## Key Functions

### Built-in Management
- `is_builtin_command(char *cmd)` - Check if command is built-in
- `execute_builtin(char **argv, t_shell *shell)` - Execute built-in command

### Individual Built-in Implementations
- `ft_echo_argv()` - Echo command implementation
- `ft_cd_argv()` - Change directory implementation  
- `ft_pwd()` - Print working directory implementation
- `ft_export_argv()` - Export command implementation
- `ft_unset_argv()` - Unset command implementation
- `print_env()` - Environment display implementation
- `ft_exit_argv()` - Exit command implementation

## Detailed Command Implementations

### Echo Command (`ft_echo_argv`)
**Functionality**: Display arguments separated by spaces
**Options**: 
- `-n`: Suppress trailing newline

**Special Handling**:
- Multiple `-n` flags are handled correctly
- Only `-n` as exact option (no bundling like `-nnn`)
- Proper argument parsing and output formatting

**Edge Cases**:
- Empty arguments produce empty output
- Multiple spaces between arguments collapsed to single space
- No arguments with `-n` produces no output

### Change Directory (`ft_cd_argv`)
**Functionality**: Change current working directory
**Behavior**:
- `cd` with no arguments goes to `HOME` directory
- `cd -` goes to previous directory (`OLDPWD`)
- `cd <path>` changes to specified path

**Environment Updates**:
- Updates `PWD` environment variable to new directory
- Updates `OLDPWD` environment variable to previous directory
- Handles both absolute and relative paths

**Error Handling**:
- Permission denied errors
- Directory not found errors
- Invalid path errors
- Missing `HOME` environment variable

### Print Working Directory (`ft_pwd`)
**Functionality**: Display current working directory
**Implementation**: Uses `getcwd()` system call
**Error Handling**: Handles cases where current directory is deleted

### Export Command (`ft_export_argv`)
**Functionality**: Set and export environment variables
**Syntax**: 
- `export` - Display all exported variables
- `export VAR` - Export existing variable
- `export VAR=VALUE` - Set and export variable

**Features**:
- Variable validation (valid identifier names)
- Value assignment parsing
- Display formatting for export list
- Handles variables without values

**Edge Cases**:
- Invalid variable names (starting with digit, containing special chars)
- Empty values vs no values
- Overwriting existing variables
- Export without value preserves existing value

### Unset Command (`ft_unset_argv`)
**Functionality**: Remove environment variables
**Syntax**: `unset VAR1 VAR2 ...`
**Implementation**: Removes variables from environment list
**Error Handling**: Ignores attempts to unset non-existent variables

### Environment Display (`print_env`)
**Functionality**: Display all environment variables with values
**Format**: `KEY=VALUE` format, one per line
**Filtering**: Only displays variables that have values

### Exit Command (`ft_exit_argv`)
**Functionality**: Exit shell with optional exit code
**Syntax**: 
- `exit` - Exit with status 0
- `exit N` - Exit with status N

**Features**:
- Numeric argument validation
- Exit status range validation (0-255)
- Proper cleanup before exit

**Edge Cases**:
- Non-numeric arguments produce error
- Multiple arguments (error in bash)
- Very large numbers handled appropriately

## Argument Processing

### Standard Argument Handling
All built-ins receive:
- `char **argv` - Argument array (including command name)
- `t_shell *shell` - Shell state structure

### Argument Validation
1. **Argument Count**: Check for required/optional arguments
2. **Argument Format**: Validate argument syntax
3. **Option Parsing**: Handle command-line options where applicable
4. **Error Reporting**: Provide meaningful error messages

## Environment Integration

### Environment Variable Access
Built-ins use the environment module for:
- Reading environment variables
- Setting new variables
- Removing variables
- Converting between formats

### Special Variables
Built-ins handle special shell variables:
- `PWD` - Current working directory
- `OLDPWD` - Previous working directory  
- `HOME` - User home directory
- `PATH` - Command search path

## Error Handling and Edge Cases

### Input Validation
1. **Invalid Options**: Reject unsupported options
2. **Argument Limits**: Handle too many/few arguments
3. **Invalid Names**: Reject invalid variable names
4. **Numeric Validation**: Validate numeric arguments

### System Errors
1. **Permission Errors**: Handle permission denied errors
2. **File System Errors**: Handle directory access errors
3. **Memory Errors**: Handle allocation failures gracefully
4. **System Limits**: Handle system resource limits

### Shell State Management
1. **Environment Consistency**: Maintain consistent environment state
2. **Working Directory**: Keep working directory synchronized
3. **Exit Status**: Properly set shell exit status
4. **Memory Cleanup**: Clean up allocated memory

## File Structure

- `builtin.c` - Built-in command dispatcher and management
- `ft_echo.c` - Echo command implementation
- `ft_cd.c` - Change directory implementation
- `ft_pwd.c` - Print working directory implementation
- `ft_export.c` - Export command core implementation
- `ft_export_utils.c` - Export command utilities
- `ft_export_print.c` - Export display formatting
- `ft_unset.c` - Unset command implementation
- `ft_exit.c` - Exit command implementation

## Integration with Shell

### Execution Integration
Built-ins are executed in the main shell process:
1. **Command Recognition**: Built-in detection during execution
2. **Direct Execution**: No fork/exec for built-ins
3. **State Modification**: Direct access to shell state
4. **Immediate Effect**: Changes take effect immediately

### Redirection Support
Built-ins work with I/O redirection:
- Output redirection for display commands
- Input redirection where applicable
- Proper file descriptor management
- Error output handling

### Pipeline Compatibility
Built-ins work correctly in pipelines:
- Proper stdin/stdout handling
- Exit status propagation
- Signal handling consistency

## Usage Examples

### Basic Usage
```bash
cd /home/user          # Change directory
pwd                    # Display current directory
export USER=john       # Set and export variable
echo $USER            # Display variable value
unset USER            # Remove variable
exit 0                # Exit with status 0
```

### Advanced Usage
```bash
cd -                  # Go to previous directory
export PATH=$PATH:/new/path    # Modify PATH
echo -n "prompt: "    # Echo without newline
export VAR            # Export existing variable
```

The built-in module provides essential shell functionality that cannot be implemented as external commands, ensuring proper shell behavior and environment management.
