/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:37:33 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:30:30 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*token_get_root(t_token *node)
{
	if (!node)
		return (NULL);
	while (node && node->prev)
		node = node->prev;
	return (node);
}

t_token	*token_get_last(t_token *node)
{
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}

bool	token_is_just_meta(t_token **token)
{
	if (!token && !*token && !(*token)->data)
		return (false);
	if ((*token)->data[0] == '>' && (*token)->data[1] == '>'
		&& (*token)->data[2] == '\0')
		(*token)->type = RED_APPEND;
	else if ((*token)->data[0] == '<' && (*token)->data[1] == '<'
		&& (*token)->data[2] == '\0')
		(*token)->type = RED_HEREDOC;
	else if ((*token)->data[0] == '|' && (*token)->data[1] == '\0')
		(*token)->type = PIPE;
	else if ((*token)->data[0] == '<' && (*token)->data[1] == '\0')
		(*token)->type = RED_INPUT;
	else if ((*token)->data[0] == '>' && (*token)->data[1] == '\0')
		(*token)->type = RED_OUTPUT;
	else
		return (false);
	return (true);
}

void	token_old_del(t_token **temp, t_token *root)
{
	t_token	*old_node;

	if (!temp || !*temp || !root)
		return ;
	old_node = *temp;
	if ((*temp)->prev)
		(*temp)->prev->next = (*temp)->next;
	if ((*temp)->next)
		(*temp)->next->prev = (*temp)->prev;
	*temp = (*temp)->next;
	token_dispose(&old_node);
}
