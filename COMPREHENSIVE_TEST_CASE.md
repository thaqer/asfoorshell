# Comprehensive Test Case - Complex Command Execution Trace

## Test Command
```bash
export VAR="hello world" && echo "Start: $VAR" | cat > output.txt && (ls -la | grep ".txt" || echo "No files") && cat << 'EOF' >> output.txt && echo 'Done!' 
This is a heredoc
with $VAR expansion
EOF
```

## Command Breakdown
This complex command demonstrates:
- **Built-in command**: `export`
- **Variable assignment**: `VAR="hello world"`
- **Logical AND**: `&&`
- **Variable expansion**: `$VAR`
- **Double quotes**: `"Start: $VAR"`
- **Pipeline**: `|`
- **Output redirection**: `>`
- **Subshell**: `(ls -la | grep ".txt" || echo "No files")`
- **Logical OR**: `||`
- **Heredoc**: `<< 'EOF'`
- **Append redirection**: `>>`
- **Single quotes**: `'EOF'` and `'Done!'`
- **Mixed content**: Variables, literals, quotes

## Phase-by-Phase Execution Trace

### Phase 1: Lexer (Tokenization)

#### Input to Lexer:
```
export VAR="hello world" && echo "Start: $VAR" | cat > output.txt && (ls -la | grep ".txt" || echo "No files") && cat << 'EOF' >> output.txt && echo 'Done!'
```

#### Lexer Processing:

**State Machine Transitions:**
1. `STATE_NORMAL` → reads "export" → `TOKEN_WORD`
2. `STATE_NORMAL` → reads "VAR=" → `TOKEN_WORD` 
3. `STATE_NORMAL` → encounters `"` → `STATE_DQUOTE`
4. `STATE_DQUOTE` → reads "hello world" → continues in quotes
5. `STATE_DQUOTE` → encounters `"` → `STATE_NORMAL`
6. `STATE_NORMAL` → reads "&&" → `TOKEN_AND`
7. ... (continues for entire command)

#### Lexer Output (Token Stream):
```c
Token[0]:  {value: "export",        type: TOKEN_WORD,        qflags: QF_NONE}
Token[1]:  {value: "VAR=hello world", type: TOKEN_WORD,      qflags: QF_HAS_DQ}
Token[2]:  {value: "&&",            type: TOKEN_AND,         qflags: QF_NONE}
Token[3]:  {value: "echo",          type: TOKEN_WORD,        qflags: QF_NONE}
Token[4]:  {value: "Start: $VAR",   type: TOKEN_WORD,        qflags: QF_HAS_DQ}
Token[5]:  {value: "|",             type: TOKEN_PIPE,        qflags: QF_NONE}
Token[6]:  {value: "cat",           type: TOKEN_WORD,        qflags: QF_NONE}
Token[7]:  {value: ">",             type: TOKEN_REDIRECT_OUT, qflags: QF_NONE}
Token[8]:  {value: "output.txt",    type: TOKEN_WORD,        qflags: QF_NONE}
Token[9]:  {value: "&&",            type: TOKEN_AND,         qflags: QF_NONE}
Token[10]: {value: "(",             type: TOKEN_OPEN_PAREN,  qflags: QF_NONE}
Token[11]: {value: "ls",            type: TOKEN_WORD,        qflags: QF_NONE}
Token[12]: {value: "-la",           type: TOKEN_WORD,        qflags: QF_NONE}
Token[13]: {value: "|",             type: TOKEN_PIPE,        qflags: QF_NONE}
Token[14]: {value: "grep",          type: TOKEN_WORD,        qflags: QF_NONE}
Token[15]: {value: ".txt",          type: TOKEN_WORD,        qflags: QF_HAS_DQ}
Token[16]: {value: "||",            type: TOKEN_OR,          qflags: QF_NONE}
Token[17]: {value: "echo",          type: TOKEN_WORD,        qflags: QF_NONE}
Token[18]: {value: "No files",      type: TOKEN_WORD,        qflags: QF_HAS_DQ}
Token[19]: {value: ")",             type: TOKEN_CLOSE_PAREN, qflags: QF_NONE}
Token[20]: {value: "&&",            type: TOKEN_AND,         qflags: QF_NONE}
Token[21]: {value: "cat",           type: TOKEN_WORD,        qflags: QF_NONE}
Token[22]: {value: "<<",            type: TOKEN_HEREDOC,     qflags: QF_NONE}
Token[23]: {value: "EOF",           type: TOKEN_WORD,        qflags: QF_HAS_SQ | QF_HD_QUOTED}
Token[24]: {value: ">>",            type: TOKEN_APPEND,      qflags: QF_NONE}
Token[25]: {value: "output.txt",    type: TOKEN_WORD,        qflags: QF_NONE}
Token[26]: {value: "&&",            type: TOKEN_AND,         qflags: QF_NONE}
Token[27]: {value: "echo",          type: TOKEN_WORD,        qflags: QF_NONE}
Token[28]: {value: "Done!",         type: TOKEN_WORD,        qflags: QF_HAS_SQ}
```

