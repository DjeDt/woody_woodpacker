/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c	                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut.student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 11:56:51 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 16:17:23 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

static bool	is_entry_segment(Elf64_Ehdr *header, Elf64_Phdr *segment)
{
	if (header->e_entry < segment->p_vaddr || header->e_entry > segment->p_vaddr + segment->p_filesz)
		return false;
	return (true);
}

static bool	is_entry_section(t_bdata *bdata, Elf64_Shdr *section)
{
	return ((bdata->header->e_entry >= section->sh_addr) && (bdata->header->e_entry < section->sh_addr + section->sh_size));
}

static bool	is_last_section(t_bdata *bdata, Elf64_Shdr *section)
{
	return ((section->sh_offset + section->sh_size) == (bdata->segment.p_offset + bdata->segment.p_filesz));
}

static bool		modify_segment(t_packer *pack, t_bdata *bdata)
{
	bool	inc = false;
	Elf64_Phdr	*seg;

	inc = 0;
	for (int i = 0 ; i < bdata->header->e_phnum ; i++)
	{
		if (!(seg = next_segment_x64(pack, bdata->header, i)))
			return (false);

		if (inc == true)
		{
			seg->p_flags |= PF_W;
			seg->p_offset += PAGESIZE;
		}
		if (is_entry_segment(bdata->header, seg) == true)
		{
			bdata->segment.p_offset = seg->p_offset;
			bdata->segment.p_vaddr = seg->p_vaddr;
			bdata->segment.p_filesz = seg->p_filesz;


			seg->p_flags |= PF_W;
			seg->p_filesz += PAYLOAD_SIZE;
			seg->p_memsz += PAYLOAD_SIZE;

			inc = true;

		}
	}
	return (true);
}

static bool		modify_section(t_packer *pack, t_bdata *bdata)
{
	bool	inc = false;
	Elf64_Shdr	*sec;

	for (int i = 0; i < bdata->header->e_shnum; i++)
	{
		if (!(sec = next_section_x64(pack, bdata->header, i)))
			return (false);

		if (inc == true)
			sec->sh_offset += PAGESIZE;

		if (is_entry_section(bdata, sec) == true)
		{
			bdata->section.sh_offset = sec->sh_offset;
			bdata->section.sh_addr = sec->sh_addr;
			bdata->section.sh_size = sec->sh_size;
		}

		if (is_last_section(bdata, sec) == true)
		{
			sec->sh_size += PAYLOAD_SIZE;
			inc = true;
		}
	}
	return (true);
}

bool		infect_x64(t_packer *pack)
{
	t_bdata		bdata;

	bdata.header = (Elf64_Ehdr*)pack->map;
	if (modify_segment(pack, &bdata) == true)
	{
		if (modify_section(pack, &bdata) == true)
		{
			if (modify_header(&bdata) == true)
			{
				create_infected(pack, &bdata);
				return (true);
			}
		}
	}
	return (false);
}
