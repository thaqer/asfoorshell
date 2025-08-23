# Expander Module

## Overview
The expander module processes tokens after lexing but before parsing, handling variable expansion, quote removal, parameter substitution, and glob expansion. It transforms tokens with shell-specific expansions into their final values that will be used during execution.

## Core Data Structures

### Expander Credentials (`t_expander_creds`)
```c
typedef struct s_expander_creds
{
    int     last_exit_status;   // $? value
    t_env   *envp;              // Environment variables
    char    *program_name;      // $0 value
} t_expander_creds;
```

### Parameter Types (`t_params`)
```c
typedef enum e_params
{
    UNDERSCORE,    // $_
    ALPHA,         // $VAR_NAME
    NUMERIC,       // $0, $1, etc.
    Q_MARK,        // $?
    OTHER,         // Other characters
} t_params;
```

### Segment Structure (`t_segment`)
```c
typedef struct s_segment
{
    char                *value;  // Segment content
    t_segment_type      type;    // Quote type
    struct s_segment    *next;   // Next segment
} t_segment;
```

### Segment Types (`t_segment_type`)
- `SEGMENT_UNQUOTED` - No quotes, full expansion
- `SEGMENT_SINGLE_QUOTED` - Single quotes, no expansion
- `SEGMENT_DOUBLE_QUOTED` - Double quotes, variable expansion only

## Main Functions

### `expand_tokens(t_token **head, t_expander_creds *expander_creds)`
**Purpose**: Main entry point for token expansion
**Process**:
1. Iterate through token list
2. Skip non-WORD tokens and heredoc delimiters
3. Apply appropriate expansion based on quote flags
4. Update token values with expanded content
**Returns**: 0 on success, non-zero on error

### Expansion Type Dispatchers

#### `expand_unquoted_token(t_token *token, t_expander_creds *creds)`
**Purpose**: Full expansion for unquoted tokens
**Expansions Applied**:
- Variable expansion (`$VAR`)
- Parameter expansion (`$?`, `$0`)
- Quote removal
- Glob expansion (if enabled)

#### `expand_quoted_token(t_token *token, t_expander_creds *creds)`
**Purpose**: Limited expansion for quoted tokens
**Single Quotes**: No expansion, only quote removal
**Double Quotes**: Variable expansion only, then quote removal

#### `expand_mixed_token(t_token *token, t_expander_creds *creds)`
**Purpose**: Complex expansion for mixed quote patterns
**Process**:
1. Parse into segments by quote boundaries
2. Apply appropriate expansion per segment
3. Concatenate results
4. Handle quote removal

## Core Expansion Functions

### Variable Expansion

#### `expand_env_variable(char *output, char *var_start, t_env *envp, char **end_pos)`
**Purpose**: Expands environment variables (`$VAR`, `${VAR}`)
**Process**:
1. Extract variable name
2. Look up in environment
3. Append value to output string
4. Update position pointer

#### `get_env_var_value(char *var_start, t_env *envp, char **end_pos)`
**Purpose**: Retrieves environment variable value
**Process**:
1. Extract variable name (alphanumeric + underscore)
2. Handle braced variables `${VAR}`
3. Search environment list
4. Return value or empty string

#### `extract_variable_name(char *start, char **end_pos)`
**Purpose**: Extracts variable name from expansion
**Rules**:
- Start with letter or underscore
- Continue with alphanumeric or underscore
- Handle braced format `${...}`

### Parameter Expansion

#### `expand_exit_status(char *output, int status)`
**Purpose**: Expands `$?` to last exit status
**Process**: Convert integer to string and append

#### `expand_numeric_value(char *output, char **pos, char *program_name)`
**Purpose**: Expands `$0`, `$1`, etc.
**Implementation**:
- `$0` → program name
- `$1`, `$2`, etc. → empty (positional params not implemented)

#### `handle_dollar_type(char *output, t_params type, char **pos, t_expander_creds *expander_creds)`
**Purpose**: Dispatcher for different `$` expansions
**Process**:
1. Determine parameter type
2. Call appropriate expansion function
3. Update output and position

