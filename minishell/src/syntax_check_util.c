/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:36:40 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:28:26 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	syntax_squote(t_syntax *syntax)
{
	if (syntax->duplex == 1)
	{
		syntax->duplex = 0;
		syntax->zero_pipe = 0;
		if (syntax->simplex)
			syntax->simplex = 0;
	}
	else if (syntax->duplex == 0)
		syntax->duplex = 1;
}

void	syntax_dquote(t_syntax *syntax)
{
	if (syntax->duplex == 2)
	{
		syntax->duplex = 0;
		syntax->zero_pipe = 0;
		if (syntax->simplex)
			syntax->simplex = 0;
	}
	else if (syntax->duplex == 0)
		syntax->duplex = 2;
}

int	syntax_pipe(t_state *shell, t_syntax *syntax, int *i)
{
	shell->cmd_ct++;
	if (!syntax->zero_pipe)
		syntax->zero_pipe = 1;
	else
		return (2);
	if (syntax->simplex)
		return (2);
	++(*i);
	return (0);
}

int	syntax_sarrow(t_syntax *syntax, int *i)
{
	if (!syntax->simplex)
		syntax->simplex = 1;
	else
		return (2);
	++*i;
	return (0);
}

int	syntax_darrow(t_syntax *syntax, int *i)
{
	if (!syntax->simplex)
		syntax->simplex = 2;
	else
		return (2);
	*i += 2;
	return (0);
}
