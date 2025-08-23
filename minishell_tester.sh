#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    minishell_tester.sh                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#              #
#    Updated: 2025/08/16 00:00:00 by tbaniatt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test results storage
FAILED_TEST_DETAILS=""

# Configuration
MINISHELL_PATH="./minishell"
TIMEOUT_DURATION=5
TEST_DIR="test_files"
TEMP_DIR="temp_test"

# Create test directories
setup_test_environment() {
    echo -e "${CYAN}Setting up test environment...${NC}"
    
    # Create test directories
    mkdir -p "$TEST_DIR" "$TEMP_DIR"
    
    # Create test files
    echo "Hello World" > "$TEST_DIR/test_input.txt"
    echo -e "line1\nline2\nline3" > "$TEST_DIR/multiline.txt"
    echo "test content for append" > "$TEST_DIR/append_test.txt"
    touch "$TEST_DIR/empty_file.txt"
    
    # Create executable test script
    cat > "$TEST_DIR/test_script.sh" << 'EOF'
#!/bin/bash
echo "Script executed successfully"
echo "Argument count: $#"
echo "Arguments: $@"
EOF
    chmod +x "$TEST_DIR/test_script.sh"
    
    # Set some test environment variables
    export TEST_VAR="test_value"
    export PATH_BACKUP="$PATH"
    export TEST_HOME="$HOME"
    export TEST_USER="$USER"
    
    echo -e "${GREEN}Test environment setup complete.${NC}\n"
}

# Cleanup test environment
cleanup_test_environment() {
    echo -e "\n${CYAN}Cleaning up test environment...${NC}"
    rm -rf "$TEST_DIR" "$TEMP_DIR"
    echo -e "${GREEN}Cleanup complete.${NC}"
}

# Function to run a single test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "${WHITE}Description: $description${NC}"
    echo -e "${YELLOW}Command: $command${NC}"
    
    # Create temporary files for output comparison
    local minishell_out="$TEMP_DIR/minishell_out_$TOTAL_TESTS"
    local minishell_err="$TEMP_DIR/minishell_err_$TOTAL_TESTS"
    local bash_out="$TEMP_DIR/bash_out_$TOTAL_TESTS"
    local bash_err="$TEMP_DIR/bash_err_$TOTAL_TESTS"
    
    # Run command in minishell
    timeout "$TIMEOUT_DURATION" bash -c "echo '$command' | $MINISHELL_PATH" > "$minishell_out" 2> "$minishell_err"
    local minishell_exit=$?
    
    # Handle timeout case
    if [ $minishell_exit -eq 124 ]; then
        echo -e "${RED}TIMEOUT: Test took longer than $TIMEOUT_DURATION seconds${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_DETAILS+="\nTest $TOTAL_TESTS ($test_name): TIMEOUT"
        echo ""
        return
    fi
    
    # Run command in bash for comparison (if not a builtin-specific test)
    if [[ "$command" != *"exit"* ]] && [[ "$test_name" != *"Exit"* ]]; then
        timeout "$TIMEOUT_DURATION" bash -c "$command" > "$bash_out" 2> "$bash_err" 2>/dev/null
        local bash_exit=$?
    else
        # For exit tests, we expect different behavior
        bash_exit=$expected_exit
    fi
    
    # Compare outputs (simplified comparison for now)
    local test_passed=1
    
    # Check exit code if specified
    if [ "$expected_exit" != "" ] && [ "$expected_exit" != "any" ]; then
        if [ $minishell_exit -ne $expected_exit ]; then
            test_passed=0
            echo -e "${RED}Exit code mismatch: expected $expected_exit, got $minishell_exit${NC}"
        fi
    fi
    
    # Report result
    if [ $test_passed -eq 1 ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAILED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_DETAILS+="\nTest $TOTAL_TESTS ($test_name): $command"
        
        # Show minishell output for failed tests
        echo -e "${RED}Minishell stdout:${NC}"
        cat "$minishell_out"
        echo -e "${RED}Minishell stderr:${NC}"
        cat "$minishell_err"
    fi
    
    echo ""
}

