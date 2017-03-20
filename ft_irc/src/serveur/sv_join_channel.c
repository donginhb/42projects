/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_join_channel.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 04:48:15 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/20 10:54:48 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <time.h>

static int		sv_channel_ident(char *name, t_chan *new, t_fd *cl, t_env *e)
{
	time_t		store;
	u_char		*ptr;
	char		*base;
	int			i[2];

	if (time(&store) == -1)
	{
		send(cl->fd, e->name, SERVER_LEN, 0);
		send(cl->fd, " ERROR ", 7, 0);
		send(cl->fd, cl->reg.nick, NICK_LEN, 0);
		send(cl->fd, " ", 1, 0);
		send(cl->fd, name, CHANNAME_LEN, 0);
		send(cl->fd, " :Server failed to create your channel", 38, 0);
		send(cl->fd, END_CHECK, END_CHECK_LEN, 0);
		free(new);
		return (0);
	}
	ptr = (u_char *)&store;
	base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	if ((i[0] = ft_strlen(name)) > CHANNAME_LEN - 5)
		i[0] = CHANNAME_LEN - 5;
	i[1] = i[0] + 5;
	while (i[0] < i[1])
		new->name[i[0]++] = base[*ptr++ % 35];
	return (1);
}

/*
** Afficher les nouveaux mode du chan si on implemente la commande MODE
*/

static t_chan	*sv_new_chan(char *name, t_fd *cl, t_env *e)
{
	t_chan		*new;

	if ((new = (t_chan*)malloc(sizeof(*new))) == NULL)
		sv_error("ERROR: SERVER: Out of memory", e);
	ft_memset(new, 0, sizeof(*new));
	ft_strncpy(new->name, name, CHANNAME_LEN);
	if (*name == '!' && !sv_channel_ident(name, new, cl, e))
		return (e->chans);
	if (!*new->name)
		ft_bzero(new->topic, TOPIC_LEN + 1);
//	new->cmode |= (*new->name == '+') ? CHFL_TOPIC : CHFL_NOMSG | CHFL_SECRET;
	new->users = sv_add_usertochan(cl, new);
	if (*new->name == '!')
		new->users->mode |= CHFL_CREATOR;
	if (*new->name != '+')
		new->users->mode |= CHFL_CHANOP;
	new->next = e->chans;
	if (e->chans)
		e->chans->prev = new;
	return (new);
}

static int		sv_check_safe_chan(char *name, t_fd *cl, t_env *e)
{
	t_chan		*ch;
	int			len;

	ch = e->chans;
	while (ch)
	{
		len = ft_strlen(ch->name);
		if (*ch->name == '!' &&
			sv_strncmp(ch->name, name, len - 5) == ch->name[len - 5])
		{
			send(cl->fd, e->name, SERVER_LEN, 0);
			send(cl->fd, " ERROR ", 7, 0);
			send(cl->fd, cl->reg.nick, NICK_LEN, 0);
			send(cl->fd, " ", 1, 0);
			send(cl->fd, name, ft_strlen(name), 0);
			send(cl->fd, " :Short name for safe channel already in use", 44, 0);
			send(cl->fd, END_CHECK, END_CHECK_LEN, 0);
			return (0);
		}
		ch = ch->next;
	}
	return (1);
}

static void		sv_check_chan_modes(char *n, t_chan *ch, char ***cmd, t_fd *cl)
{
	if (ch->cmode & CHFL_KEY && ft_strcmp((**cmd) ? *((*cmd)++) : **cmd, ch->key))
		return (sv_err(ERR_BADCHANNELKEY, ch->name, NULL, cl));
	if (ch->cmode & CHFL_LIMIT && ch->nbusers >= ch->limit)
		return (sv_err(ERR_CHANNELISFULL, ch->name, NULL, cl));
	ch->users = sv_add_usertochan(cl, ch);
	cl->chans = sv_add_chantouser(ch, cl);
	sv_who_chan(&n, cl, &e);
}

void			sv_join_chan(char *name, char ***c, t_fd *cl, t_env *e)
{
	t_chan		*chan;

	chan = e->chans;
	while (chan)
	{
		if (!sv_strcmp(name, chan->name))
			return (sv_check_chan_modes(name, chan, c, cl));
		chan = chan->next;
	}
	if (*name == '!' && !sv_check_safe_chan(name, cl, e))
		return ;
	if ((e->chans = sv_new_chan(name, cl, e)) == NULL)
		return ;
	if ((cl->chans = sv_add_chantouser(e->chans, cl)) == NULL)
		return ;
	if (*name == '!')
		cl->chans->mode |= CHFL_CREATOR;
	else if (*name != '+')
		cl->chans->mode |= CHFL_CHANOP;
	sv_who_chan(&name, cl, e);
}
