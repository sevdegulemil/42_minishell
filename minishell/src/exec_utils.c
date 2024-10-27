/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:27:37 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:19:25 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	w_exit_status(int status)
{
	return ((status >> 8) & 0x000000ff);
}

bool	token_has_cmd(t_token *token)
{
	if (!token)
		return (false);
	while (token)
	{
		if (token->type == CMD)
			return (true);
		token = token->next;
	}
	return (false);
}

int	cmd_init(t_cmd *cmd, int arr_len)
{
	int	i;

	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->in = NAFD;
	cmd->out = NAFD;
	cmd->bin = NAFD;
	cmd->bout = NAFD;
	cmd->idx = 0;
	cmd->count = 0;
	cmd->heredoc = (int *)malloc(sizeof(int) * arr_len);
	if (!cmd->heredoc)
		return (FAILURE);
	i = 0;
	while (i < arr_len)
	{
		cmd->heredoc[i] = NAFD;
		i++;
	}
	return (SUCCESS);
}

int	**pipe_fd_dispose_idx(int **pipe_fd, int i)
{
	if (!pipe_fd)
		return (NULL);
	if (i < 1)
		return (free(pipe_fd), NULL);
	i--;
	while (i >= 0)
	{
		free(pipe_fd[i]);
		i--;
	}
	free(pipe_fd);
	return (NULL);
}

int	**pipe_fd_init(int pipe_count)
{
	int	**pipe_fd;
	int	i;

	pipe_fd = (int **)malloc(sizeof(int *) * pipe_count);
	if (!pipe_fd)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
			return (pipe_fd_dispose_idx(pipe_fd, i));
		i++;
	}
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipe_fd[i]) == -1)
			return (pipe_fd_dispose_idx(pipe_fd, pipe_count - 1), NULL);
		i++;
	}
	return (pipe_fd);
}
