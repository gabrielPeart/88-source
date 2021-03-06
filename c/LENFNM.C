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
/ File:  LENFNM.C   Version:  01.02
/	---------------------------------------
/
/	Contents:	Function lenfnm
*/

/*
/   lenfnm( scptr )
/
/   lenfnm() examines the file argument within the passed SCBLK and returns
/   the length of the filename contained within it.  This function will be
/   called from any of the OSINT functions dealing with filenames or I/O
/   options.
*/

/*  The file argument string will contain a filename and/or options with the
/   options enclosed in "[" and "]", as in
/
/	"filename"
/	"filename[options]"
/	"[options]"
/
/  v1.02 23-Feb-96 - Check pipe syntax when bracketed options present.
*/

#if !(APPLE | OS2 | WINNT)
/*  The file argument can also contain options separated from the filename
/	by a blank.
/
/	"filename options"
/	" options"
/
*/
#endif					/* !(APPLE | OS2) */

#if PIPES
/*
/   The file argument may instead be a command string with options as in
/
/	"!*commandstring"
/	"!*commandstring*"
/	"!*commandstring* options"
/
/   Notice that the character following the '!' serves as a delimiter to
/   separate the end of the command string from the space preceding any
/   options.
*/
#endif					/* PIPES */

/*  Parameters:
/	scptr	pointer to SCBLK containg filename string
/   Returns:
/	length of filename (0 is possible)
/       -1 if illegal name
*/

#include "port.h"

word lenfnm( scptr )

struct	scblk	*scptr;

{
  register word cnt, len, len2;
	register char	*cp;
#if PIPES
	register char delim;
#endif					/* PIPES */

/*
/	Null strings have filenames with lengths of 0.
*/
  len = len2 = scptr->len;
  if ( len == 0 )
		return	0L;

/*
/   Here to examine end of string for "[option]".
*/
  cp = &scptr->str[--len2];    /* last char of strng */
	if ( *cp == ']')			/* string end with "]" ? */
	{
		/* String ends with "]", find preceeding "[" */
    while (len2--)
		{
			if (*--cp == ']')
				break;
			if (*cp == '[')
      {
        /* valid option syntax, remove from length of string we'll examine */
        len = cp - scptr->str;
        break;
      }
		}
	}

	/* Look for space as the options delimiter */
	cp = scptr->str;

#if PIPES
/*
/	Here to bypass spaces within a pipe command.
/   Count characters through second occurrence of delimiting
/   character.  lenfnm( "!!foo goo!" ) = 10
*/
	if ( *cp == '!' )
	{
		if ( len < 3L )		/* "!!" clearly invalid		*/
			return	-1L;
		delim = *++cp;		/*  pick up delimiter		*/
		if ( *++cp == delim )	/* "!!!" also invalid		*/
			return	-1L;
					/*  count chars up to delim	*/
    for ( cnt = 2; cnt < len  && *cp++ != delim; cnt++ )
			;
    if ( *--cp == delim )   /* if last char is delim then */
         ++cnt;             /*   include it in the count  */
    return cnt;
	}
#endif					/* PIPES */

#if APPLE | OS2 | WINNT
  /* Apple, OS/2 HPFS, and WIN NT NTFS permit blanks within file names.
	 */
  return len;
#else						/* APPLE | OS2 */
/*
/	Here for a normal filename.  Just count the number of characters
/	up to the first blank or end of string, whichever occurs first.
*/
  for ( cnt = 0; cnt < len  &&  *cp++ != ' '; cnt++ )
		;
	return cnt;
#endif          /* APPLE | OS2 | WINNT */
}
