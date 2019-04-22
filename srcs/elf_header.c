/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 12:40:26 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 16:16:22 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packer.h"

bool	modify_header(t_bdata *data)
{
	// insert signature
	*(uint32_t *)&data->header->e_ident[EI_PAD] = SIGNATURE;

   	data->header->e_shoff += PAGESIZE;
	data->original_entrypoint = data->header->e_entry;
	data->header->e_entry = data->segment.p_vaddr + data->segment.p_filesz;
	return (true);
}
