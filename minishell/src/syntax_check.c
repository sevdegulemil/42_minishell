/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:36:56 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:29:34 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_space(char character)
{
	return (' ' == character || '\t' == character || '\v' == character
		|| '\f' == character || '\r' == character);
}

void	syntax_other(t_state *shell, t_syntax *syntax, int *i)
{
	if (is_space(shell->prompt[*i]))
		++*i;
	else
		syntax->zero_pipe = (syntax->simplex = (++*i, 0));
}

int	choose(t_state *shell, t_syntax *syntax, int *i)
{
	(void)(((shell->prompt[*i] == '\'') && (syntax_squote(syntax), 1))
				|| ((shell->prompt[*i] == '"') && (syntax_dquote(syntax), 1)));
	if (syntax->duplex)
		return ((*i)++, 1);
	if ((shell->prompt[*i] == '>' && shell->prompt[(*i) + 1] != '>')
		|| (shell->prompt[*i] == '<' && shell->prompt[(*i) + 1] != '<'))
	{
		if (syntax_sarrow(syntax, i))
			return (2);
	}
	else if ((shell->prompt[*i] == '>' && shell->prompt[(*i) + 1] == '>')
		|| (shell->prompt[*i] == '<' && shell->prompt[(*i) + 1] == '<'))
	{
		if (syntax_darrow(syntax, i))
			return (2);
	}
	else if (shell->prompt[*i] == '|')
	{
		if (syntax_pipe(shell, syntax, i))
			return (2);
	}
	else
		syntax_other(shell, syntax, i);
	return (0);
}

int	syntax_check(t_state *shell)
{
	int			i;
	t_syntax	syntax;
	int			result;

	syntax.undefined = 0;
	syntax.zero_pipe = 1;
	syntax.duplex = 0;
	syntax.simplex = 0;
	i = 0;
	while (is_space(shell->prompt[i]))
		i++;
	if (shell->prompt[i] == '\0')
		return (0);
	while (shell->prompt[i])
	{
		result = choose(shell, &syntax, &i);
		if (result == 2)
			break ;
	}
	return ((syntax.duplex << 0) | (syntax.simplex << 8)
		| (syntax.zero_pipe << 16) | (syntax.undefined << 24));
}

void	print_syntax_err(int errs, t_state *state)
{
	if (errs & 0xff000000)
	{
		eprintln(ESTR_SYN_UNKNOWN_ERR);
		state->status = 1;
	}
	else if (errs & 0x00ff0000)
	{
		eprintln(ESTR_SYN_ZERO_PIPE);
		state->status = 258;
	}
	else if (errs & 0x0000ff00)
	{
		eprintln(ESTR_SYN_EMPTY_AFTER);
		state->status = 258;
	}
	else if (errs & 0x000000ff)
	{
		eprintln(ESTR_SYN_MISS_QUOTE);
		state->status = 1;
	}
}
