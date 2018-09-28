/*
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * Based on linux/include/asm-arm/arch-stmp37xx/stmp37xx.h:
 * Copyright (C) 2005 Sigmatel Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __STMP37XX_H
#define __STMP37XX_H
#include <types.h>
#include <arch/platform.h>

#define __raw_readl(p) (*(volatile u32*)(p))
#define __raw_writel(x, p) ((*(volatile u32*)(p)) = (x))

#define HW_STMP3xxx_SET		0x04
#define HW_STMP3xxx_CLR		0x08
#define HW_STMP3xxx_TOG		0x0c

#define HW_REGISTER_WO(id,base,offset) \
	static const u32 id = base;			\
	static inline void id##_WR(u32 v) {		\
		__raw_writel(v, base + offset);		\
	}

#define HW_REGISTER_RO(id,base,offset) \
	static const u32 id = base;			\
	static inline u32 id##_RD(void) {		\
		return __raw_readl(base + offset);	\
	}

#define HW_REGISTER(id,base,offset) \
	static const u32 id = base;			\
	static inline u32 id##_RD(void) {		\
		return __raw_readl(base + offset); 	\
	}						\
	static inline void id##_WR(u32 v) {		\
		__raw_writel(v, base + offset);		\
	}						\
	static inline void id##_SET(u32 v) {		\
		__raw_writel(v, base + offset + HW_STMP3xxx_SET);	\
	}						\
	static inline void id##_CLR(u32 v) {		\
		__raw_writel(v, base + offset + HW_STMP3xxx_CLR);	\
	}						\
	static inline void id##_TOG(u32 v) {		\
		__raw_writel(v, base + offset + HW_STMP3xxx_TOG);	\
	}

#define HW_REGISTER_INDEXED(id,base,offset,step) \
	static inline u32 id(int n) {			\
		return base + n * step;                 \
	}						\
	static inline u32 id##_RD(int n) {		\
		return __raw_readl(base + offset + n * step); 	\
	}						\
	static inline void id##_WR(int n, u32 v) {	\
		__raw_writel(v, base + offset + n * step);		\
	}						\
	static inline void id##_SET(int n, u32 v) {	\
		__raw_writel(v, base + offset + HW_STMP3xxx_SET + n * step);	\
	}						\
	static inline void id##_CLR(int n, u32 v) {	\
		__raw_writel(v, base + offset + HW_STMP3xxx_CLR + n * step);	\
	}						\
	static inline void id##_TOG(int n, u32 v) {	\
		__raw_writel(v, base + offset + HW_STMP3xxx_TOG + n * step);	\
	}

#define HW_REGISTER_RO_INDEXED(id,base,offset,step) \
	static inline u32 id(int n) {			\
		return base + n * step;                 \
	}						\
	static inline u32 id##_RD(int n) {		\
		return __raw_readl(base + offset + n * step); 	\
	}						\

#define HW_REGISTER_0(id,base,offset) \
	static const u32 id = base;                     \
	static inline u32 id##_RD(void) {		\
		return __raw_readl(base + offset); 	\
	}						\
	static inline void id##_WR(u32 v) {		\
		__raw_writel(v, base + offset);		\
	}						\
	static inline void id##_SET(u32 v) {		\
		__raw_writel(				\
			v | __raw_readl(base + offset),	\
			base + offset);			\
	}						\
	static inline void id##_CLR(u32 v) {		\
		__raw_writel(				\
			__raw_readl(base + offset) & ~v,\
			base + offset);			\
	}						\
	static inline void id##_TOG(u32 v) {		\
		__raw_writel(				\
			__raw_readl(base + offset) ^ v,	\
			base + offset);			\
	}

#define HW_REGISTER_0_INDEXED(id,base,offset, step) 	\
	static inline u32 id(int n) {			\
		return base + n * step;                 \
	}						\
	static inline u32 id##_RD(int n) {		\
		return __raw_readl(base + offset + n * step); 	\
	}						\
	static inline void id##_WR(int n, u32 v) {	\
		__raw_writel(v, base + offset + n * step);		\
	}						\
	static inline void id##_SET(int n, u32 v) {	\
		__raw_writel(				\
			id##_RD(n) | v,			\
			base + offset + n * step);	\
	}						\
	static inline void id##_CLR(int n, u32 v) {	\
		__raw_writel(				\
			id##_RD(n) & ~v,		\
			base + offset + n * step);	\
	}						\
	static inline void id##_TOG(int n, u32 v) {	\
		__raw_writel(				\
			id##_RD(n) ^ v,			\
			base + offset + n * step);	\
	}

#ifndef __ASSEMBLY__
#include <arch/regs-lradc.h>
#include <arch/regs-emi.h>
#endif

#endif /* __STMP37XX_H */
