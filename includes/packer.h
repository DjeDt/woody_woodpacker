/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut.student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 11:15:26 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 16:28:53 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKER_H
# define PACKER_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <unistd.h>
# include <elf.h>
# include <stdbool.h>

/*
** DEFINE
*/

# define SUCCESS 0
# define ERROR -1

# define OUTPUT_NAME		"woody"
# define KEY_SIZE			8
# define PAYLOAD_SIZE		374
# define PAGESIZE			4096
# define SIGNATURE			0x216948 // 'Hi!'
# define ELF_MAGIC_NUMBER	1179403647

/*
** STRUCTURES
*/
typedef struct	s_packer
{
	int			fd;
	size_t		size;
	char		*map;
	char		key[KEY_SIZE];
}				t_packer;

typedef struct	s_segment
{
	Elf64_Off	p_offset;
	Elf64_Addr	p_vaddr;
	uint64_t	p_filesz;
}				t_segment;

typedef struct	s_section
{
	Elf64_Off	sh_offset;
	Elf64_Addr	sh_addr;
	uint64_t	sh_size;
}				t_section;

typedef struct	s_bdata
{
	Elf64_Ehdr	*header;
	t_segment	segment;
	t_section	section;
	Elf64_Addr	original_entrypoint;
}				t_bdata;

/*
** FUNCTIONS
*/
int				packer(int ac, char **av);
bool			criteria(t_packer *pack, const char *filename);
bool			infect_x64(t_packer *pack);
void			create_infected(t_packer *pack, t_bdata *data);

bool			modify_header(t_bdata *data);
Elf64_Shdr		*next_section_x64(t_packer *pack, Elf64_Ehdr *e_hdr, size_t count);
Elf64_Phdr		*next_segment_x64(t_packer *pack, Elf64_Ehdr *e_hdr, size_t count);
void			_rc4(unsigned char *key, size_t key_len, char *data, size_t data_len);

/* lib */
void			*ft_memcpy(void *dst, const void *src, size_t n);
size_t			ft_strlen(const char *src);
void			insert_signature(t_packer *pack);
bool			generate_key(char *key);
void			encrypt_data(char *addr, size_t const size, char const *key);

#endif
