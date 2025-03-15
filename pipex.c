/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efe <efe@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:54:57 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 16:34:49 by efe              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_process
{
	int		in_out;
	char	*file;
	char	*cmd;
}	t_process;

void	execute_command(char *cmd, char **env)
{
	char	*path;
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args || !args[0])
	{
		ft_putstr_fd("empty command\n", 2);
		ft_free_tab(args);
		exit(1);
	}
	path = get_path(args[0], env);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		ft_putstr_fd(args[0], 2);
		ft_free_tab(args);
		exit(127);
	}
	execve(path, args, env);
	perror("execve error");
	ft_free_tab(args);
	free(path);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(126);
}

void	set_up_file(t_process *process, int *fd)
{
	int	file_fd;

	if (process->in_out == 0)
		file_fd = open(process->file, O_RDONLY);
	else
		file_fd = open(process->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		ft_putstr_fd("pipex: error opening file\n", 2);
		exit(1);
	}
	if (process->in_out == 0)
	{
		if (dup2(file_fd, STDIN_FILENO) == -1
			|| dup2(fd[1], STDOUT_FILENO) == -1)
			perror("pipex dup2 error");
	}
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1
			|| dup2(file_fd, STDOUT_FILENO) == -1)
			perror("pipex dup2 error");
	}
	close(file_fd);
}

void	child_process(t_process *process, int *fd, char **env)
{
	set_up_file(process, fd);
	close(fd[0]);
	close(fd[1]);
	execute_command(process->cmd, env);
}

void	create_forks(int *id, t_process *processes, int *fd, char **env)
{
	id[0] = fork();
	if (id[0] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (id[0] == 0)
		child_process(&processes[0], fd, env);
	id[1] = fork();
	if (id[1] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (id[1] == 0)
		child_process(&processes[1], fd, env);
}

int	main(int argc, char *argv[], char *env[])
{
	int			fd[2];
	int			id[2];
	int			status1;
	int			status2;
	t_process	processes[2];

	if (argc != 5)
		return (perror("Incorrect amount of arguments"), 1);
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	processes[0].in_out = 0;
	processes[0].file = argv[1];
	processes[0].cmd = argv[2];
	processes[1].in_out = 1;
	processes[1].file = argv[4];
	processes[1].cmd = argv[3];
	create_forks(id, processes, fd, env);
	close(fd[0]);
	close(fd[1]);
	waitpid(id[0], &status1, 0);
	waitpid(id[1], &status2, 0);
	if (status1 != 0)
		return (status1);
	if (status2 != 0)
		return (status2);
	return (0);
}

