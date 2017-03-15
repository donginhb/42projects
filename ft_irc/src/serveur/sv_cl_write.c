/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_cl_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/21 17:15:05 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/15 05:15:30 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <sys/socket.h>

static void			sv_aff_wr(t_fd *cl)
{
	write(1, "Client ", 8);
	write(1, cl->addr, ft_strlen(cl->addr));
	write(1, " ", 1);
	write(1, cl->port, ft_strlen(cl->port));
	write(1, " wr: ", 5);
	write(1, cl->wr.start, BUFF);
	write(1, "\n", 1);
}

static void			sv_null(char **cmds, t_env *e, t_fd *cl)
{
	sv_err(ERR_UNKNOWNCOMMAND, cmds[0], NULL, cl, e);
}

static void			sv_cmd_client(t_env *e, t_fd *cl)
{
	char			**cmds;
	char			*dup;
	int				nb;
	static t_com	com[] = { SV_COMMANDS1, SV_COMMANDS2 };

	nb = 0;
	if ((cmds = sv_split(&cl->wr)) == NULL)
		return (sv_error("Server: split failed.", e));
	dup = ft_strdup(cmds[0]);
	ft_strtoupper(dup);
	while (com[nb].name && ft_strcmp(com[nb].name, dup))
		nb++;
	if (cl->reg.registered <= 0 && LOCK_SERVER)
		sv_get_cl_password(cl, e);
	else if (cl->reg.registered > 0 || nb == 7 || nb == 10)
		com[nb].fct(cmds, e, cl);
	else if (cl->reg.registered == 0)
	{
		send(cl->fd, e->name, SERVER_LEN, 0);
		send(cl->fd, " 451 * :You have not registered", 31, 0);
		send(cl->fd, END_CHECK, END_CHECK_LEN, 0);
		cl->reg.registered = -1;
	}
	free(dup);
	ft_free(&cmds);
}

static void			sv_clean_buf(t_buf *wr)
{
	while (wr->head && (*wr->head == ' ' || *wr->head == '\t'))
	{
		wr->head++;
		if (wr->head >= wr->end)
			wr->head = wr->start;
	}
}

void				sv_cl_write(t_env *e, t_fd *cl)
{
	if (cl->wr.tail && *cl->wr.tail == '\n')
	{
		sv_clean_buf(&cl->wr);
		sv_cmd_client(e, cl);
/* 			else if (cl->wr.head != cl->wr.tail && cl->chan) */
/* 				sv_sendto_chan(cl); */
/* 			} */
		if (cl->wr.tail)
			*cl->wr.tail = '\0';
		cl->wr.head = cl->wr.tail;
		if (e->verb)
			sv_aff_wr(cl);
	}
}
