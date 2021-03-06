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
/	File:  ARG2SCB.C	Version:  01.02
/	---------------------------------------
/
/	Contents:	Function arg2scb
/
/	1.02	Rewritten to append to scblk instead of copy to block.
*/

/*
/   arg2scb( req, argc, argv, scptr, maxs )
/
/   arg2scb() makes a copy of the req-th argument in the argv array.
/   The copy is appended to the string in the SCBLK provided.
/
/   Parameters:
/	req	number of argument to copy
/	argc	number of arguments
/	argv	pointer to array of pointers to strings (arguments)
/	scptr	pointer to SCBLK to receive copy of argument
/	maxs	maximum number of characters to append.
/   Returns:
/	Length of argument copied or -1 if req is out of range.
/   Side Effects:
/	Modifies contents of passed SCBLK (scptr).
/	SCBLK length field is incremented.
*/

#include "port.h"

int	arg2scb( req, argc, argv, scptr, maxs )

int	req;
int	argc;
char	*argv[];
struct	scblk	*scptr;
int	maxs;

{
	register word	i;
	register char	*argcp, *scbcp;

	if ( req < 0  ||  req >= argc )
		return	-1;

	argcp	= argv[req];
	scbcp	= scptr->str + scptr->len;
	for( i = 0 ; i < maxs  &&  ((*scbcp++ = *argcp++) != 0) ; i++ )
		;
	scptr->len += i;
	return i;
}