**Special Lexer Handling:**
- **Heredoc delimiter**: `'EOF'` marked with `QF_HD_QUOTED` flag
- **Quote flags**: Properly set for expansion control
- **Multi-character operators**: `&&`, `||`, `<<`, `>>` recognized as single tokens

### Phase 2: Expander (Variable Expansion)

#### Input to Expander:
Token stream from lexer with quote flags

#### Expansion Processing:

**Token[1]: "VAR=hello world"**
- Quote flags: `QF_HAS_DQ`
- Processing: Double quoted content, remove outer quotes
- Result: `"VAR=hello world"` → `VAR=hello world`

**Token[4]: "Start: $VAR"** 
- Quote flags: `QF_HAS_DQ`
- Processing: Double quoted with variable expansion
- Variable lookup: `$VAR` → not yet set, expands to empty
- Result: `"Start: $VAR"` → `Start: `

**Token[15]: ".txt"**
- Quote flags: `QF_HAS_DQ` 
- Processing: Remove outer quotes
- Result: `".txt"` → `.txt`

**Token[18]: "No files"**
- Quote flags: `QF_HAS_DQ`
- Processing: Remove outer quotes  
- Result: `"No files"` → `No files`

**Token[28]: "Done!"**
- Quote flags: `QF_HAS_SQ`
- Processing: Single quoted, no expansion, remove quotes
- Result: `'Done!'` → `Done!`

#### Expander Output:
```c
Token[0]:  {value: "export",        type: TOKEN_WORD,        qflags: QF_EXPANDED}
Token[1]:  {value: "VAR=hello world", type: TOKEN_WORD,      qflags: QF_EXPANDED}
Token[2]:  {value: "&&",            type: TOKEN_AND,         qflags: QF_NONE}
Token[3]:  {value: "echo",          type: TOKEN_WORD,        qflags: QF_EXPANDED}
Token[4]:  {value: "Start: ",       type: TOKEN_WORD,        qflags: QF_EXPANDED}
// ... (other tokens processed similarly)
Token[28]: {value: "Done!",         type: TOKEN_WORD,        qflags: QF_EXPANDED}
```

### Phase 3: Parser (AST Construction)

#### Input to Parser:
Expanded token stream

#### Parsing Strategy:
```
parse_input_line()
  └─ parse_logical_line()  // Handle && operators
      ├─ parse_pipeline()  // Handle first command group
      ├─ parse_pipeline()  // Handle second command group  
      ├─ parse_pipeline()  // Handle third command group (subshell)
      ├─ parse_pipeline()  // Handle fourth command group (heredoc)
      └─ parse_pipeline()  // Handle fifth command group
```

