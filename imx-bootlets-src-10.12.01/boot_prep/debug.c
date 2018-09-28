/*
 * Boot Prep common file
 *
 * Copyright 2008-2009 Freescale Semiconductor
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <stdarg.h>
void printhex(int data)
{
	int i = 0;
	char c;
	for (i = sizeof(int)*2-1; i >= 0; i--) {
		c = data>>(i*4);
		c &= 0xf;
		if (c > 9)
			putc(c-10+'A');
		else
			putc(c+'0');
	}
}

int Pos_Div(int x,int y)
{
    int ans = 0;
    int i ;
    for (i = 31; i>=0; i--) {
        if ((x>>i)>=y) {
            ans +=(1<<i);
            x -= (y<<i);
        }
    }

    return ans;
}

printint(int data)
{

    int i = 0;
    char str[10] ={0};
    int j = 0;
    while (j < 10 && data) {
        str[j] = data%10;
        data = Pos_Div(data, 10);
        j++;
    }

    for (i = j-1; i>=0; i--) {
        putc(str[i]+'0');
    }

}


void printf(char *fmt, ...)
{
	va_list args;
	int one;
	va_start(args, fmt);
	while (*fmt) {

		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {

			case 'x':
			case 'X':
				printhex(va_arg(args, int));
				break;
            case 'd':
            case 'D':
                printint(va_arg(args, int));
                break;
			case '%':
				putc('%');
				break;
			default:
				break;
			}

		} else {
			putc(*fmt);
		}
		fmt++;
	}
	va_end(args);
}