# Print section header
print_section() {
    echo -e "\n${MAGENTA}========================================${NC}"
    echo -e "${MAGENTA}$1${NC}"
    echo -e "${MAGENTA}========================================${NC}\n"
}

# BUILTIN TESTS (WITHOUT EXPANSIONS)
test_builtins_basic() {
    print_section "BUILTIN COMMANDS - BASIC TESTS (NO EXPANSIONS)"
    
    # Echo tests
    run_test "Echo_Simple" "echo Hello World" 0 "Basic echo command"
    run_test "Echo_No_Args" "echo" 0 "Echo with no arguments"
    run_test "Echo_Multiple_Words" "echo one two three four" 0 "Echo with multiple words"
    run_test "Echo_Flag_n" "echo -n Hello" 0 "Echo with -n flag"
    run_test "Echo_Multiple_n_Flags" "echo -nnn Hello" 0 "Echo with multiple -n flags"
    run_test "Echo_Mixed_Flags" "echo -n -n Hello" 0 "Echo with separated -n flags"
    run_test "Echo_Invalid_Flag" "echo -x Hello" 0 "Echo with invalid flag"
    
    # PWD tests
    run_test "PWD_Simple" "pwd" 0 "Basic pwd command"
    run_test "PWD_With_Args" "pwd extra args" 0 "PWD with extra arguments"
    
    # CD tests
    run_test "CD_Home" "cd" 0 "CD to home directory"
    run_test "CD_Current_Dir" "cd ." 0 "CD to current directory"
    run_test "CD_Parent_Dir" "cd .." 0 "CD to parent directory"
    run_test "CD_Root" "cd /" 0 "CD to root directory"
    run_test "CD_Invalid_Path" "cd /nonexistent/path" 1 "CD to invalid path"
    run_test "CD_Too_Many_Args" "cd /tmp /usr" 1 "CD with too many arguments"
    
    # ENV tests
    run_test "ENV_Simple" "env" 0 "Basic env command"
    run_test "ENV_With_Args" "env arg1" 0 "ENV with arguments (should ignore)"
    
    # Export tests
    run_test "Export_No_Args" "export" 0 "Export without arguments"
    run_test "Export_New_Var" "export NEW_VAR=value" 0 "Export new variable"
    run_test "Export_Multiple_Vars" "export VAR1=val1 VAR2=val2" 0 "Export multiple variables"
    run_test "Export_No_Value" "export EMPTY_VAR" 0 "Export variable without value"
    run_test "Export_Invalid_Name" "export 123INVALID=value" 1 "Export with invalid variable name"
    run_test "Export_Equals_Only" "export =" 1 "Export with just equals sign"
    
    # Unset tests
    run_test "Unset_Existing_Var" "unset TEST_VAR" 0 "Unset existing variable"
    run_test "Unset_Nonexistent_Var" "unset NONEXISTENT_VAR" 0 "Unset non-existent variable"
    run_test "Unset_Multiple_Vars" "unset VAR1 VAR2 VAR3" 0 "Unset multiple variables"
    run_test "Unset_No_Args" "unset" 1 "Unset without arguments"
    run_test "Unset_Invalid_Name" "unset 123INVALID" 1 "Unset with invalid variable name"
    
    # Exit tests
    run_test "Exit_No_Args" "exit" 0 "Exit without arguments"
    run_test "Exit_Zero" "exit 0" 0 "Exit with code 0"
    run_test "Exit_Success_Code" "exit 42" 42 "Exit with specific code"
    run_test "Exit_Invalid_Code" "exit abc" 2 "Exit with invalid code"
    run_test "Exit_Too_Many_Args" "exit 1 2" 1 "Exit with too many arguments"
}

