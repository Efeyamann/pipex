/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:54:57 by marvin            #+#    #+#             */
/*   Updated: 2025/04/15 16:45:19 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static int	open_file(t_process *process)
{
	int	file_fd;

	if (process->in_out == 0)
		file_fd = open(process->file, O_RDONLY);
	else
		file_fd = open(process->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		perror("pipex: error opening file");
		exit(1);
	}
	return (file_fd);
}

static void	dup_file(t_process *process, int *fd, int file_fd)
{
	if (process->in_out == 0)
	{
		if (dup2(file_fd, STDIN_FILENO) == -1
			|| dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("pipex dup2 error");
			exit(1);
		}
	}
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1
			|| dup2(file_fd, STDOUT_FILENO) == -1)
		{
			perror("pipex dup2 error");
			exit(1);
		}
	}
}

void	set_up_file(t_process *process, int *fd)
{
	int	file_fd;

	file_fd = open_file(process);
	dup_file(process, fd, file_fd);
	close(file_fd);
}

int	get_exit_status(int status)
{
	return ((status & 0xff00) >> 8);
}

int	main(int argc, char *argv[], char *env[])
{
	int			fd[2];
	int			id[2];
	int			status1;
	int			status2;
	t_process	processes[2];

	if (argc != 5)
		return (ft_putstr_fd("Error: Incorrect arguments\n", 2), 1);
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
	return (get_exit_status(status2));
}
