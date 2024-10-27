/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:28:45 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:24:02 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_env_path_arr_as_str(char **env)
{
	int	i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
		i++;
	}
	return (NULL);
}

char	*get_cmd_absolute_path(t_token *token, t_state *state)
{
	struct stat	buf;
	char		*new;

	stat(token->data, &buf);
	if (errno == EACCES)
		return (print_execute_err(state, token, 106, EACCES), NULL);
	if (S_ISDIR(buf.st_mode))
		return (print_execute_err(state, token, 126, ERR_IS_DIR), NULL);
	if (access(token->data, F_OK))
		return (print_execute_err(state, token, 127, ERR_NO_SUCH_FILE_OR_DIR),
			NULL);
	if (access(token->data, X_OK))
		return (print_execute_err(state, token, 126, ERR_PERMISSION_DENIED),
			NULL);
	new = ft_strdup(token->data);
	if (!new)
		return (NULL);
	return (new);
}

static char	*get_check_cmd_path(t_token *token, t_state *state, char *temp)
{
	char		*cmd_path;
	struct stat	buf;

	cmd_path = ft_strjoin(temp, token->data, true);
	if (!cmd_path)
		return (NULL);
	stat(cmd_path, &buf);
	if (S_ISDIR(buf.st_mode))
		return (free(cmd_path), print_execute_err(state, token, 127,
				ERR_IS_DIR), NULL);
	return (cmd_path);
}

static char	*get_cmd_relative_path(t_token *token, t_state *state,
		char **path_arr)
{
	char	*cmd_path;
	char	*temp;
	int		i;

	i = 0;
	if (*token->data == '\0')
		return (print_execute_err(state, token, 127, ERR_CMD_NOT_FOUND), NULL);
	while (path_arr[i])
	{
		temp = ft_strjoin(path_arr[i++], "/", false);
		if (!temp)
			return (NULL);
		while (token->type != CMD)
			token = token->next;
		cmd_path = get_check_cmd_path(token, state, temp);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
	}
	if (token_is_built_in(token))
		return (token->data);
	return (print_execute_err(state, token, 127, ERR_CMD_NOT_FOUND), NULL);
}

char	*get_cmd_path(t_token *token, t_state *state)
{
	char	*path_arr_str;
	char	**path_arr;
	char	*rsp;

	if (ft_strchr(token->data, '/'))
		return (get_cmd_absolute_path(token, state));
	if (!state->env)
		return (print_execute_err(state, token, 127, ERR_CMD_NOT_FOUND), NULL);
	path_arr_str = get_env_path_arr_as_str(state->env);
	if (!path_arr_str)
		return (NULL);
	path_arr = ft_split(path_arr_str, ':');
	if (!path_arr)
		return (NULL);
	free(path_arr_str);
	rsp = get_cmd_relative_path(token, state, path_arr);
	dispose_paths(path_arr);
	return (rsp);
}
