/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   criteria.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 19:43:46 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 16:27:07 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

static bool is_executable(Elf64_Ehdr *header)
{
	return (header->e_entry != 0);
}

static bool is_elf(Elf64_Ehdr *header)
{
	return (*(uint32_t *)header == ELF_MAGIC_NUMBER);
}

static bool is_x64(Elf64_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS64);
}

static bool is_x86(Elf64_Ehdr *header)
{
	return (header->e_machine == EM_X86_64);
}

static bool	is_packed(Elf64_Ehdr *header)
{
	return ((*(uint32_t*)&header->e_ident[EI_PAD] == SIGNATURE));
}

static bool is_corrupted(t_packer *pack, Elf64_Ehdr *header)
{
	return (header->e_shoff + (header->e_shnum * sizeof(Elf64_Shdr)) != pack->size);
}

bool	criteria(t_packer *pack, const char *filename)
{
	Elf64_Ehdr *header;

	header = (Elf64_Ehdr*)pack->map;
	if (is_executable(header) == false)
	{
		dprintf(STDERR_FILENO, "'%s' is not an executable.\n", filename);
		return (false);
	}
	if (is_elf(header) == false)
	{
		dprintf(STDERR_FILENO, "'%s' is not an elf file.\n", filename);
		return (false);
	}
	if (is_x64(header) == false || is_x86(header) == false)
	{
		dprintf(STDERR_FILENO, "'%s' is not an x86_64 file.\n", filename);
		return (false);
	}
	if (is_packed(header) == true)
	{
		dprintf(STDERR_FILENO, "'%s' is already packed.\n", filename);
		return (false);
	}
	if (is_corrupted(pack, header) == true)
	{
		dprintf(STDERR_FILENO, "'%s' is corrupted.\n", filename);
		return (false);
	}
	return (true);
}
