/*
 * Linux parameters setting code
 *
 * - Parse a command line linux_prep section
 *
 * - Detect a magic key combination to choice a proper
 *   command line
 *
 * - Generate a list of ARM tags handled by Linux kernel
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include <setup.h>
#include <keys.h>
#include <stmp3xxx.h>
#include <arch/platform.h>

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */



static struct tag *params;
static char *cmdlines[MAGIC_KEY_NR];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])
#define isspace(c)	((__ismask(c)&(_S)) != 0)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;



/* minimal implementation of string functions */
static char *strstr(const char *s1, const char *s2)
{
	int i;

	if (*s1 == '\0')
		return *s2 ? 0 : (char *)s1;

	while (*s1) {
		for (i = 0; ; i++) {
			if (s2[i] == '\0')
				return (char *)s1;
			if (s2[i] != s1[i])
				break;
		}
		s1++;
	}
	return 0;
}

static int strlen(const char *s)
{
	const char *start = s;

	while (*s)
		s++;

	return s - start;
}

static char *strcpy(char *s1, const char *s2)
{
	char *s = s1;

	while ((*s1++ = *s2++) != '\0')
		;

	return s;
}

static void *memcpy(void *s1, const void *s2, int n)
{
	char *dst = s1;
	const char *src = s2;

	while (n-- > 0)
		*dst++ = *src++;

	return s1;
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res = 0;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}
	return __res;
}



//modified by luozhizhuo
/**
 * skip_spaces - Removes leading whitespace from @str.
 * @str: The string to be stripped.
 *
 * Returns a pointer to the first non-whitespace character in @str.
 */
 
char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}

/* You can use " around spaces, but can't escape ". */
/* Hyphens and underscores equivalent in parameter names. */
static char *next_arg(char *args, char **param, char **val,int  *index)
{
	unsigned int i, equals = 0;
	int in_quote = 0, quoted = 0;
	char *next,*temp;

	if (*args == '"') {
		args++;
		in_quote = 1;
		quoted = 1;
	}

	for (i = 0; args[i]; i++) {
		if (isspace(args[i]) && !in_quote)
			break;
		if (equals == 0) {
			if (args[i] == '=')
				equals = i;
		}
		if (args[i] == '"')
			in_quote = !in_quote;
	}

	*param = args;
	if (!equals)
		*val = (void*)0;
	else {
		args[equals] = '\0';
		*val = args + equals + 1;

		/* Don't include quotes in value. */
		if (**val == '"') {
			(*val)++;
			if (args[i-1] == '"')
				args[i-1] = '\0';
		}
		if (quoted && args[i-1] == '"')
			args[i-1] = '\0';
	}
	if (args[i]) {
		args[i] = '\0';
		next = args + i + 1;
		*index += i + 1;
	} else{
		next = args + i;
		*index += i;
	}	
	/* Chew up trailing spaces. */
	temp = skip_spaces(next);
	*index += (temp - next);//skip spaces and then fix index 
	return temp;
}

int parse_args(char *args , char *memparam)
{
	char *param, *val;
	char tembuf[256];
	char *ptembuf;
	int pre = 0,next = 0;
	
	int length1 = strlen("mem=") ;
	int length2 = strlen(memparam);
	int lengtotal = strlen(args);

	ptembuf = tembuf;

	memcpy(ptembuf,args,lengtotal + 1);
	ptembuf = skip_spaces(ptembuf);	
	while (*ptembuf) {
		ptembuf = next_arg(ptembuf, &param, &val,&next);
		if (strcmp(param,"mem") == 0){
			if (strcmp(val,memparam) == 0){
				return 0;
			}else{		
				args += (pre + length1);
				memcpy(args ,memparam,length2);
				args +=length2;
				*args++ = ' '; //spase
				memcpy(args ,ptembuf,lengtotal - pre + length1 + length2);
				return 0;

			}		
		}	
		pre = next;
	}
	memcpy(args + lengtotal," mem=",length1 +1);//space		
	memcpy(args + lengtotal + length1 + 1,memparam,length2 + 1);//'\0'
	/* All parsed OK. */
	return 0;
}
//end modified


#ifdef MX28
#include "../../mach-mx28/includes/registers/regsrtc.h"
#elif defined(STMP378X)
#include "../../mach-mx23/includes/registers/regsrtc.h"
#endif

#define NAND_SECONDARY_BOOT          0x00000002

/***********************************************************************/

/*
 * Magic key detection
 */
static enum magic_key get_magic_key (void)
{
	u32 keys;
	int magic;

	/* Detect pressed keys */
	keys = get_pressed_keys();

