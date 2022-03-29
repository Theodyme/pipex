/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 18:56:35 by flplace           #+#    #+#             */
/*   Updated: 2022/03/27 16:37:09 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	buildfreer(t_bld *s)
{
	if (s->fdin != -1)
		close(s->fdin);
	if (s->fdout != -1)
		close(s->fdout);
	free_split(s->cmd[0]);
	free_split(s->cmd[1]);
	free(s->path[0]);
	free(s->path[1]);
	return ;
}

int	childprocess(t_bld *s, int *pipe)
{
	if (s->fdin == -1 || !(s->path[0]))
	{
		buildfreer(s);
		close(pipe[0]);
		close(pipe[1]);
		exit(1);
	}
	dup2(s->fdin, 0);
	dup2(pipe[1], 1);
	close(pipe[0]);
	close(pipe[1]);
	close(s->fdin);
	close(s->fdout);
	execve(s->path[0], &(s->cmd[0][0]), s->env);
	return (-1);
}

void	parentprocess(t_bld *s, int *pipe)
{
	if (!(s->path[1]))
	{
		close(pipe[1]);
		close(pipe[0]);
		return ;
	}
	dup2(pipe[0], 0);
	dup2(s->fdout, 1);
	close(s->fdin);
	close(s->fdout);
	close(pipe[0]);
	close(pipe[1]);
	execve(s->path[1], &(s->cmd[1][0]), s->env);
	return ;
}

void	launcher(t_bld *s)
{
	int	pid[2];
	int	tuyau[2];
	int	status;

	status = 0;
	pipe(tuyau);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		childprocess(s, tuyau);
	}
	else
	{
		pid[1] = fork();
		if (pid[1] == 0)
			parentprocess(s, tuyau);
		else
		{
			close(tuyau[0]);
			close(tuyau[1]);
			waitpid(pid[0], &status, 0);
			waitpid(pid[1], &status, 0);
		}
	}
	return ;
}

int	main(int ac, char **av, char **env)
{
	t_bld	s;

	if (ac != 5 || !env[0])
		return (0);
	s.env = env;
	s.fdin = fdsbuilder(av, 1);
	s.fdout = fdsbuilder(av, 2);
	if (!av[2][0] || !av[3][0] || (!av[2][0] && !av[3][0]))
	{
		write(0, "error: command not found.", 25);
		close(s.fdin);
		close(s.fdout);
		return (0);
	}
	if (s.fdout >= 0)
	{
		s.cmd[0] = cmdbuilder(av[2]);
		s.cmd[1] = cmdbuilder(av[3]);
		s.path[0] = pathbuilder(pathfinder(env), s.cmd[0][0]);
		s.path[1] = pathbuilder(pathfinder(env), s.cmd[1][0]);
		launcher(&s);
		buildfreer(&s);
	}
	close(s.fdin);
	return (0);
}
