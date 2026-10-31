# minishell (asfoorshell)

A small Bash-like shell written in C for the 42 curriculum. It tokenizes a command line, builds an abstract syntax tree, expands variables and quotes, then executes the tree with pipes, redirections and here-documents.

```
asfoorshell🐥:~ echo hello | tr a-z A-Z
HELLO
asfoorshell🐥:~ export NAME=Thaer
asfoorshell🐥:~ echo $NAME
Thaer
asfoorshell🐥:~ ls nonexistent
ls: cannot access 'nonexistent': No such file or directory
asfoorshell🐥:~ echo $?
2
```

## Features

- Interactive prompt with history (GNU readline)
- Builtins: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- Pipelines with `|`, and the logical operators `&&` and `||`
- Redirections `<`, `>`, `>>`, and here-documents `<<`
- Single and double quotes, `$VAR` expansion and `$?` (exit status of the last command)
- Signal handling for `Ctrl-C`, `Ctrl-D` and `Ctrl-\`, with different behavior at the prompt, in a child process and inside a here-document
- External commands found through `PATH`

## Build and run

Requirements: a C compiler, `make`, and the readline development headers (`sudo apt install libreadline-dev` on Debian or Ubuntu).

```sh
make
./minishell
```

## How it works

Each stage is its own module under `src/`, and most have a README with the data structures and examples.

| Module | Role |
| --- | --- |
| `lexer/` | Splits the input into tokens (words, operators, redirections) and records quote state |
| `parser/` | Builds the AST: simple commands, pipelines and logical operators |
| `expander/` | Expands environment variables and removes quotes |
| `execution/` | Walks the AST, forks processes, wires pipes and runs commands |
| `redirections/` | Opens files and handles here-documents |
| `builtin/` | The builtin commands |
| `enviroment/` | Copy of the environment and variable lookup |
| `cleanup.c`, `signals*.c` | Freeing memory and signal modes |

For `echo hello | wc -l` the parser produces:

```
PIPE
├── left:  SIMPLE_COMMAND argv=["echo", "hello"]
└── right: SIMPLE_COMMAND argv=["wc", "-l"]
```

## Testing

`minishell_tester.sh` runs a set of commands through the shell. To check for leaks and file descriptor problems:

```sh
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=readline_curses.supp ./minishell
```

`readline_curses.supp` hides leaks that come from the readline library itself.

## Authors

Built by [thaqer](https://github.com/thaqer) and [daniah](https://github.com/daniah).
