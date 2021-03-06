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
/	File:  OSCLOSE.C	Version:  01.00
/	---------------------------------------
/
/	Contents:	Function osclose
*/

/*
/   osclose( ioptr )
/
/   osclose() closes the file represented by the passed IOBLK.
/
/   Parameters:
/	ioptr	pointer to IOBLK
/   Returns:
/	Number of I/O errors, should be 0.
*/

#include "port.h"

#if WINNT
extern void kill(int pid);
#endif

osclose( ioptr )
struct	ioblk	*ioptr;
{
	register int	errcnt = 0;

/*
/	If not open, nothing to do.
*/
	if ( !(ioptr->flg1 & IO_OPN) )
		return 0;

/*
/	Flush buffer before closing output file.
*/
	if ( ioptr->flg1 & IO_OUP )
		errcnt += flush( ioptr );

/*
/ DO NOT CLOSE SYSTEM FILE 0, 1 or 2; file was opened by shell.
*/
  if ( (ioptr->flg1 & IO_SYS) && ioptr->fdn >= 0 && ioptr->fdn <= 2 )
		return errcnt;

/*
/	Now we can reset open flag and close the file descriptor associated
/	with file/pipe.
*/
	ioptr->flg1 &= ~IO_OPN;
    if ( close(ioptr->fdn ) < 0 )
		errcnt++;

#if PIPES
/*
/	For a pipe, must deal with process at other end.
*/
	if ( ioptr->flg2 & IO_PIP )
	{
		/*
		/   If process already dead just reset flag.
		*/
		if ( ioptr->flg2 & IO_DED )
			ioptr->flg2 &= ~IO_DED;

		/*
		/   If reading from pipe, kill the process at other end
		/   and wait for its termination.
		*/
		else if ( ioptr->flg1 & IO_INP )
		{
			kill( ioptr->pid );
			oswait( ioptr->pid );
		}

		/*
		/   If writing to pipe, wait for it to terminate.
		*/
		else
			oswait( ioptr->pid );
	}
#endif					/* PIPES */

/*
/	Return number of errors.
*/
	return errcnt;
}
