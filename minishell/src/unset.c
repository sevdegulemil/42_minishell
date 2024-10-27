/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:39:39 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:31:55 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static bool	validate_unset_arg(t_state *state, t_token *arg);

int	run_unset(t_state *state, t_token *token)
{
	if (!token || !state)
		return (FAILURE);
	if (!token->next)
	{
		state->status = 0;
		return (SUCCESS);
	}
	token = token->next;
	while (token)
	{
		if (validate_unset_arg(state, token) == false)
			return (FAILURE);
		state->env = str_arr_remove(state->env, token->data);
		if (!state->env)
			return (FAILURE);
		token = token->next;
	}
	state->status = 0;
	return (SUCCESS);
}

static bool	validate_unset_arg(t_state *state, t_token *arg)
{
	int	i;

	if (!arg || !state)
		return (false);
	while (arg)
	{
		if (!is_al_underscore(arg->data[0]))
			return (print_execute_err(state, arg, 1,
					ERRP_NOT_A_VALID_IDENTIFIER), false);
		i = 0;
		while (arg->data[i])
		{
			if (!is_alnum_underscore(arg->data[i]))
				return (print_execute_err(state, arg, 1,
						ERRP_NOT_A_VALID_IDENTIFIER), false);
			i++;
		}
		arg = arg->next;
	}
	return (true);
}

static char	**allocate_new_str(char **str_arr)
{
	char	**new_str_arr;
	int		i;

	i = 0;
	while (str_arr[i])
		i++;
	new_str_arr = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_str_arr)
		return (NULL);
	return (new_str_arr);
}

char	**str_arr_remove(char **str_arr, char *key)
{
	char	**new_str_arr;
	int		i;
	int		j;

	if (!str_arr || !key)
		return (NULL);
	new_str_arr = allocate_new_str(str_arr);
	i = 0;
	j = 0;
	while (str_arr[i])
	{
		if (str_arr[i] && key && ft_strncmp(str_arr[i], key,
				ft_strlen(key)) == 0 && str_arr[i][ft_strlen(key)] == '=')
		{
			free(str_arr[i]);
			str_arr[i] = NULL;
		}
		else
			new_str_arr[j++] = str_arr[i];
		i++;
	}
	new_str_arr[j] = NULL;
	return (free(str_arr), new_str_arr);
}
