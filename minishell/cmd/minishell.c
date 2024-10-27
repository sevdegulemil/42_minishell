/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:24:19 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:13:35 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int				g_sig = 0;

static void	dispose_env_idx(char **copy_env, int i)
{
	if (!copy_env)
		return ;
	if (i > 0)
		while (i--)
			free(copy_env[i]);
	free(copy_env);
}

char	**copy_env(char **env)
{
	char	**env_copy;
	int		i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
		i++;
	env_copy = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
			return (dispose_env_idx(env_copy, i), NULL);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

static t_state	*state_init(char **argv, char **env)
{
	t_state	*state;

	state = malloc(sizeof(t_state));
	if (!state)
		return (NULL);
	state->env = copy_env(env);
	if (!state->env)
		return (state_dispose(&state), NULL);
	state->argv = argv;
	state->token_arr = NULL;
	state->prompt = NULL;
	state->status = 0;
	state->cmd_ct = 0;
	state->err = 0;
	return (state);
}

static void	shell_routine(t_state *state)
{
	while (true)
	{
		state->prompt = readline(COLOR PROMPT COLOR_RESET);
		if (!state->prompt)
			break ;
		add_history(state->prompt);
		state->err = syntax_check(state);
		if (state->err)
		{
			print_syntax_err(state->err, state);
			dispose_prompt(state);
			continue ;
		}
		state->err = SUCCESS;
		state->token_arr = run_lexer(state);
		if (!state->token_arr)
		{
			free(state->prompt);
			continue ;
		}
		exec_start(state);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_state	*state;

	handle_signals();
	state = state_init(argv, env);
	if (!state)
		return (argc);
	shell_routine(state);
	return (state_dispose(&state), state->status);
}