### Quote Processing

#### `strip_quotes(char *str)`
**Purpose**: Removes surrounding quotes from string
**Process**:
1. Identify quote type (single/double)
2. Check if entirely quoted
3. Remove outer quotes while preserving content

#### Quote Flag Analysis Functions
- `word_all_single(unsigned int f)` - Check if entirely single-quoted
- `word_all_double(unsigned int f)` - Check if entirely double-quoted
- `word_has_quotes(unsigned int f)` - Check if contains any quotes
- `word_mixed(unsigned int f)` - Check if mixed quoting
- `word_unquoted(unsigned int f)` - Check if no quotes

### Expansion Control

#### `allow_param_expansion(unsigned int f)`
**Purpose**: Determines if parameter expansion is allowed
**Rules**:
- Allowed in unquoted context
- Allowed in double quotes
- NOT allowed in single quotes

#### `allow_glob(unsigned int f)`
**Purpose**: Determines if glob expansion is allowed
**Rules**:
- Only allowed in completely unquoted tokens
- Disabled in any quoted context

## Utility Functions

### String Manipulation

#### `append_string_to_output(char *output, char *str)`
**Purpose**: Safely concatenates string to growing output
**Process**:
1. Reallocate output buffer if needed
2. Append new string
3. Return new output pointer

#### `append_literal_segment(char *output, char *start, size_t len)`
**Purpose**: Appends literal characters without expansion
**Usage**: For single-quoted segments and escaped characters

### Type Detection

#### `param_determinant(char c)`
**Purpose**: Classifies characters for parameter expansion
**Returns**:
- `ALPHA` for letters and underscore
- `NUMERIC` for digits
- `Q_MARK` for `?`
- `OTHER` for everything else

## Processing Flow

### Token-Level Flow
1. **Token Iteration**: Process each WORD token
2. **Flag Analysis**: Check quote flags from lexer
3. **Expansion Strategy**: Choose expansion method
4. **Apply Expansions**: Variable, parameter, quote removal
5. **Update Token**: Replace value with expanded result

### Character-Level Flow (for complex tokens)
1. **Character Scan**: Iterate through token value
2. **State Tracking**: Track quote context
3. **Expansion Detection**: Find `$` characters
4. **Context Check**: Verify expansion is allowed
5. **Apply Expansion**: Replace with expanded value
6. **Continue Scan**: Process remaining characters

### Segment-Based Flow (for mixed tokens)
1. **Segment Parsing**: Split by quote boundaries
2. **Type Assignment**: Classify each segment
3. **Individual Expansion**: Process per segment rules
4. **Concatenation**: Combine expanded segments
5. **Final Processing**: Quote removal, glob expansion

## Expansion Rules

### Variable Expansion (`$VAR`)
- **Unquoted**: Full expansion + glob if applicable
- **Double-quoted**: Expansion only
- **Single-quoted**: No expansion (literal `$VAR`)

### Special Parameters
- `$?` - Last exit status (always expanded in unquoted/double-quoted)
- `$0` - Program name
- `$_` - Last argument (not implemented)
- `$1`, `$2`, ... - Positional parameters (not implemented)

### Quote Removal
- **After expansion**: Quotes removed from expanded tokens
- **Preserves content**: Inner quotes and escapes handled correctly
- **Context aware**: Different rules for different quote types

## Error Handling
- **Memory allocation**: Check malloc failures during expansion
- **Invalid variables**: Undefined variables expand to empty string
- **Malformed syntax**: `${}`, incomplete expansions handled gracefully

## Usage Example

```c
// Input token: "Hello $USER, status: $?"
// Environment: USER=john
// Exit status: 0

t_expander_creds creds;
creds.last_exit_status = 0;
creds.envp = env_list;

expand_tokens(&token_list, &creds);

// Result: "Hello john, status: 0"
```

## Integration Points
- **Input**: Token list from lexer with quote flags
- **Output**: Expanded tokens ready for parsing
- **Environment**: Access to shell environment variables
- **State**: Last exit status for `$?` expansion
