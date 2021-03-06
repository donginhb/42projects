/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/14 23:13:57 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/08/30 21:02:23 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/sem.h>

void		ft_exit_server(int print_err, char *err)
{
	fprintf(stderr, "%s: ", e.prog);
	if (print_err)
		perror(err);
	else
		fprintf(stderr, "%s\n", err);
	if (e.msgqid != -1 && msgctl(e.msgqid, IPC_RMID, NULL))
		perror("msgctl");
	if (e.semid != -1 && semctl(e.semid, 0, IPC_RMID))
		perror("semctl");
	if (e.shmid != -1)
	{
		if (e.data != (void *)-1 && shmdt(e.data))
			perror("shmdt");
		if (shmctl(e.shmid, IPC_RMID, NULL))
			perror("shmctl");
	}
	ft_restore_term();
	if (e.players != (void *)-1)
		free(e.players);
	ft_bzero(&e, sizeof(e));
	exit(1);
}

void		ft_exit_client(int print_err, char *err)
{
	fprintf(stderr, "%s: ", e.prog);
	if (print_err)
		perror(err);
	else
		fprintf(stderr, "%s\n", err);
	if (e.data != (void *)-1 && shmdt(e.data) == -1)
		perror("shmdt");
	ft_restore_term();
	if (e.players != (void *)-1)
		free(e.players);
	ft_bzero(&e, sizeof(e));
	exit(1);
}

void		ft_exit(int print_err, char *err)
{
	fprintf(stderr, "%s: ", e.prog);
	if (print_err)
		perror(err);
	else
		fprintf(stderr, "%s\n", err);
	ft_restore_term();
	ft_bzero(&e, sizeof(e));
	exit(1);
}

void		ft_free_exit(void)
{
	if (ft_nb_players(e.data->connected) == 0)
	{
		if (e.msgqid != -1 && msgctl(e.msgqid, IPC_RMID, NULL))
			perror("msgctl");
		if (e.semid != -1 && semctl(e.semid, 0, IPC_RMID))
			perror("semctl");
		if (e.shmid != -1)
		{
			if (e.data != (void *)-1 && shmdt(e.data))
				perror("shmdt");
			if (shmctl(e.shmid, IPC_RMID, NULL))
				perror("shmctl");
		}
	}
	else if (e.data != (void *)-1 && shmdt(e.data) == -1)
		perror("shmdt");
	ft_restore_term();
	if (e.players != (void *)-1)
		free(e.players);
	ft_bzero(&e, sizeof(e));
	exit(0);
}