	/*
	 * Check if detected keys combination corresponds to
	 * any magic key code
	 */
	for (magic = MAGIC_KEY1; magic < MAGIC_KEY_NR; magic++)
		if (magic_keys[magic] == keys)
			return magic;

	return NO_MAGIC_KEY;
}

/*
 * Command lines handling
 *
 * Go over a .cmdlines section. It must contatin \0 terminated
 * command lines storead one by one:
 *
 * 	command_line1\0command_line2\0command_line3\0
 *	|					     |
 * cmdlines_start				cmdlines_end
 */

//modified by luozhizhuo
#define EMI_DDR2_SIZE_BASE					0x0000F100
#define EMI_DDR2_SIZE_BASE_ANOTHER		0x0000F400

uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte|0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if (in&0x100)
            ++crc;
        if (crc&0x10000)
            crc ^= 0x1021;
    }
    while (!(in&0x10000));
    return crc&0xffffu;
}

uint16_t Cal_CRC16(const char* data, int size)
{
    int crc = 0;
    const char* dataEnd = data+size;
    while (data<dataEnd)
        crc = UpdateCRC16(crc,*data++);

    crc = UpdateCRC16(crc,0);
    crc = UpdateCRC16(crc,0);
    return crc&0xffffu;
}

void getddr2_information(char **ddrinfo)
{
	char *pDDR2Size1 = (char *)EMI_DDR2_SIZE_BASE;	
	char *pDDR2Size2 = (char *)EMI_DDR2_SIZE_BASE_ANOTHER;
	uint16_t crc,length;

	if (0xFE == *pDDR2Size1) {
		length = *(pDDR2Size1 + 1);
		if (length > 255) 
			length = 10;
		crc = *(pDDR2Size1 + 2 + length);
		crc <<= 8;
		crc += *(pDDR2Size1 + 2 + length + 1);
		if (crc  == Cal_CRC16(pDDR2Size1,length + 2)){
			//setenv ("mem",pDDR2Size1 + 2);
			*ddrinfo = pDDR2Size1 + 2;
		} else {
			goto ddr2_bk;	
		}
	} else if (0xFE == *pDDR2Size2){
ddr2_bk:
		length = *(pDDR2Size2 + 1);
		if (length > 255) 
			length = 10;
		crc = *(pDDR2Size1 + 2 + length);
		crc <<= 8;
		crc += *(pDDR2Size1 + 2 + length + 1);

		if (crc  == Cal_CRC16(pDDR2Size1,length + 2)){
			//setenv ("mem",pDDR2Size2 + 2);
			*ddrinfo = pDDR2Size2 + 2;
		} else {
			//printf("get ddr2 infromation fail,check it!\n");
			*ddrinfo = (void *)0;
			return;
		}
	}
}

static void find_command_lines (void)
{
	char *start = &cmdlines_start[0];
	char *end = &cmdlines_end[0];
	char *head, *cur;
	int l;
	char*pDDR2Info;
	
	head = cur = start;

	for (l = 0; l < MAGIC_KEY_NR && cur < end; l++) {

		/* Find a command line end */
		while (cur < end && *cur++ != '\0') ;

		/*
		 * If there is a garbage or unterminated command
		 * line at the end of the section then skip it.
		 */
		if (cur == end && *(cur-1) != '\0')
			break;

		cmdlines[l] = head;
		head = cur;
	}

	/*
	 * Bind default command line to magic keys not presented
	 * in .commandlines
	 */
	while (l < MAGIC_KEY_NR)
		cmdlines[l++] = cmdline_def;
	
		
	cmdlines[0] = cmdline_def;		//modefy by zhuguojun 只用默认启动参数
	//to fix the mem size automatically
	getddr2_information(&pDDR2Info);
	if (pDDR2Info != (void *)0)
		parse_args(cmdline_def,pDDR2Info);
}

/*
 * Calculate SDRAM size:
 *
 * size = #cs * 4 banks * #columns * #rows * 2 bytes/word
 */

#define MAX_ROW		13
#define MAX_COLUMN	12
#define EMI_MAX_MEM_BK				(0x0000F100)

static u32 get_sdram_size (void)
{
	#if 1
	u32 row, column, csmap, size;
	u32 cs = 0;

#ifdef SDRAM_SIZE
	return SDRAM_SIZE;
#endif

	row = MAX_ROW -
	      ((HW_DRAM_CTL10_RD() & BM_DRAM_CTL10_ADDR_PINS) >> 16);

	column = MAX_COLUMN -
		 ((HW_DRAM_CTL11_RD() & BM_DRAM_CTL11_COLUMN_SIZE) >> 8);

	csmap = HW_DRAM_CTL14_RD() & BM_DRAM_CTL14_CS_MAP;

/*
 * Calculate the number of 1 bits in csmap
 * x & (x - 1) 	- clears the least significant 1 bit.
 */
	while (csmap) {
		csmap &= (csmap - 1);
		cs++;
	}

	size = cs * 4 * (1 << row) * (1 << column) * 2;
	
	return size;
#else
	int *pDDR2Size = (int *)EMI_MAX_MEM_BK;
	return (*pDDR2Size * 1024*1024);
#endif
}

