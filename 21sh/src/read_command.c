/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/25 03:37:30 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/24 21:56:01 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
** "cd"		Clear screen from the cursor position adn all the lines below.
*/

void			check_cmd(char buf, t_env *e)
{
	if ((buf == '\'' || buf == '"'))
	{
		if (e->quote == 0)
			e->quote = buf;
		else if (e->quote == buf)
			e->quote = 0;
	}
	ft_insert_str(&e->hist->cmd[e->pos], &buf, 1);
	tputs(ft_tgetstr("cd"), 1, ft_pchar);
	ft_putstr_fd(&e->hist->cmd[e->pos], e->fd);
	e->hist->cmd_len++;
	ft_pos(1, e);
}

void			check_cmd_len(int len, t_env *e)
{
	if (e->hist->cmd_len + len >= e->hist->cmd_size)
	{
		while (e->hist->cmd_len + len >= e->hist->cmd_size)
			e->hist->cmd_size += CMD_SIZE;
		if (!(e->hist->cmd = ft_realloc(e->hist->cmd, e->hist->cmd_size)))
			ft_exit_all("Malloc failed.");
	}
	else if (e->hist->cmd_len + len < e->hist->cmd_size - CMD_SIZE)
	{
		while (e->hist->cmd_len + len < e->hist->cmd_size - CMD_SIZE)
			e->hist->cmd_size -= CMD_SIZE;
		if (!(e->hist->cmd = ft_realloc(e->hist->cmd, e->hist->cmd_size)))
			ft_exit_all("Malloc failed.");
	}
}

void			read_command(int len, char *buf, t_env *e)
{
	int			i;

	i = 0;
	check_cmd_len(len, e);
	while (i < len)
	{
		if (buf[i] == ENTER)
			treat_command(e);
		else
			check_cmd(buf[i], e);
		i++;
	}
}
