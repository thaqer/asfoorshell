# Expander Module - README

## Overview

The expander module handles variable expansion, quote removal, and word splitting for shell tokens. It processes tokens after lexing but before parsing, transforming raw token values into their expanded forms while respecting shell quoting rules. The module handles complex cases like mixed quoting, parameter expansion, and field splitting.

## Core Components

### Expansion Types
The expander handles several types of expansion:
- **Parameter Expansion**: `$VAR`, `${VAR}`
- **Special Parameters**: `$?` (exit status), `$0` (program name)
- **Quote Removal**: Removes outer quotes after expansion
- **Field Splitting**: Splits expanded values into multiple tokens
- **Pathname Expansion**: (Not implemented - would be glob patterns)

### Quote Context Types
Different quote contexts affect expansion behavior:
- `SEGMENT_UNQUOTED` - Normal expansion and splitting
- `SEGMENT_SINGLE_QUOTED` - No expansion, preserve literally
- `SEGMENT_DOUBLE_QUOTED` - Parameter expansion but no splitting

### Expansion Control Flags
Quote flags from lexer control expansion behavior:
- `QF_ALL_SQ` - All single quoted: no expansion
- `QF_ALL_DQ` - All double quoted: expand but no split
- `QF_MIXED` - Mixed quoting: complex processing required
- `QF_EXPANDED` - Already processed flag

### Core Data Structures

#### Expander Credentials
```c
typedef struct s_expander_creds {
    int last_exit_status;    // For $? expansion
    t_env *envp;            // Environment variables
    char *program_name;      // For $0 expansion
} t_expander_creds;
```

#### Segment Structure
Used for mixed quote processing:
```c
typedef struct s_segment {
    char *value;            // Segment content
    t_segment_type type;    // Quote context
    struct s_segment *next; // Linked list
} t_segment;
```

## Key Functions

### Main Entry Points
- `expand_tokens()` - Main function that expands entire token list
- `dquote_expander()` - Handles double-quoted expansions
- `unquoted_variable_handler()` - Processes unquoted parameter expansion

### Core Expansion Logic
- `expansion_loop()` - Main expansion processing loop
- `process_dollar_expansion()` - Handles $VAR expansion
- `handle_dollar_type()` - Dispatches different $ parameter types
- `get_env_var_value()` - Retrieves environment variable values
- `expand_env_variable()` - Expands single environment variable

### Mixed Quote Processing  
- `parse_mixed_segments()` - Parses mixed quoted/unquoted segments
- `process_segments_with_splitting()` - Handles segment processing with field splitting
- `process_single_segment()` - Processes individual segments
- `splice_mixed_expansion()` - Inserts expanded tokens into token list

### Utility Functions
- `strip_quotes()` - Removes outer quotes from strings
- `param_determinant()` - Identifies parameter types ($, alpha, numeric)
- `word_all_single()`, `word_all_double()` - Quote type checking
- `allow_param_expansion()` - Determines if expansion should occur

## Expansion Process

### Simple Cases
1. **All Single Quoted**: No expansion, just quote removal
2. **All Double Quoted**: Parameter expansion only, no field splitting
3. **Unquoted**: Full expansion and field splitting

### Complex Mixed Expansion
For tokens with mixed quoting like `"hello"$USER'world'`:

1. **Segment Parsing**: Break into segments by quote boundaries
2. **Individual Processing**: Process each segment according to its quote type
3. **Result Accumulation**: Combine processed segments  
4. **Field Splitting**: Split result if needed
5. **Token Replacement**: Replace original with expanded tokens

### Parameter Expansion Details

#### Variable Types
- **Regular Variables**: `$VAR` → environment variable value
- **Exit Status**: `$?` → last command exit status  
- **Program Name**: `$0` → shell program name
- **Numeric Parameters**: `$1`, `$2`, etc. → Not implemented

#### Expansion Rules
- Inside single quotes: No expansion
- Inside double quotes: Expand but preserve as single field
- Unquoted: Expand and split on whitespace

## Edge Cases and Special Handling

### Empty Expansions
1. **Undefined Variables**: `$NONEXISTENT` → empty string
2. **Empty Values**: `$EMPTY` where EMPTY="" → empty string
3. **Quote Preservation**: `"$EMPTY"` → empty token preserved

### Quote Interaction
1. **Nested Quotes**: `"don't expand $USER"` → literal dollar
2. **Adjacent Segments**: `$USER"@host"` → expanded user + literal @host
3. **Escaped Characters**: Basic escape handling in double quotes

### Field Splitting
1. **IFS Handling**: Uses space/tab/newline for splitting (simplified)
2. **Empty Fields**: Consecutive delimiters create empty fields
3. **Leading/Trailing**: Whitespace trimming behavior

### Memory Management
1. **Incremental Building**: Builds result string incrementally
2. **Segment Cleanup**: Frees temporary segment structures  
3. **Token List Updates**: Properly maintains token list integrity
4. **Error Recovery**: Cleans up on allocation failures

## File Structure

- `dispatcher_expander.c` - Main expansion dispatch logic
- `core_expand.c` - Core expansion processing
- `core_expand_utils.c` - Core expansion utilities
- `core_expand_helpers.c` - Helper functions for core expansion
- `mixed_expand.c` - Mixed quote expansion handling
- `parse_segments.c` - Quote segment parsing
- `parse_segments_utils.c` - Segment parsing utilities
- `parse_segments_more_utils.c` - Additional segment utilities
- `quote_processing.c` - Quote processing and removal
- `segment_processing.c` - Individual segment processing
- `segment_utils.c` - Segment management utilities
- `token_utils.c` - Token manipulation utilities
- `type_utils.c` - Type checking and classification
- `expand.c` - High-level expansion interface
- `expander_utils.c` - General expansion utilities

## Usage Examples

### Simple Variable Expansion
```bash
export USER=john
echo $USER          # → ["echo", "john"]
echo "$USER"        # → ["echo", "john"] 
echo '$USER'        # → ["echo", "$USER"]
```

### Mixed Quoting
```bash
echo "Hello "$USER" world"  # → ["echo", "Hello john world"]
echo $USER"@host"           # → ["echo", "john@host"]
```

### Field Splitting
```bash
export FILES="a.txt b.txt c.txt"
echo $FILES         # → ["echo", "a.txt", "b.txt", "c.txt"]
echo "$FILES"       # → ["echo", "a.txt b.txt c.txt"]
```

## Integration

The expander operates between the lexer and parser:
1. **Input**: Token list from lexer with quote flags
2. **Processing**: Expands variables and handles quotes
3. **Output**: Modified token list with expanded values
4. **Integration**: Parser receives expanded tokens for AST construction

The module is crucial for proper shell behavior and handles the complex interaction between quoting, variable expansion, and word splitting that defines shell semantics.
