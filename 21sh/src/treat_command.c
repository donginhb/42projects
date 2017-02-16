/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/01 23:39:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/16 18:19:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
** "sf"		If cursor at the end of the window, scrolls the screen one line up.
			Else, move the cursor one line down.
** "cr"		Move the cursor to the beginning of the line it is on.
*/

void			treat_command(t_env *e)
{
	char		*cmd;
	char		**args;

	rewrite_command(e);
	tputs(ft_tgetstr("sf"), 1, ft_pchar);
	tputs(ft_tgetstr("cr"), 1, ft_pchar);
	if (e->quote == 0)
	{
		cmd = expansions_check(e);
		args = split_command(cmd);
		if (cmd)
			free(cmd);
		if (args == NULL)
			return (ft_putendl_fd("Parsing failed.", 2));
		restore_term();
		check_and_exec(args, &e->env);
		redefine_term();
		hist_add(e);
	}
	else
	{
		ft_insert_str(&e->hist->cmd[e->hist->cmd_len], "\n", 1);
		e->hist->cmd_len += 1;
		e->pos = e->hist->cmd_len;
		e->q_pos = e->hist->cmd_len;
	}
	prompt(e);
	cursor_position(&e->origin);
	ft_memcpy(&e->cursor, &e->origin, sizeof(e->cursor));
}
