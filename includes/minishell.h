/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:12:42 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 20:38:10 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtin.h"
# include "enviroment.h"
# include "execution.h"
# include "expander.h"
# include "lexer.h"
# include "libft.h"
# include "parser.h"
# include "redirection.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell
{
	t_token		*tokens;
	int			status;
	int			sav_stdin;
	int			sav_stdout;
	int			single_q;
	t_ast		*ast_root;
	t_parser	*parser;
	t_env		*env;
	t_execution	*execution;
}				t_shell;

void			setup_signals(int mode);
void			run_ast_if_no_signal(t_shell *shell);
void			sig_status(t_shell *shell, t_expander_creds *expander_creds);
#endif
