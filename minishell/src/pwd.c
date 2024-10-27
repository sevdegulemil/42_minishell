/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:28:59 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:24:25 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	run_pwd(t_cmd *cmd, t_state *state)
{
	char	cwd[PATH_MAX];

	if ((getcwd(cwd, sizeof(cwd)) == 0) || ft_strlen(cwd) == 0)
		return (FAILURE);
	dprintln(cmd->bout, cwd);
	state->status = 0;
	return (SUCCESS);
}
