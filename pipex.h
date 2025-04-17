/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:55:00 by marvin            #+#    #+#             */
/*   Updated: 2025/04/12 14:30:14 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"

typedef struct s_process
{
	int		in_out;
	char	*file;
	char	*cmd;
}	t_process;

void	ft_free_tab(char **tab);
char	*get_path(char *cmd, char **env);
void	create_forks(int *id, t_process *processes, int *fd, char *env[]);
void	execute_command(t_process *process, char *env[]);
void	set_up_file(t_process *process, int *fd);

#endif