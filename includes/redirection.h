/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:08:22 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 18:49:39 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdbool.h>
// Forward declarations
typedef struct s_shell	t_shell;

// Redirection types (matches parser.h definitions)
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

// Redirection node structure
typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	int				heredoc_fd;
	bool			s_delim_quoted;
	struct s_redir	*next;
}						t_redir;

// Core redirection functions
int						handle_redirections(t_redir *redirs, t_shell *shell);
int						handle_heredoc_redir(char *delimiter, t_shell *shell);
int						handle_heredocs_first(t_redir *redirs, t_shell *shell);
char						*expand_heredoc_line(char *line, t_shell *shell);

#endif