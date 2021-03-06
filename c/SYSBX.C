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
/	File:  SYSBX.C		Version:  01.06
/	---------------------------------------
/
/	Contents:	Function zysbx
*/

/*
/	zysbx - before execution setup
/
/	Setup here so that all further "standard output" goes to stdout.
/	This allows us to separate compiler/interpreter generated output
/	from output generated by the executing program.
/
/	If the -w command line option has been invoked, this module will
/	write an executable module and terminate.
/
/	If the -y command line option has been invoked, this module will
/	write a save (.spx) file and terminate.

/	Parameters:
/	    None
/	Returns:
/	    Nothing
/	Exits:
/	    None
/
/   V1.07   02/03/97 - call swcinp to close input file
/
/   V1.06   05/12/92 - Remove nameblk.  Use TSCBLK instead.
/
/	V1.05	10/20/89 - Distinguish -w and -y options
/
/	V1.04   09/14/89 - Use input file name with RUNEXT extension
/			   for file name with -y option.
*/

#include "port.h"

#if APPLE
#include "PEdit.h"
extern WindInfoPtr runWptr;
#endif					/* APPLE */


zysbx()
{
#if !RUNTIME

	executing = 1;

#if !APPLE
	if (readshell0) {
		doset(getrdiob(), 0L, 2); /* bypass rest of source file */
		curfile = inpcnt;	  /* skip rest of cmd line files */
        swcinp( inpcnt, inpptr ); /* v1.07 switch away from source file */
		}
#endif					/* !APPLE */

#if EXECFILE
	/*
	/   do we need to write an executable module, and if
	/   so does writing it produce an error?
	*/
	if ( spitflag & WRTEXE)
	{
	    pTSCBLK->len = appendext( *inpptr, BINEXT, pTSCBLK->str, 1 );

	    if ( makeexec( pTSCBLK, spitflag & NOEXEC ? 3 : 4 ) )
	    {
		wrterr( "Error writing load module." );
		zysej();
	    }
	}
#endif					/* EXECFILE */

#if SAVEFILE
	/*
	/   do we need to write a save (.spx) file, and if
	/   so does writing it produce an error?
	*/
	if ( spitflag & WRTSAV)
	{
	    pTSCBLK->len = appendext( *inpptr, RUNEXT, pTSCBLK->str, 1 );
	    if ( makeexec( pTSCBLK, spitflag & NOEXEC ? -3 : -4 ) )
	    {
#if USEQUIT
		quit(357);
#else					/* USEQUIT */
		wrterr( "Error writing save file." );
		zysej();
#endif					/* USEQUIT */
	    }
	}
	/*
	/   Execution does not resume here for dirty load modules.
	/   Because we must allow for new versions with different
	/   size C code, the stacked return addresses are not valid.
	/   Therefore, inter.asm forces a jump to restart code that
  /   eventually jumps to the MINIMAL code following the call
	/   the sysbx call.
	/
	/ ***********************************************************
	/ * WE DO NOT RETURN HERE.  ANY NEW CODE ADDED HERE MUST BE *
	/ * DUPLICATED IN THE RESTART CODE                          *
	/ ***********************************************************
	*/
#endif					/* SAVEFILE */

	/*  execution resumes here when a.out file created with		*/
	/*  the -w option is reloaded.					*/

#if MSDOS | OS2 | UNIX | WINNT
	startbrk();			/* turn on Control-C checking */
#endif               /* MSDOS | OS2 | WINNT */

	/*  swcoup does real work  */
	swcoup( outptr );

#if APPLE
	setstate(RunState, runWptr);
	exephase();
	exeinput();
#endif					/* APPLE */
#else					/* !RUNTIME */
	__exit(1);
#endif					/* !RUNTIME */

	return NORMAL_RETURN;
}
