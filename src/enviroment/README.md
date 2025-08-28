# Environment Module - README

## Overview

The environment module manages shell environment variables, providing functionality to get, set, and manipulate the shell's environment. It maintains a linked list representation of environment variables and handles conversion between different formats (linked list and array) for compatibility with system calls.

## Core Components

### Environment Structure
The environment is represented using a linked list:
```c
typedef struct s_env {
    char *key;          // Variable name
    char *value;        // Variable value  
    struct s_env *next; // Next environment variable
} t_env;
```

### Key Features
- **Dynamic Management**: Add, modify, and remove variables at runtime
- **Format Conversion**: Convert between linked list and array formats
- **Memory Safety**: Proper allocation and deallocation of environment data
- **Shell Integration**: Seamless integration with built-in commands and expansion

## Key Functions

### Environment Initialization
- `get_env(char **env)` - Initialize environment from system environment array
- `create_env_from_string()` - Parse single environment string into node
- `init_env_node_values()` - Initialize environment node with key/value

### Environment Access
- `get_env_value(char *key, t_env *env)` - Retrieve value for given key
- `set_env_value(char *key, char *value, t_env *env)` - Set or update variable
- `find_env(char *key, t_env *env)` - Find environment node by key

### Format Conversion
- `env_list_to_array(t_env *env)` - Convert linked list to array format
- `count_env_with_values(t_env *env)` - Count variables with non-null values

### Memory Management
- `cleanup_env_node(t_env *node)` - Free single environment node
- `free_env(t_env *env)` - Free entire environment list

## Environment Initialization Process

### System Environment Import
1. **Array Processing**: Iterate through system environment array
2. **String Parsing**: Parse each `KEY=VALUE` string
3. **Node Creation**: Create linked list nodes for each variable
4. **List Building**: Build linked list by prepending nodes
5. **Error Handling**: Handle memory allocation failures gracefully

### Key-Value Parsing
The module handles various environment string formats:
- **Standard Format**: `KEY=VALUE`
- **Empty Values**: `KEY=` (empty string value)
- **No Equals**: `KEY` (value set to empty string)

## Environment Operations

### Variable Lookup
The `get_env_value()` function:
1. **Linear Search**: Traverse linked list to find matching key
2. **Exact Match**: Compare full key length for exact matches
3. **Value Return**: Return copy of value or NULL if not found
4. **Empty Handling**: Return empty string for variables with no value

### Variable Setting
Environment modification through various built-ins:
1. **Export Command**: Add or modify exported variables
2. **Assignment**: Direct variable assignment (in some contexts)
3. **Unset Command**: Remove variables from environment
4. **Value Updates**: Modify existing variable values

### Array Conversion
For system calls requiring environment arrays:
1. **Count Variables**: Count variables with non-null values
2. **Allocate Array**: Allocate array with extra slot for NULL terminator
3. **Format Strings**: Create `KEY=VALUE` strings for each variable
4. **NULL Termination**: Ensure proper array termination

## Special Handling and Edge Cases

### Variable Types
1. **Exported Variables**: Variables available to child processes
2. **Local Variables**: Shell-only variables (basic support)
3. **Special Variables**: `PWD`, `OLDPWD`, `PATH`, etc.
4. **Read-only Variables**: Some variables should not be modified

### Memory Management
1. **String Duplication**: All strings are dynamically allocated copies
2. **Node Cleanup**: Proper cleanup of key and value strings
3. **List Traversal**: Safe traversal and cleanup of linked list
4. **Allocation Failures**: Graceful handling of memory allocation failures

### Key Comparison
1. **Exact Matching**: Keys must match exactly in length and content
2. **Case Sensitivity**: Environment variables are case-sensitive
3. **NULL Handling**: Proper handling of NULL keys and values
4. **Empty Keys**: Rejection of invalid empty key names

### Value Handling
1. **Empty Values**: Distinction between empty string and NULL
2. **String Copying**: All values are copied to prevent external modification
3. **Value Updates**: Proper replacement of existing values
4. **NULL Values**: Variables can exist without values

## File Structure

- `enviroment.c` - Core environment management and initialization
- `enviroment_utils.c` - Environment utility functions
- `enviroment_utils_extra.c` - Additional environment utilities

## Integration with Shell Components

### Built-in Commands
- **export**: Uses environment module to add/modify variables
- **unset**: Uses environment module to remove variables  
- **env**: Uses environment module to display all variables
- **cd**: Updates `PWD` and `OLDPWD` environment variables

### Variable Expansion
- **Expander Integration**: Provides variable values for `$VAR` expansion
- **Special Variables**: Handles special parameters like `$?`, `$0`
- **Dynamic Lookup**: Real-time variable lookup during expansion

### External Commands
- **execve Integration**: Converts environment to array format for system calls
- **Child Processes**: Inherits environment variables to child processes
- **PATH Handling**: Uses `PATH` variable for command resolution

## Usage Examples

### Basic Operations
```c
// Initialize from system environment
t_env *env = get_env(environ);

// Get variable value
char *home = get_env_value("HOME", env);

// Convert to array for execve
char **env_array = env_list_to_array(env);
```

### Built-in Integration
```bash
export USER=john        # Adds USER to environment
echo $USER             # Expander uses environment module
unset USER             # Removes USER from environment
env                    # Displays all environment variables
```

## Error Handling

### Allocation Failures
- Returns NULL on memory allocation failures
- Cleanup of partial structures on failure
- Graceful degradation when possible

### Invalid Operations
- Rejection of invalid key names
- Proper handling of NULL inputs
- Safe operations on empty environment

The environment module provides a robust foundation for shell variable management, ensuring proper memory handling and seamless integration with other shell components.
