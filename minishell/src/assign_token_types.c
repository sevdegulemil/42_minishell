/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:24:45 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:14:09 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	assign_token_arr_types(t_token **token_arr)
{
	int	i;

	i = 0;
	if (!token_arr || !(*token_arr))
		return ;
	while (token_arr[i])
	{
		assign_token_types(token_arr[i]);
		i++;
	}
}

static void	assign_token_types_file(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (temp->type == RED_INPUT || temp->type == RED_OUTPUT
			|| temp->type == RED_APPEND)
		{
			temp = temp->next;
			if (temp)
				temp->type = RED_FILE;
			else
				break ;
		}
		else if (temp->type == RED_HEREDOC)
		{
			temp = temp->next;
			if (temp)
				temp->type = RED_HEREDOC_TYPE;
			else
				break ;
		}
		temp = temp->next;
	}
}

void	assign_token_types(t_token *token)
{
	if (!token)
		return ;
	assign_token_types_file(token);
	while (token && token->type != NONE && token->type != ARG)
		token = token->next;
	if (token && token->type != RED_APPEND)
	{
		token->type = CMD;
		token = token->next;
	}
	while (token)
	{
		if (token->type == NONE)
			token->type = ARG;
		token = token->next;
	}
}
