/*
 Copyright (c)  1991 Robert Goldberg and Catspaw, Inc.

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
/	File:  SYSRW.C		Version:  01.01
/	---------------------------------------
/
/	Contents:	Function zysrw
*/

/*
/   zysrw - rewind file
/
/	Parameters
/	    WA - pointer to FCBLK or 0
/	    XR - pointer to SCBLK containing rewind argument
/	Returns:
/	    Nothing
/	Exits:
/	    1 - file doesn't exit
/	    2 - rewind not allowed on this device
/	    3 - I/O error
/
*/

#include "port.h"

zysrw()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct ioblk *iob = MK_MP(fcb->iob, struct ioblk *);

	/* ensure the file is open */
	if ( !(iob->flg1 & IO_OPN) )
		return EXIT_1;

   /* see if this file can be LSEEK'ed */
   if ( LSEEK(iob->fdn, (FILEPOS)0, 1) < (FILEPOS)0 )
		return EXIT_2;

	/* seek to the beginning */
   if (doset( iob, 0L, 0 ) == (FILEPOS)-1)
		return EXIT_3;

	return NORMAL_RETURN;
}
