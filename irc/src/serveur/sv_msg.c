/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/02 18:01:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/04/01 22:07:39 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <sys/socket.h>

static void		rpl_msg(char **cmds, t_fd *to, t_fd *cl)
{
	sv_cl_write(":", to);
	sv_cl_write(cl->reg.nick, to);
	sv_cl_write("!~", to);
	sv_cl_write(cl->reg.username, to);
	sv_cl_write("@", to);
	sv_cl_write(cl->addr, to);
	sv_cl_write(" MSG ", to);
	sv_cl_write(to->reg.nick, to);
	sv_cl_write(" :", to);
	sv_cl_write(*cmds, to);
	while (*++cmds)
	{
		sv_cl_write(" ", to);
		sv_cl_write(*cmds, to);
	}
	sv_cl_write(END_CHECK, to);
}

static void		sv_msg_client(char *nick, char **cmds, t_fd *cl, t_env *e)
{
	t_fd		*to;

	to = e->fds;
	while (to)
	{
		if (!sv_strcmp(nick, to->reg.nick))
		{
			rpl_msg(cmds, to, cl);
			if (to->reg.umode &= USR_AWAY)
				rpl_away(cl, to, e);
			return ;
		}
		to = to->next;
	}
	sv_err(ERR_NOSUCHNICK, nick, NULL, cl);
}

void			sv_msg(char **cmds, t_env *e, t_fd *cl)
{
	char		**targets;
	int			i;

	if (!cmds[0] || *cmds[0] == '\0')
		return (sv_err(ERR_NORECIPIENT, "MSG", NULL, cl));
	if (!cmds[1] || !*cmds[1])
		return (sv_err(ERR_NOTEXTTOSEND, "MSG", NULL, cl));
	if ((targets = ft_strsplit(cmds[0], ',')) == NULL)
		sv_error("ERROR: Server out of memory", e);
	i = 0;
	while (targets[i])
	{
		if (ISCHAN(*targets[i]))
			sv_msg_chan(targets[i], cmds + 1, cl);
		else
			sv_msg_client(targets[i], cmds + 1, cl, e);
		i++;
	}
	ft_free(&targets);
}