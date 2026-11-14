/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:53:25 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 20:39:56 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdlib.h>

typedef struct s_shell	t_shell;
typedef struct s_env	t_env;

typedef struct s_builtin
{
	char				*name;
	int					(*func)(char **args, t_shell *shell);
}						t_builtin;

int						is_builtin_command(char *cmd);
int						execute_builtin(char **argv, t_shell *shell);

// Individual builtin commands (argv-based versions)
int						ft_cd_argv(char **argv, t_shell *shell);
int						ft_echo_argv(char **argv, t_shell *shell);
int						ft_exit_argv(char **argv, t_shell *shell);
int						ft_export_argv(char **argv, t_shell *shell);
t_env					*new_env_node(const char *key, const char *value);
int						set_env(t_shell *shell, const char *key,
							const char *value, int *flag);
int						parse_export_arg(char *arg, char **key, char **value);
void					print_export_list(t_env *env);
int						print_env(char **args, t_shell *shell);
int						ft_pwd(char **args, t_shell *shell);
int						ft_unset_argv(char **argv, t_shell *shell);

// Helper functions for cd
void					end_cd_process(t_shell *shell, char *oldpwd, char *pwd);

// Helper functions for export
void					handle_value(char *value);
void					veiled_key(char *key, t_shell *shell, int *flag);
t_env					*find_env(char *key, t_env *env);

// Cleanup and utility functions
void					cleanup(t_shell *shell);
void					ft_free_array(char **array);
void					free_env(t_env *env);

// Error handling
int						shell_error_message(char *message);
#endif
