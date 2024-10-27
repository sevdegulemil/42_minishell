/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyetis <eyetis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:28:53 by seemil            #+#    #+#             */
/*   Updated: 2024/09/29 13:24:16 by eyetis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	dprint(int fd, const char *s)
{
	write(fd, s, ft_strlen(s));
}

void	dprintln(int fd, const char *s)
{
	dprint(fd, s);
	dprint(fd, ENDL);
}

void	eprint(const char *str)
{
	dprint(STDERR_FILENO, str);
}

void	eprintln(const char *str)
{
	dprintln(STDERR_FILENO, str);
}