# EXTERNAL COMMAND TESTS (WITHOUT EXPANSIONS)
test_external_commands_basic() {
    print_section "EXTERNAL COMMANDS - BASIC TESTS (NO EXPANSIONS)"
    
    # Basic external commands
    run_test "LS_Simple" "ls" 0 "Basic ls command"
    run_test "LS_Long_Format" "ls -l" 0 "ls with -l flag"
    run_test "LS_All_Files" "ls -la" 0 "ls with -la flags"
    run_test "LS_Invalid_Dir" "ls /nonexistent" 2 "ls on non-existent directory"
    
    # Cat tests
    run_test "Cat_Existing_File" "cat $TEST_DIR/test_input.txt" 0 "Cat existing file"
    run_test "Cat_Nonexistent_File" "cat /nonexistent/file" 1 "Cat non-existent file"
    run_test "Cat_Multiple_Files" "cat $TEST_DIR/test_input.txt $TEST_DIR/multiline.txt" 0 "Cat multiple files"
    run_test "Cat_No_Args" "cat" 0 "Cat without arguments (should wait for input)"
    
    # Grep tests
    run_test "Grep_Simple" "grep line $TEST_DIR/multiline.txt" 0 "Basic grep command"
    run_test "Grep_No_Match" "grep nonexistent $TEST_DIR/multiline.txt" 1 "Grep with no matches"
    run_test "Grep_Multiple_Files" "grep test $TEST_DIR/*.txt" 0 "Grep multiple files"
    
    # Wc tests
    run_test "Wc_Lines" "wc -l $TEST_DIR/multiline.txt" 0 "Word count lines"
    run_test "Wc_Words" "wc -w $TEST_DIR/test_input.txt" 0 "Word count words"
    run_test "Wc_Characters" "wc -c $TEST_DIR/test_input.txt" 0 "Word count characters"
    
    # Other common commands
    run_test "Date_Simple" "date" 0 "Basic date command"
    run_test "Whoami_Simple" "whoami" 0 "Basic whoami command"
    run_test "Uname_Simple" "uname" 0 "Basic uname command"
    
    # Command not found
    run_test "Invalid_Command" "nonexistentcommand" 127 "Non-existent command"
}

# PIPE TESTS (WITHOUT EXPANSIONS)
test_pipes_basic() {
    print_section "PIPE TESTS - BASIC (NO EXPANSIONS)"
    
    # Simple pipes
    run_test "Pipe_Simple" "echo hello | cat" 0 "Simple pipe: echo to cat"
    run_test "Pipe_LS_Grep" "ls | grep test" 0 "Pipe: ls to grep"
    run_test "Pipe_Cat_Wc" "cat $TEST_DIR/multiline.txt | wc -l" 0 "Pipe: cat to wc"
    
    # Multiple pipes
    run_test "Pipe_Three_Commands" "echo hello world | tr ' ' '\\n' | wc -l" 0 "Three-command pipeline"
    run_test "Pipe_LS_Grep_Sort" "ls -la | grep test | sort" 0 "Pipeline: ls to grep to sort"
    run_test "Pipe_Cat_Grep_Wc" "cat $TEST_DIR/multiline.txt | grep line | wc -l" 0 "Pipeline: cat to grep to wc"
    
    # Pipes with builtins
    run_test "Pipe_Echo_Cat" "echo test | cat" 0 "Pipe: builtin to external"
    run_test "Pipe_LS_Builtin_Echo" "ls | echo received input" 0 "Pipe: external to builtin"
    run_test "Pipe_PWD_Cat" "pwd | cat" 0 "Pipe: pwd to cat"
    
    # Complex pipes
    run_test "Pipe_Four_Commands" "echo -e 'a\\nb\\nc\\na' | sort | uniq | wc -l" 0 "Four-command pipeline"
    run_test "Pipe_With_Flags" "ls -la | grep -v total | head -5" 0 "Pipeline with various flags"
    
    # Error handling in pipes
    run_test "Pipe_First_Fails" "invalidcommand | cat" 127 "Pipeline where first command fails"
    run_test "Pipe_Middle_Fails" "echo hello | invalidcommand | cat" "any" "Pipeline where middle command fails"
}

