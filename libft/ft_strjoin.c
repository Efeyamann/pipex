/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efe <efe@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:36:04 by esir              #+#    #+#             */
/*   Updated: 2025/02/25 16:41:12 by efe              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		j;
	int		k;
	int		l;
	char	*a;

	k = ft_strlen(s1) + ft_strlen(s2) + 1;
	j = 0;
	l = 0;
	a = (char *)malloc(k * sizeof(char));
	if (a == NULL)
		return (NULL);
	while (s1[j])
	{
		a[l++] = s1[j++];
	}
	j = 0;
	while (s2[j])
	{
		a[l++] = s2[j++];
	}
	a[l] = '\0';
	return (a);
}
