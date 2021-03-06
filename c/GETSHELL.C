/*
 Copyright 1991 Robert Goldberg and Catspaw, Inc.

 SPITBOL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
/	File:  GETSHELL.C	Version:  01.04
/	---------------------------------------
/
/	Contents:	Function getshell
/
/	V1.04	18-Oct-90	Rewrite to use findenv().
/
/	V1.03	23-Jun-90	Move pathlast() to swcinp.c.
/
/	V1.02	03-Mar-88	Return in tscblk in all cases, so that
/				there is room to append after command string.
/
/	V1.01	28-Feb-88	Remove usage of strlen and strcpy
/
*/

#include "port.h"

/*
/   getshell()
/
/   Function getshell returns the path for the current shell.
/
/   Parameters:
/	None
/   Returns:
/	Pointer to character string representing current shell path
*/

char *getshell()
{
	register char *p;

	if ((p = findenv(SHELL_ENV_NAME, sizeof(SHELL_ENV_NAME))) == (char *)0)
		p = SHELL_PATH;		/* failure -- use default */
	return p;			/* value (with a null terminator) */
}
