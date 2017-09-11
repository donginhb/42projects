/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 17:14:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/09/06 13:12:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void		print_map(void)
{
	int			i;
	int			j;
	char		c;

	i = 0;
	while (i < MAP_HEIGTH)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			c = *(e.map + (i * MAP_WIDTH + j));
			c = (c == MAP_VALUE) ? '.' : c + '0';
			write(1, &c, 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
	i = 0;
	while (i++ < MAX_TEAMS)
		printf("%d(%d) ", i - 1, e.data->connected[i - 1]);
	printf("\n");
}

static void		print_info(void)
{
	if (ft_check_minimum_players(e.data->connected) != 0)
		printf("\033[1;31mWAITING FOR PLAYERS... \033[0m\n");
	else if (ft_check_even_teams(e.data->connected) != 0)
		printf("\033[1;33mWAITING FOR EVEN TEAMS...\033[0m\n");
	else
		printf("\033[1;32mGAME IN PROGRESS...\033[00m\n");
}

void			exec_print(void)
{
	extern char	**environ;

	e.pid = fork();
	if (e.pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (e.pid == 0)
	{
		ft_termcaps(environ);
		ft_signal(ft_restore_term);
		ft_termdo("cl");
		ft_termdo("sc");
		while (e.data && e.data->end == -1)
		{
			ft_termdo("rc");
			print_map();
			print_info();
		}
		if (e.data)
			printf("Team %d has won the game !!!\n", e.data->end);
		ft_restore_term();
	}
}