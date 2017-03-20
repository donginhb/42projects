/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/06 17:26:15 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/20 10:48:01 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static int		inchannel(char *name, t_fd *cl)
{
	t_listin	*list;

	list = cl->chans;
	while (list)
	{
		if (!sv_strcmp(((t_chan *)list->is)->name, name))
			return (1);
		list = list->next;
	}
	return (0);
}

static void		sv_leaveall_channels(t_fd *cl, t_env *e)
{
	t_listin	*ch;
	char		*cmd[3];

	ch = cl->chans;
	cmd[2] = NULL;
	while (ch)
	{
		cmd[1] = ((t_chan *)ch->is)->name;
		sv_leave(cmd, e, cl);
		ch = cl->chans;
	}
}

/*
** Si on implemente /NAMES, l'appeler au lieu de /WHO.
*/

static void		sv_check_channels(char **cmds, t_env *e, t_fd *cl)
{
	char		**chans;
	int			i;

	if ((chans = ft_strsplit(cmds[0], ',')) == NULL)
		sv_error("ERROR: SERVER: out of memory", e);
	i = 0;
	if ((!ft_strcmp("0", chans[i]) && ++i) ||
		!ft_strcmp("0", cmds[ft_tablen(cmds) - 1]))
		sv_leaveall_channels(cl, e);
	cmds++;
	while (chans[i])
	{
		if (!ISCHAN(*chans[i]) || !chans[i][1])
			sv_err(ERR_NOSUCHCHANNEL, chans[i], NULL, cl);
		else if (ft_strchr(chans[i], 7))
			sv_err(ERR_ILLEGALNAME, chans[i], NULL, cl);
		else if (!inchannel(chans[i], cl))
			sv_join_chan(chans[i], &cmds, cl, e);
		i++;
	}
	ft_free(&chans);
}

void			sv_join(char **cmds, t_env *e, t_fd *cl)
{
	if (!cmds[1] || *cmds[1] == '\0')
		return (sv_err(ERR_NEEDMOREPARAMS, "JOIN", NULL, cl));
	sv_check_channels(cmds + 1, e, cl);
}
