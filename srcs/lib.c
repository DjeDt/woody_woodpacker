/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/20 20:23:29 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 13:24:09 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*s1;
	char	*s2;

	s1 = (char*)dst;
	s2 = (char*)src;
	for (register size_t i = 0 ; i < n ; i++)
		s1[i] = s2[i];
	return (s1);
}

size_t	ft_strlen(const char *src)
{
	size_t count;

	count = 0;
	while (src[count])
		count++;
	return (count);
}

void	insert_signature(t_packer *pack)
{
	*(uint32_t *)&pack->map[EI_PAD] = SIGNATURE;
}

bool	generate_key(char *key)
{
	int		fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		for (register int off = 0 ; off < KEY_SIZE ; off++)
			key[off] = 0x42;
		return (false);
	}

	if (read(fd, key, KEY_SIZE) == -1)
	{
		perror("read");
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}
