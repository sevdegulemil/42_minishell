/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:25:38 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:15:08 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	set_fds_1(int **fd, t_cmd *cmd, int i)
{
	if (fd)
	{
		if (i != 0)
		{
			close(fd[i - 1][1]);
			if (cmd->in != NAFD)
			{
				cmd->bin = cmd->in;
				close(fd[i - 1][0]);
			}
			else
				cmd->bin = fd[i - 1][0];
		}
		else
		{
			if (cmd->in != NAFD)
				cmd->bin = cmd->in;
			else
				cmd->bin = STDIN_FILENO;
		}
	}
	else if (cmd->in != NAFD)
		cmd->bin = cmd->in;
}

static void	set_fds_2(int **fd, t_cmd *cmd, int arr_len, int i)
{
	if (fd)
	{
		if (i != arr_len - 1)
		{
			close(fd[i][0]);
			if (cmd->out != NAFD)
			{
				cmd->bout = cmd->out;
				close(fd[i][1]);
			}
			else
				cmd->bout = fd[i][1];
		}
		else
		{
			if (cmd->out != NAFD)
				cmd->bout = cmd->out;
			else
				cmd->bout = STDOUT_FILENO;
		}
	}
	else if (cmd->out != NAFD)
		cmd->bout = cmd->out;
}

void	built_in_handle_fds(t_cmd *cmd, int **pipe_fds)
{
	if (cmd->in == NAFD)
		if (cmd->heredoc)
			cmd->in = cmd->heredoc[cmd->idx];
	set_fds_1(pipe_fds, cmd, cmd->idx);
	set_fds_2(pipe_fds, cmd, cmd->count, cmd->idx);
	if (cmd->bout == NAFD)
		cmd->bout = STDOUT_FILENO;
	if (cmd->bin == NAFD)
		cmd->bin = STDIN_FILENO;
}

int	which_command_built_in(t_state *state, t_token *token, t_cmd *cmd,
		int **pipe_fds)
{
	built_in_handle_fds(cmd, pipe_fds);
	if (ft_strncmp(token->data, "echo", 5) == 0)
		return (run_echo(state, token, cmd));
	if (ft_strncmp(token->data, "cd", 3) == 0)
		return (run_cd(state, token));
	if (ft_strncmp(token->data, "pwd", 4) == 0)
		return (run_pwd(cmd, state));
	if (ft_strncmp(token->data, "export", 7) == 0)
		return (run_export(state, token, cmd));
	if (ft_strncmp(token->data, "env", 4) == 0)
		return (run_env(state, cmd));
	if (ft_strncmp(token->data, "unset", 6) == 0)
		return (run_unset(state, token));
	if (ft_strncmp(token->data, "exit", 5) == 0)
		return (run_exit(state, token));
	return (FAILURE);
}
