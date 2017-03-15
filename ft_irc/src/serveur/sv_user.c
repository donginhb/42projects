/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_user.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 04:40:46 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/14 22:34:28 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static void			already_registered(t_fd *cl, t_env *e)
{
	send(cl->fd, e->name, SERVER_LEN, 0);
	send(cl->fd, " 462 ", 5, 0);
	send(cl->fd, cl->reg.nick, NICK_LEN, 0);
	send(cl->fd, " :You may not reregister", 24, 0);
	send(cl->fd, END_CHECK, END_CHECK_LEN, 0);
}

static void			missing_parameters(t_fd *cl, t_env *e)
{
	send(cl->fd, e->name, SERVER_LEN, 0);
	send(cl->fd, " 461 * USER :Not enough parameters", 34, 0);
	send(cl->fd, END_CHECK, END_CHECK_LEN, 0);
}

void				sv_user(char **cmds, t_env *e, t_fd *cl)
{
	if (cl->reg.registered > 0)
		return (already_registered(cl, e));
	if (ft_tablen(cmds) < 5)
		return (missing_parameters(cl, e));
	ft_strncpy(cl->reg.username, cmds[1], USERNAME_LEN);
	cl->reg.umode = ft_atoi(cmds[2]);
	cl->reg.realname = ft_tabdup(&cmds[4]);
	if (cl->reg.registered == 0 && *cl->reg.nick)
		sv_welcome(e, cl);
}