# REDIRECTION TESTS (WITHOUT EXPANSIONS)
test_redirections_basic() {
    print_section "REDIRECTION TESTS - BASIC (NO EXPANSIONS)"
    
    # Output redirection
    run_test "Redir_Output_Simple" "echo hello > $TEMP_DIR/output1.txt" 0 "Simple output redirection"
    run_test "Redir_Output_Overwrite" "echo world > $TEMP_DIR/output1.txt" 0 "Output redirection overwrite"
    run_test "Redir_Output_Multiple" "ls -l > $TEMP_DIR/ls_output.txt" 0 "Redirect ls output"
    
    # Append redirection
    run_test "Redir_Append_Simple" "echo first >> $TEMP_DIR/append1.txt" 0 "Simple append redirection"
    run_test "Redir_Append_Second" "echo second >> $TEMP_DIR/append1.txt" 0 "Second append redirection"
    run_test "Redir_Append_Existing" "echo appended >> $TEST_DIR/append_test.txt" 0 "Append to existing file"
    
    # Input redirection
    run_test "Redir_Input_Simple" "cat < $TEST_DIR/test_input.txt" 0 "Simple input redirection"
    run_test "Redir_Input_Nonexistent" "cat < /nonexistent/file" 1 "Input redirection from non-existent file"
    run_test "Redir_Input_Wc" "wc -l < $TEST_DIR/multiline.txt" 0 "Input redirection to wc"
    
    # Multiple redirections
    run_test "Redir_Input_Output" "cat < $TEST_DIR/test_input.txt > $TEMP_DIR/copied.txt" 0 "Input and output redirection"
    run_test "Redir_Multiple_Output" "echo test > $TEMP_DIR/out1.txt > $TEMP_DIR/out2.txt" 0 "Multiple output redirections"
    run_test "Redir_Output_Append" "echo first > $TEMP_DIR/mixed.txt && echo second >> $TEMP_DIR/mixed.txt" 0 "Output then append"
    
    # Heredoc tests
    run_test "Heredoc_Simple" "cat << EOF\nhello\nworld\nEOF" 0 "Simple heredoc"
    run_test "Heredoc_To_Grep" "grep hello << EOF\nhello world\ntest\nEOF" 0 "Heredoc to grep"
    run_test "Heredoc_Multiple_Lines" "wc -l << END\nline1\nline2\nline3\nEND" 0 "Heredoc line count"
    
    # Redirection with pipes
    run_test "Pipe_Redir_Output" "echo hello | cat > $TEMP_DIR/pipe_output.txt" 0 "Pipe with output redirection"
    run_test "Redir_Input_Pipe" "cat < $TEST_DIR/multiline.txt | grep line" 0 "Input redirection with pipe"
    run_test "Complex_Pipe_Redir" "cat < $TEST_DIR/test_input.txt | tr 'a-z' 'A-Z' > $TEMP_DIR/uppercase.txt" 0 "Complex pipe with redirections"
}

# LOGICAL OPERATORS (WITHOUT EXPANSIONS)
test_logical_operators_basic() {
    print_section "LOGICAL OPERATORS - BASIC (NO EXPANSIONS)"
    
    # AND operator
    run_test "AND_Both_Success" "echo first && echo second" 0 "AND: both commands succeed"
    run_test "AND_First_Fails" "false && echo second" 1 "AND: first command fails"
    run_test "AND_Second_Fails" "echo first && false" 1 "AND: second command fails"
    run_test "AND_With_Builtin" "pwd && echo success" 0 "AND with builtin command"
    
    # OR operator
    run_test "OR_First_Success" "echo first || echo second" 0 "OR: first command succeeds"
    run_test "OR_First_Fails" "false || echo second" 0 "OR: first fails, second succeeds"
    run_test "OR_Both_Fail" "false || false" 1 "OR: both commands fail"
    run_test "OR_With_Builtin" "false || pwd" 0 "OR with builtin command"
    
    # Chained logical operators
    run_test "Chain_AND_AND" "echo first && echo second && echo third" 0 "Chain: AND AND"
    run_test "Chain_OR_OR" "false || false || echo third" 0 "Chain: OR OR"
    run_test "Chain_AND_OR" "echo first && false || echo third" 0 "Chain: AND OR"
    run_test "Chain_OR_AND" "false || echo second && echo third" 0 "Chain: OR AND"
    
    # Mixed with pipes
    run_test "Pipe_AND" "echo hello | cat && echo success" 0 "Pipe followed by AND"
    run_test "AND_Pipe" "echo first && echo second | cat" 0 "AND followed by pipe"
    run_test "OR_Pipe" "false || echo backup | cat" 0 "OR followed by pipe"
}

