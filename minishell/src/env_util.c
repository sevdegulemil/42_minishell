/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:26:56 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:16:38 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_empty_arg(t_token *arg)
{
	while (arg)
	{
		if (arg->type == ARG)
			return (false);
		arg = arg->next;
	}
	return (true);
}

int	env_len(t_state *state)
{
	int	i;

	i = 0;
	if (!state)
		return (0);
	while (state->env[i])
		i++;
	return (i);
}
