/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:26:07 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:15:34 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	state_dispose(t_state **state)
{
	if (!state || !*state)
		return ;
	free((*state)->prompt);
	(*state)->prompt = NULL;
	token_arr_dispose(&(*state)->token_arr);
}

void	dispose_prompt(t_state *state)
{
	if (state)
	{
		free(state->prompt);
		state->prompt = NULL;
	}
	if (state->token_arr)
		token_arr_dispose(&state->token_arr);
}

void	state_dispose_single(t_state **state)
{
	int	i;

	if (!state || !*state)
		return ;
	i = 0;
	while ((*state)->env && (*state)->env[i])
		free((*state)->env[i++]);
	free((*state)->env);
	(*state)->env = NULL;
	free((*state)->prompt);
	(*state)->prompt = NULL;
}

void	dispose_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
