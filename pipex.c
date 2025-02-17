/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:54:57 by marvin            #+#    #+#             */
/*   Updated: 2025/02/12 16:54:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char *argv)
{
    int fd[2];
    int id;

    if (argc != 5)
        return (perror("Incorrect amount of arguments", 1));
    if (pipe(fd) == -1)
        return (perror("pipe"), 1);
    id = fork();
    if (id == -1)
        return (perror("fork"), 1);
    open(argv[1], O_RDONLY);
    open(argv[4], O_CREAT | O_WRONLY | O_TRUNC);
    //open kontrol
    if (id == 0)
    {

    }
}