/*
 * ARM atags
 */
static void setup_start_tag (void)
{
	params = (struct tag*)ATAGS_BASE_ADDRESS;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next(params);
}

static void setup_mem_tag (void)
{
	char * pdata  = (char *)EMI_MAX_MEM_BK;
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size(tag_mem32);

	params->u.mem.start = SDRAM_BASE;
	params->u.mem.size = get_sdram_size();

	params = tag_next(params);
}

static void setup_initrd_tag(void)
{
	params->hdr.tag = ATAG_INITRD2 ;
	params->hdr.size = tag_size(tag_initrd);

	params->u.initrd.start = 0x40800000;
	params->u.initrd.size =  0x00400000;

	params = tag_next(params);
}

static void setup_cmdline_tag (char *cmdline)
{

	u32 len;
	const char *mtdprefix[] = { "root=/dev/mtdblock", "root=ubi0:rootfs"};
#if 0
	char *o, *m, *s, *rest;
	int nr, i, j, p, digits;
	unsigned int val;
	// modefy by zhuguojun 不能给这段代码改动我们的启动参数
	/* is a nand root present in the command line? */
	for (p = 0; p < sizeof(mtdprefix)/sizeof(mtdprefix[0]); p++) {
		m = strstr(cmdline, mtdprefix[p]);
		if (m)
			break;
	}

	if (!m)
		goto normal_copy;

	/* yes, we have to read the persistent bit now */

	/* start the RTC block */
	HW_RTC_CTRL_CLR(BM_RTC_CTRL_SFTRST | BM_RTC_CTRL_CLKGATE);

	while (HW_RTC_STAT_RD() & BF_RTC_STAT_STALE_REGS(0x1 << 1))
		;
	val = HW_RTC_PERSISTENT1_RD();

	/* normal boot, no need to modify cmdline */
	if ((val & NAND_SECONDARY_BOOT) == 0)
		goto normal_copy;

	/* add one to the rootfs partition number
	 * we assume that the secondary rootfs partition
	 * is the next one
	 */
	o = params->u.cmdline.cmdline;
	m += strlen(mtdprefix[p]);
	memcpy(o, cmdline, m - cmdline);
	o += m - cmdline;

	s = m;
	while (*s && *s >= '0' && *s <= '9')
		s++;

	rest = s;

	s--;
	nr = 0;
	while (s >= m) {
		nr = nr * 10 + (*s - '0');
		s--;
	}

	nr++;
	digits = 0;
	i = nr;
	while (i > 0) {
		digits++;
		i /= 10;
	}

	i = nr;
	j = digits;
	while (j > 0) {
		o[j - 1] = '0' + (i % 10);
		i /= 10;
		j--;
	}
	o += digits;
	strcpy(o, rest);

	goto done_copy;
#endif
#if 0
normal_copy:
#endif
	/* Copy unchanged command line to SDRAM */
	strcpy(params->u.cmdline.cmdline, cmdline);
#if 0
done_copy:
#endif
	/* Command line length including '\0' */
	len = strlen(params->u.cmdline.cmdline) + 1;

	params->hdr.tag = ATAG_CMDLINE;

	/*
	 * Tag size should be multiple of 4, so it is needed to round
	 * up a command line size.
	 */
	params->hdr.size = (sizeof(struct tag_header) + len + 4) >> 2;

	params = tag_next(params);
}

static void setup_end_tag (void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

/*
 * External interface to set up tags in SDRAM
 * Returns a pointer to tag list in SDRAM
 *
 * The following tags are passed to kernel in addition to start and end
 * tags which must always exist:
 *
 * - Memory tag
 * - Command line tag
 */
u32 setup_tags (void)
{
	enum magic_key magic_key;

	//printf("come to set tags\r\n");
	magic_key = get_magic_key();
	find_command_lines();

	setup_start_tag();
	setup_mem_tag();
	setup_initrd_tag();
	//setup_cmdline_tag(cmdlines[magic_key]);
	setup_cmdline_tag(cmdlines[0]);				// modefy by zhuguojun 只用默认的启动参数
	setup_end_tag();
	//printf("end to set tags\r\n");

	return (u32)ATAGS_BASE_ADDRESS;
}
