/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strategy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/21 16:11:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/09/06 20:50:11 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include "libft.h"
//#include <stdio.h>

static void		ft_get_players_position(void)
{
	int			i;
	int			p;

	i = ft_nb_players(e.data->connected);
	ft_memset(e.players, 0, i * sizeof(*e.players));
	i = 0;
	p = 0;
	e.target = NULL;
	while (i < MAP_WIDTH * MAP_HEIGTH)
	{
		if (e.map[i] != MAP_VALUE)
		{
			e.players[p].team = e.map[i];
			e.players[p].x = i % MAP_WIDTH;
			e.players[p].y = i / MAP_WIDTH;
			e.players[p].dist = (e.x > e.players[p].x) ?
				e.x - e.players[p].x : e.players[p].x - e.x;
			e.players[p].dist += (e.y > e.players[p].y) ?
				e.y - e.players[p].y : e.players[p].y - e.y;
			if (e.team != e.players[p].team &&
				(e.target == NULL || e.players[p].dist < e.target->dist))
				e.target = &e.players[p];
			p += sizeof(*e.players);
		}
		i++;
	}
}

void			ft_strategy(void)
{
	ft_get_players_position();
	ft_rcvmsg();
	ft_move_to_target();
	ft_sendmsg();
}