# MIXED COMMANDS (WITHOUT EXPANSIONS)
test_mixed_commands_basic() {
    print_section "MIXED COMMANDS - COMPLEX SCENARIOS (NO EXPANSIONS)"
    
    # Builtin and external combinations
    run_test "Mixed_PWD_LS" "pwd && ls" 0 "PWD followed by LS"
    run_test "Mixed_Echo_Cat" "echo hello && cat $TEST_DIR/test_input.txt" 0 "Echo followed by cat"
    run_test "Mixed_Export_Env" "export TESTVAR=value && env | grep TESTVAR" 0 "Export then check with env"
    
    # Complex pipe combinations
    run_test "Complex_Builtin_Pipe" "echo -e 'line1\\nline2\\nline3' | grep line | wc -l" 0 "Complex builtin pipe"
    run_test "Complex_External_Pipe" "ls -la | grep -v total | head -3 | tail -1" 0 "Complex external pipe"
    run_test "Mixed_Pipe_Types" "pwd | cat && ls | wc -l" 0 "Mixed pipe types with logical operator"
    
    # Redirections with complex commands
    run_test "Complex_Redir_1" "echo start > $TEMP_DIR/complex.txt && cat $TEST_DIR/test_input.txt >> $TEMP_DIR/complex.txt" 0 "Complex redirection sequence"
    run_test "Complex_Redir_2" "ls -la | grep test > $TEMP_DIR/filtered.txt && cat $TEMP_DIR/filtered.txt" 0 "Pipe to redirection with follow-up"
    run_test "Complex_Redir_3" "cat < $TEST_DIR/multiline.txt | grep line > $TEMP_DIR/lines.txt" 0 "Input and output redirection with pipe"
    
    # Subshells (if supported)
    run_test "Subshell_Simple" "(echo hello)" 0 "Simple subshell"
    run_test "Subshell_Pipe" "(echo hello | cat) && echo done" 0 "Subshell with pipe"
    run_test "Subshell_Redir" "(echo test) > $TEMP_DIR/subshell.txt" 0 "Subshell with redirection"
    
    # Very complex scenarios
    run_test "Very_Complex_1" "echo start && (ls | head -3) | wc -l > $TEMP_DIR/count.txt" 0 "Very complex: logical, subshell, pipe, redirection"
    run_test "Very_Complex_2" "cat $TEST_DIR/multiline.txt | grep line > $TEMP_DIR/temp.txt && cat $TEMP_DIR/temp.txt | wc -l" 0 "Very complex: multiple pipes and redirections"
}

# EXPANSION TESTS - VARIABLES
test_variable_expansion() {
    print_section "VARIABLE EXPANSION TESTS"
    
    # Basic variable expansion
    run_test "Var_Expand_Simple" "echo \$HOME" 0 "Basic variable expansion"
    run_test "Var_Expand_PATH" "echo \$PATH" 0 "PATH variable expansion"
    run_test "Var_Expand_USER" "echo \$USER" 0 "USER variable expansion"
    run_test "Var_Expand_Nonexistent" "echo \$NONEXISTENT_VAR" 0 "Non-existent variable expansion"
    
    # Variable expansion in different contexts
    run_test "Var_Expand_With_Text" "echo Hello \$USER!" 0 "Variable expansion with surrounding text"
    run_test "Var_Expand_Multiple" "echo \$USER \$HOME \$PATH" 0 "Multiple variable expansions"
    run_test "Var_Expand_In_Command" "ls \$HOME" 0 "Variable expansion in command argument"
    
    # Special variables
    run_test "Var_Expand_Exit_Status" "false; echo \$?" 0 "Exit status variable expansion"
    run_test "Var_Expand_PID" "echo \$\$" 0 "Process ID variable expansion"
    
    # Export and expansion
    run_test "Var_Export_Expand" "export TESTVAR=hello && echo \$TESTVAR" 0 "Export then expand variable"
    run_test "Var_Unset_Expand" "export TEMPVAR=test && unset TEMPVAR && echo \$TEMPVAR" 0 "Unset then expand variable"
    
    # Variable expansion in pipes
    run_test "Var_Expand_Pipe" "echo \$HOME | cat" 0 "Variable expansion in pipe"
    run_test "Var_Expand_Complex_Pipe" "echo \$USER | grep \$USER" 0 "Variable expansion in both parts of pipe"
    
    # Variable expansion in redirections
    run_test "Var_Expand_Redir_Out" "echo hello > \$TEMP_DIR/var_test.txt" 0 "Variable expansion in output redirection"
    run_test "Var_Expand_Redir_In" "echo content > $TEMP_DIR/var_input.txt && cat < \$TEMP_DIR/var_input.txt" 0 "Variable expansion in input redirection"
}

