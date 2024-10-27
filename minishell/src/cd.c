/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:25:47 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:15:19 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	env_set_oldpwd(t_state *state, char *path)
{
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", path, 0);
	if (!oldpwd)
		return (FAILURE);
	if (!state)
		return (FAILURE);
	if (env_set_value(state, oldpwd) != SUCCESS)
		return (free(oldpwd), FAILURE);
	free(oldpwd);
	return (SUCCESS);
}

static int	old_path_util(t_state *state, char *old_path)
{
	if (env_set_oldpwd(state, old_path) != SUCCESS)
		return (FAILURE);
	else
		free(old_path);
	return (SUCCESS);
}

static int	home_set(t_state *state, char *home, t_token *token)
{
	if (!token->next)
	{
		if (!home)
			return (print_execute_err(state, token, 1, ERR_HOME_NOT_SET));
		if (chdir(home) == -1)
			return (print_execute_err(state, token, 1, ERR_CANT_CHANGE_DIR));
		if (env_set_pwd(state) != SUCCESS)
			return (FAILURE);
		state->status = 0;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	run_cd(t_state *state, t_token *token)
{
	char	*home;
	char	*old_path;

	old_path = getcwd(NULL, 256);
	if (!old_path)
		return (print_execute_err(state, token, 1, ERR_OLDPWD_NOT_SET));
	if (!token)
		return (FAILURE);
	home = get_env_value(state, "HOME");
	if (home_set(state, home, token) != SUCCESS)
		return (free(old_path), FAILURE);
	if (chdir(token->next->data) == -1)
		return (free(old_path), print_execute_err(state, token, 1,
				ERR_NO_SUCH_FILE_OR_DIR));
	if (env_set_pwd(state) != SUCCESS)
		return (FAILURE);
	if (old_path_util(state, old_path) != SUCCESS)
		return (free(old_path), FAILURE);
	state->status = 0;
	return (SUCCESS);
}
