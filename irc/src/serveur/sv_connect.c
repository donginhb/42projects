/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/15 00:30:13 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/22 19:55:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

/*
** CONNECT is not implemented in this version (1.0)
*/

void			sv_connect(char **cmds, t_env *e, t_fd *cl)
{
	(void)e;
	if (!cmds[0] || !*cmds[0] || !cmds[1] || !*cmds[1])
		return (sv_err(ERR_NEEDMOREPARAMS, "CONNECT", NULL, cl));
	if (cl->reg.umode & USR_OP || cl->reg.umode & USR_LOCALOP)
		return ;
	sv_err(ERR_NOPRIVILEGES, "CONNECT", NULL, cl);
}