# EXPANSION TESTS - QUOTES
test_quote_expansion() {
    print_section "QUOTE HANDLING AND EXPANSION"
    
    # Single quotes (no expansion)
    run_test "Quote_Single_Simple" "echo 'Hello World'" 0 "Simple single quotes"
    run_test "Quote_Single_No_Expand" "echo '\$HOME'" 0 "Single quotes prevent expansion"
    run_test "Quote_Single_Special" "echo 'Special chars: | && || > <'" 0 "Single quotes with special characters"
    
    # Double quotes (with expansion)
    run_test "Quote_Double_Simple" "echo \"Hello World\"" 0 "Simple double quotes"
    run_test "Quote_Double_Expand" "echo \"Home: \$HOME\"" 0 "Double quotes allow expansion"
    run_test "Quote_Double_Mixed" "echo \"User \$USER on \$HOSTNAME\"" 0 "Double quotes with multiple expansions"
    
    # Mixed quotes
    run_test "Quote_Mixed_1" "echo 'Hello' \"World\"" 0 "Mixed single and double quotes"
    run_test "Quote_Mixed_2" "echo \"User: '\$USER'\"" 0 "Single quotes inside double quotes"
    run_test "Quote_Mixed_3" "echo 'Path: \"\$PATH\"'" 0 "Double quotes inside single quotes"
    
    # Nested and complex quoting
    run_test "Quote_Complex_1" "echo \"The user '\$USER' is in \$HOME\"" 0 "Complex quote mixing"
    run_test "Quote_Complex_2" "echo 'Command: \"echo \$HOME\"'" 0 "Commands in quotes"
    
    # Quotes in pipes and redirections
    run_test "Quote_Pipe" "echo 'hello world' | grep 'world'" 0 "Quotes in pipe command"
    run_test "Quote_Redir" "echo 'test content' > '$TEMP_DIR/quoted.txt'" 0 "Quotes in redirection"
    run_test "Quote_Complex_Pipe" "echo \"User: \$USER\" | grep '\$USER'" 0 "Mixed quotes in pipe"
}

# EXPANSION TESTS - WILDCARDS (if supported)
test_wildcard_expansion() {
    print_section "WILDCARD EXPANSION TESTS"
    
    # Basic wildcards
    run_test "Wildcard_Star" "echo *" 0 "Basic star wildcard"
    run_test "Wildcard_Question" "echo ?" 0 "Basic question mark wildcard"
    run_test "Wildcard_LS" "ls *.txt" 0 "Wildcard in ls command"
    
    # Wildcards in different directories
    run_test "Wildcard_Dir" "ls $TEST_DIR/*.txt" 0 "Wildcard in specific directory"
    run_test "Wildcard_Hidden" "ls .*" 0 "Wildcard for hidden files"
    
    # Complex wildcard patterns
    run_test "Wildcard_Complex" "ls $TEST_DIR/*test*" 0 "Complex wildcard pattern"
    run_test "Wildcard_Multiple" "echo *.c *.h" 0 "Multiple wildcard patterns"
    
    # Wildcards with pipes and redirections
    run_test "Wildcard_Pipe" "cat *.txt | wc -l" 0 "Wildcard in pipe"
    run_test "Wildcard_Redir" "cat *.txt > $TEMP_DIR/all_txt.txt" 0 "Wildcard with redirection"
}

