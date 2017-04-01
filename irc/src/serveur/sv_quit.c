/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_quit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/06 22:03:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/04/01 22:16:06 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <sys/socket.h>

static void		rpl_quit(char **cmds, t_chan *ch, t_fd *to, t_fd *cl)
{
	if (ch && ch->cmode & CHFL_ANON)
	{
		sv_cl_write(":anonymous!~anonymous@anonymous LEAVE ", to);
		sv_cl_write(ch->name, to);
		sv_cl_write(END_CHECK, to);
		return ;
	}
	sv_cl_write(":", to);
	sv_cl_write(cl->reg.nick, to);
	sv_cl_write("!~", to);
	sv_cl_write(cl->reg.username, to);
	sv_cl_write("@", to);
	sv_cl_write(cl->addr, to);
	sv_cl_write(" QUIT :", to);
	if (cmds == NULL || !*cmds)
		sv_cl_write("Client Quit", to);
	while (cmds && *cmds)
	{
		sv_cl_write(*cmds, to);
		if (*++cmds)
			sv_cl_write(" ", to);
	}
	sv_cl_write(END_CHECK, to);
}

void			sv_quit(char **cmds, t_env *e, t_fd *cl)
{
	t_listin	*ch;
	t_listin	*us;

	ch = cl->chans;
	while (ch)
	{
		us = ((t_chan *)ch->is)->users;
		while (us)
		{
			if (((t_fd *)us->is)->fd != cl->fd)
				rpl_quit(cmds, ch->is, us->is, cl);
			us = us->next;
		}
		ch = ch->next;
	}
	if (*cl->reg.username && cl->reg.password && *cl->reg.nick)
	{
		add_in_userslist(e->users, cl);
		add_in_users(e->users, cl);
	}
	cl->leaved = 1;
	cl->reason = "Client Quit";
}