/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:27:09 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:17:06 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	print_exec_err_with_prefix(const t_token *token, int err)
{
	if (err != ENO_OTHER)
	{
		eprint(COLOR_RED PROMPT COLOR_RESET);
		eprint(token->data);
		eprint(": ");
	}
	if (err == ENO_OTHER)
		return (FAILURE);
	else if (err == ENOENT)
		return (eprint(strerror(err)), FAILURE);
	else if (err == ERR_CMD_NOT_FOUND)
		return (eprintln(ESTR_CMD_NOT_FOUND), FAILURE);
	else if (err == ERR_NOT_A_VALID_IDENTIFIER)
		return (eprintln(ESTR_NOT_A_VALID_IDENTIFIER), FAILURE);
	else if (err == ERRP_NOT_A_VALID_IDENTIFIER)
		return (eprintln(ESTR_NOT_A_VALID_IDENTIFIER), FAILURE);
	else if (err == ERR_NO_SUCH_FILE_OR_DIR)
		return (eprintln(ESTR_NO_SUCH_FILE_OR_DIR), FAILURE);
	return (SUCCESS);
}

void	print_unknown_err(t_state *state)
{
	if (state->err == FAILURE)
	{
		state->status = 1;
		eprintln(ESTR_UNKNOWN);
	}
}

int	print_execute_err(t_state *state, const t_token *token, int status, int err)
{
	state->err = HANDLED;
	state->status = status;
	if (print_exec_err_with_prefix(token, err) != SUCCESS)
		return (FAILURE);
	else if (err == ERR_FORK_ERROR)
		eprintln("fork: Resource temporarily unavailable");
	else if (err == ERR_NUMERIC_ARG_REQUIRED)
		eprintln(ESTR_NUMERIC_ARG_REQUIRED);
	else if (err == ERR_TOO_MANY_ARG)
		eprintln(ESTR_TOO_MANY_ARG);
	else if (err == ERR_PERMISSION_DENIED)
		eprintln(ESTR_PERMISSION_DENIED);
	else if (err == ERR_PERMISSION_DENIED_BROKEN_PIPE)
		eprintln(ESTR_PERMISSION_DENIED_BROKEN_PIPE);
	else if (err == ERR_NOT_VALID_IN_THIS_CTX)
		eprintln(ESTR_NOT_VALID_IN_THIS_CTX);
	else if (err == ERR_IS_DIR)
		eprintln(ESTR_IS_DIR);
	else if (err == ERR_HOME_NOT_SET)
		eprintln(ESTR_HOME_NOT_SET);
	else
		eprintln(ESTR_UNEXPECTED);
	return (FAILURE);
}

void	print_fatal_err(const char *msg, const int err)
{
	eprintln(msg);
	exit(err);
}