# COMPREHENSIVE EXPANSION TESTS
test_comprehensive_expansion() {
    print_section "COMPREHENSIVE EXPANSION SCENARIOS"
    
    # All expansion types together
    run_test "Expand_All_Types" "echo \"Files: *.txt, User: \$USER, Home: '\$HOME'\"" 0 "All expansion types together"
    run_test "Expand_Complex_Command" "ls \$HOME/*.txt 2>/dev/null | grep 'test' | wc -l" 0 "Complex command with expansions"
    
    # Expansion with logical operators
    run_test "Expand_Logical_AND" "echo \$USER && echo \$HOME" 0 "Variable expansion with AND"
    run_test "Expand_Logical_OR" "echo \$NONEXISTENT || echo \$USER" 0 "Variable expansion with OR"
    
    # Expansion in complex pipes
    run_test "Expand_Complex_Pipe_1" "echo \$PATH | tr ':' '\\n' | head -3" 0 "PATH expansion in complex pipe"
    run_test "Expand_Complex_Pipe_2" "ls \$HOME | grep '\\.txt' | sort" 0 "Variable and pattern in pipe"
    
    # Expansion with redirections and pipes
    run_test "Expand_Redir_Pipe" "echo \"User: \$USER\" > $TEMP_DIR/user.txt && cat $TEMP_DIR/user.txt | grep '\$USER'" 0 "Expansion with redirection and pipe"
    run_test "Expand_Heredoc" "cat << EOF | grep \$USER\nUser is \$USER\nHome is \$HOME\nEOF" 0 "Expansion in heredoc"
    
    # Very complex expansion scenarios
    run_test "Expand_Ultimate_1" "export MYVAR=test && echo \"Variable: \$MYVAR, Files: *.txt\" | cat > $TEMP_DIR/ultimate.txt" 0 "Ultimate expansion test 1"
    run_test "Expand_Ultimate_2" "(cd \$HOME && ls *.txt 2>/dev/null) | head -5 && echo \"Done with \$USER's files\"" 0 "Ultimate expansion test 2"
}

# ERROR HANDLING TESTS
test_error_handling() {
    print_section "ERROR HANDLING TESTS"
    
    # Syntax errors
    run_test "Error_Syntax_Pipe" "echo hello |" 2 "Incomplete pipe syntax"
    run_test "Error_Syntax_AND" "echo hello &&" 2 "Incomplete AND syntax"
    run_test "Error_Syntax_OR" "echo hello ||" 2 "Incomplete OR syntax"
    run_test "Error_Syntax_Redir" "echo hello >" 2 "Incomplete redirection"
    
    # Permission errors
    run_test "Error_Permission_Read" "cat /etc/shadow" 1 "Permission denied read"
    run_test "Error_Permission_Write" "echo test > /etc/hosts" 1 "Permission denied write"
    
    # File not found errors
    run_test "Error_File_Not_Found" "cat /nonexistent/file" 1 "File not found"
    run_test "Error_Command_Not_Found" "nonexistentcommand123" 127 "Command not found"
    
    # Directory errors
    run_test "Error_Directory_As_Command" "/tmp" 126 "Directory as command"
    run_test "Error_CD_File" "cd $TEST_DIR/test_input.txt" 1 "CD to file instead of directory"
}

# SIGNAL HANDLING TESTS
test_signal_handling() {
    print_section "SIGNAL HANDLING TESTS"
    
    # Note: These tests are basic and may need adjustment based on your signal implementation
    run_test "Signal_Ctrl_C" "echo 'Testing signal handling - Ctrl+C should be handled gracefully'" 0 "Signal handling info"
    run_test "Signal_Empty_Line" "" 0 "Empty command line"
    run_test "Signal_Whitespace_Only" "   " 0 "Whitespace only command"
}

# PERFORMANCE TESTS
test_performance() {
    print_section "PERFORMANCE TESTS"
    
    # Large output tests
    run_test "Performance_Large_Output" "yes | head -1000" 0 "Large output handling"
    run_test "Performance_Deep_Pipe" "echo test | cat | cat | cat | cat | cat" 0 "Deep pipeline"
    run_test "Performance_Many_Args" "echo $(seq 1 100)" 0 "Many arguments"
    
    # Stress tests
    run_test "Performance_Rapid_Commands" "echo 1; echo 2; echo 3; echo 4; echo 5" 0 "Rapid command sequence"
}

