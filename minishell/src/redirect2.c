/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:32:58 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:27:58 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	set_other_redirect(t_token *token, t_cmd *cmd, t_state *state)
{
	bool	has_last_heredoc;
	t_token	*temp;

	if (!token)
		return (FAILURE);
	has_last_heredoc = false;
	temp = token;
	while (temp)
	{
		if (temp->type == RED_HEREDOC)
			has_last_heredoc = true;
		else if (temp->type == RED_INPUT)
			has_last_heredoc = false;
		temp = temp->next;
	}
	if (handle_fds(token, cmd, state, has_last_heredoc) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}

int	handle_fds(t_token *token, t_cmd *cmd, t_state *state,
		bool has_last_heredoc)
{
	while (token)
	{
		if (token->type == RED_INPUT)
		{
			if (handle_red_input(token, cmd, has_last_heredoc,
					state) != SUCCESS)
				return (FAILURE);
		}
		else if (token->type == RED_OUTPUT)
		{
			if (handle_red_output(token, cmd, state) != SUCCESS)
				return (FAILURE);
		}
		else if (token->type == RED_APPEND)
		{
			if (handle_red_append(token, cmd, state) != SUCCESS)
				return (FAILURE);
		}
		token = token->next;
		if (!token)
			break ;
	}
	return (SUCCESS);
}

int	set_heredoc(t_token *token, t_cmd *cmd, int i)
{
	if (!token)
		return (FAILURE);
	while (token)
	{
		if (token->type == RED_HEREDOC)
			if (handle_red_heredoc(token, cmd, i) != SUCCESS)
				return (FAILURE);
		token = token->next;
	}
	return (SUCCESS);
}
