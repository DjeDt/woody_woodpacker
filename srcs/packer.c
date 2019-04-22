/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut.student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 16:52:01 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 16:28:40 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

static bool	start(t_packer *pack, const char *filename)
{
	struct stat st;

	if ((pack->fd = open(filename, O_RDONLY)) < 0)
	{
		perror("open:");
		return (false);
	}

	if (fstat(pack->fd, &st) != 0)
    {
		perror("fstat:");
		close(pack->fd);
		return (false);
    }

	if ((pack->map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, pack->fd, 0)) == MAP_FAILED)
	{
		perror("mmap:");
		close(pack->fd);
		return (false);
	}
	pack->size = st.st_size;
	generate_key(pack->key);
	return (true);
}

static int	release(t_packer *pack)
{
	int ret;

	ret = close(pack->fd);
	if (ret == -1)
		perror("close: ");
	ret = munmap(pack->map, pack->size);
	if (ret == -1)
		perror("munmap: ");
	return (ret);
}

int		main(int ac, char **av)
{
	int			ret = ERROR;
	t_packer	pack = {0};

	if (ac != 2)
	{
		printf("Wrong argument number.\n\tUsage : %s <target> <key>\n", av[0]);
		return (-1);
	}
	if (start(&pack, av[1]) == true)
	{
		if (criteria(&pack, av[1]) == true)
		{
			ret = infect_x64(&pack);
			release(&pack);
		}
	}
	return (ret);
}