#### AST Structure:
```
AST_LOGICAL_AND
├─ left: AST_LOGICAL_AND
│   ├─ left: AST_LOGICAL_AND  
│   │   ├─ left: AST_LOGICAL_AND
│   │   │   ├─ left: AST_SIMPLE_COMMAND ["export", "VAR=hello world"]
│   │   │   └─ right: AST_PIPELINE
│   │   │       ├─ left: AST_SIMPLE_COMMAND ["echo", "Start: "]
│   │   │       └─ right: AST_SIMPLE_COMMAND ["cat"] 
│   │   │                   └─ redirections: [REDIR_OUT: "output.txt"]
│   │   └─ right: AST_SUBSHELL
│   │       └─ child: AST_LOGICAL_OR
│   │           ├─ left: AST_PIPELINE
│   │           │   ├─ left: AST_SIMPLE_COMMAND ["ls", "-la"]
│   │           │   └─ right: AST_SIMPLE_COMMAND ["grep", ".txt"]
│   │           └─ right: AST_SIMPLE_COMMAND ["echo", "No files"]
│   └─ right: AST_SIMPLE_COMMAND ["cat"]
│       └─ redirections: [REDIR_HEREDOC: "EOF"], [REDIR_APPEND: "output.txt"]
└─ right: AST_SIMPLE_COMMAND ["echo", "Done!"]
```

**Parser Special Handling:**
- **Operator precedence**: Handles && and || correctly
- **Subshell grouping**: Parentheses create subshell nodes
- **Redirection parsing**: Attaches redirections to appropriate commands
- **Heredoc handling**: Marks heredoc delimiter as quoted/unquoted

### Phase 4: Execution Engine

#### Signal Setup:
```c
setup_signals(SIGNAL_PARENT);  // Main shell context
```

#### Execution Flow:

**Step 1: Preprocess Heredocs**
```c
preprocess_all_heredocs(ast_root, shell);
```
- **Setup signals**: `SIGNAL_HEREDOC` mode
- **Read heredoc content**:
  ```
  This is a heredoc
  with $VAR expansion
  ```
- **No expansion** (delimiter was quoted: `'EOF'`)
- **Create temporary file** with content
- **Signal handling**: SIGINT would terminate heredoc

**Step 2: Execute AST** 
```c
execute_ast(ast_root, shell);
```

##### Command 1: `export VAR="hello world"`
```c
execute_simple_command() → execute_builtin()
```
- **Built-in detection**: `is_builtin_command("export")` → true
- **Execution**: `ft_export_argv(["export", "VAR=hello world"], shell)`
- **Environment update**: Add VAR="hello world" to shell->env
- **Exit status**: 0

##### Command 2: `echo "Start: " | cat > output.txt`
```c
execute_pipeline()
```
- **Create pipe**: `pipe(pipefd)`
- **Fork left child**: 
  ```c
  setup_signals(SIGNAL_CHILD);
  // stdout → pipe write end
  execute_simple_command() → execute_builtin()
  // echo outputs: "Start: hello world" (VAR now expanded)
  ```
- **Fork right child**:
  ```c  
  setup_signals(SIGNAL_CHILD);
  // stdin ← pipe read end
  // stdout → output.txt (redirection)
  execute_external_command(["cat"])
  ```
- **Parent waits**: `waitpid()` for both children
- **Exit status**: 0

##### Command 3: Subshell `(ls -la | grep ".txt" || echo "No files")`
```c
execute_subshell()
```
- **Fork subshell**:
  ```c
  setup_signals(SIGNAL_CHILD);
  execute_ast() // Execute inner logical OR
  ```
- **Inner execution**:
  ```c
  execute_logical_or()
  // Execute: ls -la | grep ".txt" 
  // If pipeline fails, execute: echo "No files"
  ```
- **Exit status**: Depends on subshell commands

##### Command 4: `cat << 'EOF' >> output.txt`
```c
execute_simple_command()
```
- **Apply redirections**:
  - **Heredoc**: stdin ← temporary heredoc file
  - **Append**: stdout → output.txt (append mode)
- **Execute**: `execute_external_command(["cat"])`
- **Output**: Heredoc content appended to output.txt
- **Cleanup**: Remove temporary heredoc file

