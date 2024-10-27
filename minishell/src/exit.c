/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:27:58 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:20:11 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static bool	ft_is_digit(char *c)
{
	int	i;

	i = 0;
	if (c[i] == '-' || c[i] == '+')
		i++;
	while (c[i])
	{
		if (!(c[i] >= '0' && c[i] <= '9'))
			return (false);
		i++;
	}
	return (true);
}

static int	exit_many_argument(t_state *state, t_token *token)
{
	print_execute_err(state, token, 1, ERR_TOO_MANY_ARG);
	return (exit(0), SUCCESS);
}

static int	exit_single_argument(t_state *state, t_token *token, int *exit_code)
{
	if (ft_is_digit(token->next->data))
		*exit_code = ft_atoi(token->next->data);
	else
		return (print_execute_err(state, token, 255, ERR_NUMERIC_ARG_REQUIRED),
			exit(255), FAILURE);
	if (*exit_code < 0)
		*exit_code = *exit_code + 256;
	return (exit(*exit_code), SUCCESS);
}

int	run_exit(t_state *state, t_token *token)
{
	int	exit_code;

	exit_code = 0;
	if (token && token->next && token->next->next)
		return (exit_many_argument(state, token));
	if (token && token->next && !token->next->next)
		return (exit_single_argument(state, token, &exit_code));
	return (exit(exit_code), SUCCESS);
}