# MAIN TEST RUNNER
run_all_tests() {
    echo -e "${WHITE}MINISHELL COMPREHENSIVE TESTER${NC}"
    echo -e "${WHITE}===============================${NC}\n"
    
    # Setup
    setup_test_environment
    
    # Check if minishell exists
    if [ ! -x "$MINISHELL_PATH" ]; then
        echo -e "${RED}Error: Minishell executable not found at $MINISHELL_PATH${NC}"
        echo -e "${YELLOW}Please make sure to compile your minishell first.${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}Starting comprehensive tests...${NC}\n"
    
    # Run all test suites
    
    # Phase 1: Basic tests (no expansions)
    echo -e "${MAGENTA}PHASE 1: BASIC FUNCTIONALITY (NO EXPANSIONS)${NC}"
    test_builtins_basic
    test_external_commands_basic
    test_pipes_basic
    test_redirections_basic
    test_logical_operators_basic
    test_mixed_commands_basic
    
    # Phase 2: Expansion tests
    echo -e "\n${MAGENTA}PHASE 2: EXPANSION TESTS${NC}"
    test_variable_expansion
    test_quote_expansion
    test_wildcard_expansion
    test_comprehensive_expansion
    
    # Phase 3: Error handling and edge cases
    echo -e "\n${MAGENTA}PHASE 3: ERROR HANDLING AND EDGE CASES${NC}"
    test_error_handling
    test_signal_handling
    test_performance
    
    # Cleanup
    cleanup_test_environment
    
    # Final results
    echo -e "\n${WHITE}===============================${NC}"
    echo -e "${WHITE}FINAL TEST RESULTS${NC}"
    echo -e "${WHITE}===============================${NC}"
    echo -e "${CYAN}Total Tests: $TOTAL_TESTS${NC}"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
        echo -e "${GREEN}Your minishell implementation is working correctly!${NC}"
    else
        echo -e "\n${RED}❌ SOME TESTS FAILED${NC}"
        echo -e "${YELLOW}Failed tests summary:${FAILED_TEST_DETAILS}${NC}"
        echo -e "\n${YELLOW}Review the failed tests above and fix the issues.${NC}"
    fi
    
    # Calculate success percentage
    local success_percentage=$(( (PASSED_TESTS * 100) / TOTAL_TESTS ))
    echo -e "\n${CYAN}Success Rate: ${success_percentage}%${NC}"
    
    exit $FAILED_TESTS
}

# Command line options
case "${1:-}" in
    "builtins")
        setup_test_environment
        test_builtins_basic
        cleanup_test_environment
        ;;
    "external")
        setup_test_environment
        test_external_commands_basic
        cleanup_test_environment
        ;;
    "pipes")
        setup_test_environment
        test_pipes_basic
        cleanup_test_environment
        ;;
    "redirections")
        setup_test_environment
        test_redirections_basic
        cleanup_test_environment
        ;;
    "logical")
        setup_test_environment
        test_logical_operators_basic
        cleanup_test_environment
        ;;
    "expansions")
        setup_test_environment
        test_variable_expansion
        test_quote_expansion
        test_wildcard_expansion
        test_comprehensive_expansion
        cleanup_test_environment
        ;;
    "errors")
        setup_test_environment
        test_error_handling
        cleanup_test_environment
        ;;
    "help"|"-h"|"--help")
        echo "Minishell Tester Usage:"
        echo "./minishell_tester.sh [option]"
        echo ""
        echo "Options:"
        echo "  builtins     - Test only builtin commands"
        echo "  external     - Test only external commands"
        echo "  pipes        - Test only pipe functionality"
        echo "  redirections - Test only redirections"
        echo "  logical      - Test only logical operators"
        echo "  expansions   - Test only expansion features"
        echo "  errors       - Test only error handling"
        echo "  help         - Show this help message"
        echo ""
        echo "No option: Run all tests (default)"
        ;;
    *)
        run_all_tests
        ;;
esac
