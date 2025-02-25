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

void	execute_command(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args || !args[0])
	{
		ft_putstr_fd("empty command\n", 2);
		exit(1);
	}
	path = get_path(args[0], env);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		exit(127);
	}
	execve(path, args, env);
	perror("execve error");
	ft_free_tab(args);
	free(path);
	exit(1);
}

void	child_process_one(int fd[2], char *infile, char *cmd, char **env)
{
	int	file;

	file = open(infile, O_RDONLY);
	if (file == -1)
	{
		perror("problem opening infile");
		exit(1);
	}
	dup2(file, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(file);
	close(fd[0]);
	close(fd[1]);
	execute_command(cmd, env);
}

void	child_process_two(int fd[2], char *outfile, char *cmd, char **env)
{
	int	file;

	file = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file == -1)
	{
		perror("problem opening outfile");
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(file, STDOUT_FILENO);
	close(file);
	close(fd[0]);
	close(fd[1]);
	execute_command(cmd, env);
}

int	main(int argc, char *argv[], char *env[])
{
	int	fd[2];
	int	id1;
	int	id2;

	if (argc != 5)
		return (perror("Incorrect amount of arguments"), 1);
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	id1 = fork();
	if (id1 == -1)
		return (perror("fork"), 1);
	if (id1 == 0)
		child_process_one(fd, argv[1], argv[2], env);
	id2 = fork();
	if (id2 == -1)
		return (perror("fork"), 1);
	if (id2 == 0)
		child_process_two(fd, argv[4], argv[3], env);
	close(fd[0]);
	close(fd[1]);
	waitpid(id1, NULL, 0);
	waitpid(id2, NULL, 0);
	return (0);
}
