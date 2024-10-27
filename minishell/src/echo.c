/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:26:47 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:16:24 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static bool	is_echo_n_flag(const char *flag)
{
	int	i;

	if (!(flag[0] == '-' && flag[1] == 'n'))
		return (false);
	i = 2;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	get_buffer_len_init(int *j, int *len, int *newline, t_token *arg)
{
	*j = 0;
	*len = 0;
	if (arg && arg->data)
		*len = ft_strlen(arg->data);
	*newline = 1;
}

static int	get_buffer_len(t_token *arg, bool n, int *j)
{
	int	len;
	int	space;
	int	newline;

	get_buffer_len_init(j, &len, &newline, arg);
	if (n)
		newline = 0;
	space = -1;
	if (!arg)
		space = 0;
	while (arg)
	{
		if (arg->type != ARG)
		{
			arg = arg->next;
			continue ;
		}
		len += ft_strlen(arg->data);
		if (arg->next)
			space++;
		arg = arg->next;
	}
	return (len + space + newline + 2);
}

static char	*merge_args(t_token *arg, bool n)
{
	char	*buffer;
	int		i;
	int		j;

	buffer = (char *)malloc(sizeof(char) * get_buffer_len(arg, n, &j));
	if (!buffer)
		return (NULL);
	while (arg)
	{
		if (arg->type != ARG)
		{
			arg = arg->next;
			continue ;
		}
		i = 0;
		while (arg->data[i])
			buffer[j++] = arg->data[i++];
		if (arg->next)
			buffer[j++] = ' ';
		arg = arg->next;
	}
	if (!n)
		buffer[j++] = '\n';
	buffer[j] = '\0';
	return (buffer);
}

int	run_echo(t_state *state, t_token *token, t_cmd *cmd)
{
	char	*buffer;
	bool	n;

	if (!state || !token || !cmd)
		return (FAILURE);
	if (is_empty_arg(token->next))
	{
		state->status = 0;
		write(1, "\n", 1);
		return (dprint(cmd->out, "\n"), SUCCESS);
	}
	token = token->next;
	n = is_echo_n_flag(token->data);
	if (n)
		token = token->next;
	while (token && is_echo_n_flag(token->data))
		token = token->next;
	buffer = merge_args(token, n);
	if (!buffer)
		return (FAILURE);
	dprint(cmd->bout, buffer);
	state->status = 0;
	return (free(buffer), SUCCESS);
}
