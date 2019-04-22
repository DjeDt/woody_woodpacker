/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_segment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut.student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 17:39:12 by ddinaut           #+#    #+#             */
/*   Updated: 2019/01/20 20:46:09 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

Elf32_Phdr	*next_segment_x32(t_packer *pack, Elf32_Ehdr *e_hdr, size_t count)
{
	Elf32_Phdr *ret;

	ret = (Elf32_Phdr*)((char*)pack->map + (e_hdr->e_phoff + sizeof(Elf32_Phdr) * count));
	if (!ret || ((void*)ret >= (void*)pack->map + pack->size))
	{
		printf("Error: maformated file\n");
		return (NULL);
	}
	return (ret);
}

Elf64_Phdr	*next_segment_x64(t_packer *pack, Elf64_Ehdr *e_hdr, size_t count)
{
	Elf64_Phdr *ret;

	ret = (Elf64_Phdr*)((char*)pack->map + (e_hdr->e_phoff + sizeof(Elf64_Phdr) * count));
	if (!ret || ((void*)ret >= (void*)pack->map + pack->size))
	{
		printf("Error: maformated file\n");
		return (NULL);
	}
	return (ret);
}