##### Command 5: `echo 'Done!'`
```c
execute_simple_command() → execute_builtin()
```
- **Built-in execution**: `ft_echo_argv(["echo", "Done!"], shell)`
- **Output**: "Done!"
- **Exit status**: 0

### Phase 5: File I/O and Redirections

#### Redirection Processing:

**Output Redirection**: `cat > output.txt`
```c
handle_redirections()
// Save stdin/stdout
int saved_stdout = dup(STDOUT_FILENO);
// Open output file
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// Redirect stdout
dup2(fd, STDOUT_FILENO);
close(fd);
```

**Append Redirection**: `cat >> output.txt`
```c
// Open in append mode
int fd = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
dup2(fd, STDOUT_FILENO);
```

**Heredoc Processing**: `<< 'EOF'`
```c
// Create temporary file during preprocessing
int heredoc_fd = open(temp_file, O_RDONLY);
dup2(heredoc_fd, STDIN_FILENO);
```

### Phase 6: Memory Management and Cleanup

#### Cleanup Process:
```c
cleanup_line(shell, expander_creds, line);
```
- **Free token list**: `free_tok_list(shell->tokens)`
- **Free AST**: `free_ast(shell->ast_root)`
- **Free input line**: `free(line)`
- **Close file descriptors**: `close_unused_fds()`
- **Update exit status**: For `$?` expansion

### Expected Final Output

#### Files Created:
**output.txt** content:
```
Start: hello world
This is a heredoc
with $VAR expansion
```

#### Terminal Output:
```
asfoorshell🐥:~ export VAR="hello world" && echo "Start: $VAR" | cat > output.txt && (ls -la | grep ".txt" || echo "No files") && cat << 'EOF' >> output.txt && echo 'Done!'
> This is a heredoc  
> with $VAR expansion
> EOF
output.txt
Done!
asfoorshell🐥:~ 
```

#### Shell State Changes:
- **Environment**: VAR="hello world" added
- **Exit status**: 0 (success)
- **Files**: output.txt created with combined content

## Execution Flow Chart

```
[Input Command]
        ↓
   [TOKENIZER]
        ↓
   [Token Stream] → [EXPANDER] → [Expanded Tokens]
        ↓
     [PARSER]
        ↓
   [AST Structure]
        ↓
[HEREDOC PREPROCESSING] → Setup SIGNAL_HEREDOC → Read heredoc content
        ↓
   [AST EXECUTOR] → Setup SIGNAL_PARENT
        ↓
┌─────────────────────────────────────────────────────┐
│                LOGICAL AND Chain                     │
├─ [export VAR="hello world"] → Built-in executor    │
├─ [echo | cat > output.txt] → Pipeline executor     │  
├─ [(subshell)] → Subshell executor                  │
├─ [cat << EOF >> output.txt] → Simple command       │
└─ [echo 'Done!'] → Built-in executor               │
└─────────────────────────────────────────────────────┘
        ↓
    [CLEANUP] → Free tokens, AST, restore FDs
        ↓
   [Shell Ready for Next Command]
```

## Signal Handling During Execution

### Signal Contexts:
1. **SIGNAL_PARENT**: Main shell loop
2. **SIGNAL_HEREDOC**: During heredoc input (special SIGINT handling)
3. **SIGNAL_CHILD**: In child processes (natural signal handling)
4. **SIGNAL_NONE**: While waiting for children (minimal interference)

### Signal Flow:
```
Main Loop (SIGNAL_PARENT) 
    ↓ 
Heredoc Input (SIGNAL_HEREDOC) → SIGINT closes stdin, terminates heredoc
    ↓
Command Execution (SIGNAL_CHILD) → Natural signal handling in children
    ↓
Parent Waiting (SIGNAL_NONE) → No readline interference
    ↓
Return to Main Loop (SIGNAL_PARENT)
```

This comprehensive test case demonstrates every major feature of your minishell and shows how the complex interaction between modules produces the correct shell behavior.
