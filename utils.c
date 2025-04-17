/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:51:06 by efe               #+#    #+#             */
/*   Updated: 2025/04/17 09:12:43 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static char	*join_paths(const char *dir, const char *cmd)
{
	char	*full_path;
	size_t	len;
	size_t	dir_len;

	dir_len = ft_strlen(dir);
	len = dir_len + ft_strlen(cmd) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + 1);
	ft_strlcat(full_path, "/", len);
	ft_strlcat(full_path, cmd, len);
	return (full_path);
}

static char	*search_path(char **paths, char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_paths(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0 && ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i] || !*(env[i] + 5))
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
	return (search_path(paths, cmd));
}

void	execute_command(t_process *process, char *env[])
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(process->cmd, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		ft_free_tab(cmd_args);
		exit(127);
	}
	cmd_path = get_path(cmd_args[0], env);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putstr_fd(cmd_args[0], 2);
		ft_putchar_fd('\n', 2);
		ft_free_tab(cmd_args);
		exit(127);
	}
	execve(cmd_path, cmd_args, env);
	perror("pipex: execve error");
	ft_free_tab(cmd_args);
	free(cmd_path);
	exit(1);
}

void	create_forks(int *id, t_process *processes, int *fd, char *env[])
{
	id[0] = fork();
	if (id[0] == -1)
	{
		perror("pipex: fork error");
		exit(1);
	}
	if (id[0] == 0)
	{
		close(fd[0]);
		set_up_file(&processes[0], fd);
		execute_command(&processes[0], env);
	}
	id[1] = fork();
	if (id[1] == -1)
	{
		perror("pipex: fork error");
		exit(1);
	}
	if (id[1] == 0)
	{
		close(fd[1]);
		set_up_file(&processes[1], fd);
		execute_command(&processes[1], env);
	}
}
