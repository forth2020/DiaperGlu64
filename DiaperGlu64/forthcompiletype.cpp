// //////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 2021 James Patrick Norris
//
//    This file is part of DiaperGlu v5.0.
//
//    DiaperGlu v5.0 is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    DiaperGlu v5.0 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with DiaperGlu v5.0; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// //////////////////////////////////////////////////////////////////////////////////////

// /////////////////////////////
// James Patrick Norris       //
// www.rainbarrel.com         //
// January 9, 2021            //
// version 5.0                //
// /////////////////////////////



#include "diapergluforth.h"

// according to the standard, dg_stateexecute needs to be NULL
// const char* dg_stateexecute = NULL;      // setting the state variable to this puts script interpreter into execute mode
// const char dg_statecompile[] = "compile"; // setting the state variable to this puts script interpreter into compile mode

void dg_forthcompilecallcore (Bufferhandle* pBHarrayhead)
//     ( addr -- )
{
	UINT64 addr;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	addr = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthcompilecallcorename);
		return;
	}

	dg_compilecallcore(
        pBHarrayhead,
        addr);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallcorename);
		return;
	}
}


void dg_forthocall(Bufferhandle* pBHarrayhead)
//     ( compileroutineoffset compileroutinebufid -- )
{
	UINT64 compilebufid = 0;
	UINT64 compilebufoffset = 0;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);

	compilebufid = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthocallname);
		return;
	}

	compilebufoffset = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthocallname);
		return;
	}

	dg_callbuffer(
        pBHarrayhead,
        compilebufid,
        compilebufoffset);
}


void dg_forthcompilecallsamebuffer(Bufferhandle* pBHarrayhead)
//     ( offsetinsamebuffer -- )
{	
	UINT64 targetoffset;
	
	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }
	
	
	targetoffset = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthcompilecallsamebuffername);
		return;
	}
	
	dg_compilealignretstack(
        pBHarrayhead,
        1);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallsamebuffername);
		return;
	}
	
	dg_compilepushpBHarrayheadtoret(pBHarrayhead);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallsamebuffername);
		return;
	}
	
	dg_compilecalloffsetinsamebuffer(
        pBHarrayhead,
        targetoffset);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallsamebuffername);
		return;
	}
	
	// dg_compileundoalignretstack(pBHarrayhead);
	
	// if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	// {
	//	dg_pusherror(pBHarrayhead, dg_forthcompilecallsamebuffername);
	//	return;
	// }
}


void dg_forthcompilecallbuffer(Bufferhandle* pBHarrayhead)
//                     ( bufferoffset bufferid -- )
{
	UINT64* pbuflength;
	unsigned char* pdatastack;
	
	UINT64* pints;
	
    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
	
	if (baderrorcount == olderrorcount)
	{
		return;
	}
	
	dg_compilealignretstack(
        pBHarrayhead,
        3);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);
	
	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	if (*pbuflength < (2 * sizeof(UINT64)) )
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	// could check for misaligned data stack here
	
	pints = (UINT64*)(pdatastack + *pbuflength - (2 * sizeof(UINT64)));
	
	dg_compilentoparameter (
        pBHarrayhead,
        pints[0], // codebufferoffset
        2); // 0 based parameter index
    
	// dg_compilepushntoret(
     //   pBHarrayhead,
     //   pints[0]); // codebufferoffset
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	// dg_compilepushntoret(
    //    pBHarrayhead,
    //    pints[1]); // codebufferid
    
    dg_compilentoparameter (
        pBHarrayhead,
        pints[1], // codebufferid
        1); // 0 based parameter index
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	dg_compilepushpBHarrayheadtoret(pBHarrayhead);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	dg_compilecalladdress(
        pBHarrayhead,
        (UINT64)&dg_callbuffer);
	
	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
		return;
	}
	
	// dg_compiledropparametersfromret(
    //    pBHarrayhead,
    //    3);
	
	// if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	// {
	//	dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
	//	return;
	// }
	
	// dg_compileundoalignretstack(pBHarrayhead);
	
	// if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	// {
	//	dg_pusherror(pBHarrayhead, dg_forthcompilecallbuffername);
	//	return;
	// }
	
	*pbuflength = *pbuflength - (2 * sizeof(UINT64));
}

const char* dg_forthcompilewordsstringquotesname = "dg_forthcompilewordsstringquotes";

void dg_forthcompilewordsstringquotes (Bufferhandle* pBHarrayhead )
{
    unsigned char* pword;
    UINT64 wordlength;
    
    UINT64 numberofwords = 0;
    
    UINT64 foundendflag = FORTH_FALSE;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }
    
    while (foundendflag == FORTH_FALSE)
    {
        pword = dg_parsewords(
            pBHarrayhead,
            &wordlength,
            '"', // enddelimiter
            &foundendflag,
            FORTH_FALSE);
            
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
          dg_pusherror(pBHarrayhead, dg_forthcompilewordsstringquotesname);
          return;
        }
        
        if (wordlength != 0) // no empty strings from this function
        {
            numberofwords++;
            
            dg_compilecopystonewstring (
                pBHarrayhead,
                (const char*)pword,
                wordlength);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
              dg_pusherror(pBHarrayhead, dg_forthcompilewordsstringquotesname);
              return;
            }
        }
    }

    dg_compilepushntodatastack (
        pBHarrayhead,
        numberofwords);
    
    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
      dg_pusherror(pBHarrayhead, dg_forthcompilewordsstringquotesname);
      return;
    }
}

const char* dg_forthcompilewords0stringquotesname = "dg_forthcompilewords0stringquotes";

void dg_forthcompilewords0stringquotes (Bufferhandle* pBHarrayhead )
{
    unsigned char* pword;
    UINT64 wordlength;
    
    UINT64 numberofwords = 0;
    
    UINT64 foundendflag = FORTH_FALSE;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }
    
    while (foundendflag == FORTH_FALSE)
    {
        pword = dg_parsewords(
            pBHarrayhead,
            &wordlength,
            '"', // enddelimiter
            &foundendflag,
            FORTH_FALSE);
            
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
          dg_pusherror(pBHarrayhead, dg_forthcompilewords0stringquotesname);
          return;
        }
        
        if (wordlength != 0) // no empty strings from this function
        {
            numberofwords++;
            
            dg_compilecopystonew0string (
                pBHarrayhead,
                (const char*)pword,
                wordlength);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
              dg_pusherror(pBHarrayhead, dg_forthcompilewords0stringquotesname);
              return;
            }
        }
    }
    
    dg_compilepushntodatastack (
        pBHarrayhead,
        numberofwords);
    
    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
      dg_pusherror(pBHarrayhead, dg_forthcompilewords0stringquotesname);
      return;
    }
}


const char* dg_forthdocompiletypealwaysexecutename = "dg_forthdocompiletypealwaysexecute";

void dg_forthdocompiletypealwaysexecute(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	// dg_forthocall(pBHarrayhead);
    UINT64 compilebufid = 0;
	UINT64 compilebufoffset = 0;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	compilebufid = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthdocompiletypealwaysexecutename);
		return;
	}

	compilebufoffset = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthdocompiletypealwaysexecutename);
		return;
	}

	dg_callbuffer(
        pBHarrayhead,
        compilebufid,
        compilebufoffset);
}

// Calls or compiles a call to a Diaperglu forth script subroutine.
// Diaperglu script subroutines have the c function prototype of:
//  void dg_forthfunctionname (Bufferhandle* pBHarrayhead)
void dg_forthdocompiletypesubroutine(Bufferhandle* pBHarrayhead)
//      ( dataoffset databufid -- )
{
	const char* state;
	UINT64 compilebufid;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)))
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - (2 * sizeof(UINT64)));

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
		return;
	}

	if (state == dg_stateexecute)
	{
        // dg_forthocall(pBHarrayhead);
		dg_forthdocompiletypealwaysexecute(pBHarrayhead);
		// no error reported from this on purpose
	}
	else 
	{
		if (state == dg_statecompile)
		{
			if (pints[1] == DG_CORE_BUFFERID) // call to the core
			{
                dg_compilecallcore(
                    pBHarrayhead,
                    pints[0]);
                
                *pbuflength -= (2 * sizeof(UINT64));
			
				// not checking error here on purpose
			}
			else // call to a buffer
			{
				compilebufid = dg_getbufferuint64(
                    pBHarrayhead,
                    DG_DATASPACE_BUFFERID,
                    currentcompilebuffer);

				if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
				{
                    dg_pusherror(pBHarrayhead, dg_forthpcurrentcompilebuffername);
					dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
					return;
				}

				// put routine to compile to a buffer here
				if (pints[1] == compilebufid)
				{
					*pbuflength -= sizeof(UINT64);  // just need data offset for this

					dg_forthcompilecallsamebuffer(pBHarrayhead);
					
					if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
					{
						dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
                        
						return;
					}
				}
				else
				{
					// call to a different buffer
					dg_forthcompilecallbuffer(pBHarrayhead);

					if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
					{
						dg_pusherror(pBHarrayhead, dg_forthodoctsubroutinename);
						return;
					}
				}
			}
		}
		else
		{
			// unknown state - do nothing
			*pbuflength -= (2 * sizeof(UINT64));
		}
	}
}


/*
void dg_forthdocompiletypepreserveregs(Bufferhandle* pBHarrayhead)
//      ( dataoffset databufid -- )
{
	const char* state;

	UINT32* pbuflength;
	unsigned char* pdatastack;

	UINT32* pints;
    
    UINT32 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(pBHarrayhead, DG_DATASTACK_BUFFERID, &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodoctpreserveregsname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT32)) )
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodoctpreserveregsname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT32*)(pdatastack + *pbuflength - 2*sizeof(UINT32));

	state = (const char*)dg_getbufferuint64(pBHarrayhead, DG_DATASPACE_BUFFERID, statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodoctpreserveregsname);
		return;
	}

	if (state == dg_stateexecute)
	{
		dg_forthocall(pBHarrayhead);
		// no error reported from this on purpose
	}
	else 
	{
		if (state == dg_statecompile)
		{
			if (pints[1] == DG_CORE_BUFFERID) // call to the core
			{
				dg_compilecallcorepreserveregs(pBHarrayhead, pints[0]);
                
                *pbuflength -= 2*sizeof(UINT32);
			
				// not checking error here on purpose
			}
			else // call to a buffer
			{
                dg_pusherror(pBHarrayhead, "preserving regs during a call only supported for core (fixed address) routines at this time");
            }
        }
		else
		{
			// unknown state - do nothing
			*pbuflength -= 2*sizeof(UINT32);
		}
	}
}
*/


void dg_forthdocompiletypesafesubroutine (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodoctsafesubroutinename);
		return;
	}

	if (state == dg_stateexecute)
	{
		dg_forthocall(pBHarrayhead); // dont need a safe return for execute because
		                       // the return address is in the core 

		// not checking error here on purpose
	}
	else 
	{
		if (state == dg_statecompile)
		{
			// compile safe call buffer
			dg_forthcompilesafecallbuffer(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctsafesubroutinename);
				return;
            }

            dg_putbufferuint64(
                pBHarrayhead,
                DG_DATASPACE_BUFFERID,
                coloncallssafeflag,
                FORTH_TRUE);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthpcoloncallssafeflagname);
				dg_pusherror(pBHarrayhead, dg_forthodoctsafesubroutinename);
				return;
            }
		}
		else
		{
			// unknown state - do nothing
			dg_forthtwodrop(pBHarrayhead);
		}
	}
}


void dg_forthdocompiletypedpushn(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints = NULL;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushnname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)))
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushnname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - 2*sizeof(UINT64));

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushnname);
		return;
	}

	if (state == dg_stateexecute)
	{
		*pbuflength -= 1*sizeof(UINT64);
	}
	else 
	{
		if (state == dg_statecompile)
		{
			*pbuflength -= sizeof(UINT64);

			dg_forthliteral(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushnname);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			*pbuflength -= (2*sizeof(UINT64));
		}
	}
}


void dg_forthdocompiletypedpushf(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
    const char* state;

    UINT64* pbuflength;
    unsigned char* pdatastack;

    UINT64* pints = NULL;
    
    UINT64 n;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

    if (pdatastack == (unsigned char*)badbufferhandle)
    {
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushfname);
        return;
    }

    if (*pbuflength < (2 * sizeof(UINT64)))
    {
        dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushfname);
        return;
    }

    // could check for misaligned data stack here

    pints = (UINT64*)(pdatastack + *pbuflength - 2*sizeof(UINT64));

    state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushfname);
        return;
    }

    if (state == dg_stateexecute)
    {
        n = pints[0];
        
        dg_pushbufferuint64(
            pBHarrayhead,
            DG_F64STACK_BUFFERID,
            n);
            
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushfname);
            return;
        }

        *pbuflength -= 2*sizeof(UINT64);
    }
    else 
    {
        if (state == dg_statecompile)
        {
            n = pints[0];
            
            dg_pushbufferuint64(
                pBHarrayhead,
                DG_F64STACK_BUFFERID,
                n);
                
            *pbuflength -= 2*sizeof(UINT64);

            dg_forthfliteral(pBHarrayhead);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushfname);
                return;
            }
        }
        else
        {
            // unknown state do nothing
            *pbuflength -= (2*sizeof(UINT64));
        }
    }
}



void dg_forthdocompiletypedpushdn(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)))
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - 2*sizeof(UINT64));

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
		return;
	}

	if (state == dg_stateexecute)
	{
		// nothing to do, n lo n hi is already on the data stack
	}
	else 
	{
		if (state == dg_statecompile)
		{
			dg_forthswap(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
				return;
			}
		
			dg_forthliteral(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
				return;
			}
		
			dg_forthliteral(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctdpushdnname);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			*pbuflength -= (2 * sizeof(UINT64));
		}
	}
}



void dg_forthdocompiletypedpushp (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)) )
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - (2 * sizeof(UINT64)));

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
		return;
	}

	
	if (state == dg_stateexecute)
	{
		dg_forthgetpbufferoffset(pBHarrayhead);
		
		if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
		{
			dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
			return;
		}
	}
	else 
	{
		if (state == dg_statecompile)
		{
			// could improve this by compiling a call to the C routine instead

            // dg_compileobtoptodatastack(
            //    pBHarrayhead,
            //    pints[1],
            //    pints[0]);
            
			dg_forthswap(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
				return;
			}

			dg_forthliteral(pBHarrayhead); // compile push offset

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
				return;
			}

			dg_forthliteral(pBHarrayhead); // compile push bufid

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
				return;
			}

            /*
			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)(&dg_forthgetpbufferoffset));

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);
            */
            
            dg_compilecallcore(
                pBHarrayhead,
                (UINT64)(&dg_forthgetpbufferoffset));

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushpname);
				return;
			}

		}
		else
		{
			// unknown state do nothing
			*pbuflength -= (2 * sizeof(UINT64));
		}
	}
}


void dg_forthdocompiletypedpushs (Bufferhandle* pBHarrayhead)
//                         ( dataoffset databufid -- )
{
	const char* state;
    unsigned char* pstring;
    UINT64 stringlength = 0;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	dg_forthtwodrop(pBHarrayhead);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
		return;
	}

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
		return;
	}

	if (state == dg_stateexecute)
	{
		dg_forthstringquotes(pBHarrayhead);

		if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
		{
			dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
			return;
		}
				
	}
	else
	{
		if (state == dg_statecompile)
		{
            /*
			dg_forthsquotes(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
				return;
			}

			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)&dg_forthstostring);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
				return;
			}
   */
            
            
            pstring = dg_parse(
                pBHarrayhead,
                &stringlength,
                '"');
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
                return;
            }
            
            dg_compilecopystonewstring (
                pBHarrayhead,
                (const char*)pstring,
                stringlength);
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypedpushsname);
                return;
            }
		}
		else
		{
			// unknown state do nothing
		}		
	}
}


void dg_forthdocompiletypewordsstringquotes (Bufferhandle* pBHarrayhead)
//                         ( dataoffset databufid -- )
{
    const char* state;
    unsigned char* pstring;
    UINT64 stringlength = 0;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    dg_forthtwodrop(pBHarrayhead);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
        return;
    }

    state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
        return;
    }

    if (state == dg_stateexecute)
    {
        dg_forthwordsstringquotes(pBHarrayhead);

        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
            return;
        }
                
    }
    else
    {
        if (state == dg_statecompile)
        {
            dg_forthcompilewordsstringquotes(pBHarrayhead);
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
                return;
            }
        }
        else
        {
            // unknown state do nothing
        }
    }
}

/*
void dg_forthdocompiletyperunfileandwaitnoenvquotes (Bufferhandle* pBHarrayhead)
//                         ( dataoffset databufid -- )
{
    const char* state;
    unsigned char* pstring;
    UINT64 stringlength = 0;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    dg_forthtwodrop(pBHarrayhead);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
        return;
    }

    state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewordsstringquotesname);
        return;
    }

    if (state == dg_stateexecute)
    {
        dg_forthrunfileandwaitnoenvquotes(pBHarrayhead);

        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletyperunfileandwaitnoenvquotesname);
            return;
        }
                
    }
    else
    {
        if (state == dg_statecompile)
        {
            dg_forthcompilewords0stringquotes(pBHarrayhead);
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletyperunfileandwaitnoenvquotesname);
                return;
            }
            
            dg_compilecallcore(
                pBHarrayhead,
                (UINT64)(&dg_forthrunfileandwaitnoenvquotessub));
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletyperunfileandwaitnoenvquotesname);
                return;
            }
        }
        else
        {
            // unknown state do nothing
        }
    }
}
*/

void dg_forthdocompiletypewords0stringquotes (Bufferhandle* pBHarrayhead)
//                         ( dataoffset databufid -- )
{
    const char* state;
    unsigned char* pstring;
    UINT64 stringlength = 0;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    dg_forthtwodrop(pBHarrayhead);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewords0stringquotesname);
        return;
    }

    state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypewords0stringquotesname);
        return;
    }

    if (state == dg_stateexecute)
    {
        dg_forthwords0stringquotes(pBHarrayhead);

        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletypewords0stringquotesname);
            return;
        }
                
    }
    else
    {
        if (state == dg_statecompile)
        {
            dg_forthcompilewords0stringquotes(pBHarrayhead);
                
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypewords0stringquotesname);
                return;
            }
        }
        else
        {
            // unknown state do nothing
        }
    }
}


void dg_forthdocompiletypecdecl (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;
    UINT64 databufid;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    // can't ignore databufid anymore... May 3, 2017
    
	// dg_forthdrop(pBHarrayhead);
    
    databufid = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
		return;
	}

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
		return;
	}

	if (state == dg_stateexecute)
	{
        if (databufid != DG_CORE_BUFFERID)
        {
            dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                databufid);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                return;
            }
    
            dg_forthgetpbufferoffset(pBHarrayhead);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                return;
            }
        }
        
		dg_forthcallprocaddress(pBHarrayhead);

		// not reporting error here on purpose
	}
	else
	{
		if (state == dg_statecompile)
		{
            if (databufid != DG_CORE_BUFFERID)
            {
                // ccbufid = dg_getbufferuint64(
                //    pBHarrayhead,
                //    DG_DATASPACE_BUFFERID,
                //    currentcompilebuffer);
                
                // if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                // {
                //    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                //    return;
                // }
                
                // if (databufid == ccbufid)
                // {
                    // need a call proc address in same buffer routine
                    
                //    return;
                // }
                
                // compile code to do O>P
                dg_forthliteral(pBHarrayhead);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }
                
                dg_pushbufferuint64(
                    pBHarrayhead,
                    DG_DATASTACK_BUFFERID,
                    databufid);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }
                
                dg_forthliteral(pBHarrayhead);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }
                
                dg_pushbufferuint64(
                    pBHarrayhead,
                    DG_DATASTACK_BUFFERID,
                    (UINT64)&dg_forthgetpbufferoffset);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }

                dg_forthcompilecallcore(pBHarrayhead);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }
            }
            else
            {
                // bufferid is core so compile code to push the address of the routine to the datastack
                dg_forthliteral(pBHarrayhead);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                    return;
                }
            }
			
            // compile code to call the proc
			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)&dg_forthcallprocaddress);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			dg_forthdrop(pBHarrayhead);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
				return;
			}
		}		
	}
}


void dg_forthdocompiletypecdeclretuint128 (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	dg_forthdrop(pBHarrayhead);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
		return;
	}

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
		return;
	}

	if (state == dg_stateexecute)
	{
		dg_forthcallprocaddressretuint128(pBHarrayhead);

		// not reporting error here on purpose
	}
	else
	{
		if (state == dg_statecompile)
		{
			dg_forthliteral(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
				return;
			}
			
			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)&dg_forthcallprocaddressretuint128);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			dg_forthdrop(pBHarrayhead);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctcdeclretuint128name);
				return;
			}
		}		
	}
}


void dg_forthdocompiletypedfpcdecl (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;
    UINT64 databufid;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    // can't ignore databufid anymore... May 3, 2017
    
	// dg_forthdrop(pBHarrayhead);
    
    databufid = dg_popbufferuint64(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
		return;
	}

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
		return;
	}

	if (state == dg_stateexecute)
	{
        if (databufid != DG_CORE_BUFFERID)
        {
            dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                databufid);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                return;
            }
    
            dg_forthgetpbufferoffset(pBHarrayhead);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                return;
            }
        }
        
		dg_forthcalldfpprocaddress(pBHarrayhead);

		// not reporting error here on purpose
	}
	else
	{
		if (state == dg_statecompile)
		{
            if (databufid != DG_CORE_BUFFERID)
            {
                // ccbufid = dg_getbufferuint64(
                //    pBHarrayhead,
                //    DG_DATASPACE_BUFFERID,
                //    currentcompilebuffer);
                
                // if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                // {
                //    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecdeclname);
                //    return;
                // }
                
                // if (databufid == ccbufid)
                // {
                    // need a call proc address in same buffer routine
                    
                //    return;
                // }
                
                // compile code to do O>P
                dg_forthliteral(pBHarrayhead);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }
                
                dg_pushbufferuint64(
                    pBHarrayhead,
                    DG_DATASTACK_BUFFERID,
                    databufid);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }
                
                dg_forthliteral(pBHarrayhead);
                
                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }
                
                dg_pushbufferuint64(
                    pBHarrayhead,
                    DG_DATASTACK_BUFFERID,
                    (UINT64)&dg_forthgetpbufferoffset);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }

                dg_forthcompilecallcore(pBHarrayhead);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }
            }
            else
            {
                // bufferid is core so compile code to push the address of the routine to the datastack
                dg_forthliteral(pBHarrayhead);

                if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
                {
                    dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
                    return;
                }
            }
			
            // compile code to call the proc
			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)&dg_forthcalldfpprocaddress);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			dg_forthdrop(pBHarrayhead);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodocpltypedfpcdeclname);
				return;
			}
		}		
	}
}


/*
void dg_forthdocompiletypecppmemberfunction (Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

	dg_forthdrop(pBHarrayhead);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
		dg_pusherror(pBHarrayhead, dg_forthodoctcppmembername);
		return;
	}

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthodoctcppmembername);
		return;
	}

	if (state == dg_stateexecute)
	{
		dg_forthcallcppmemberfunction(pBHarrayhead);

		// not reporting error here on purpose
	}
	else
	{
		if (state == dg_statecompile)
		{
			dg_forthliteral(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctcppmembername);
				return;
			}
			
			dg_pushbufferuint64(
                pBHarrayhead,
                DG_DATASTACK_BUFFERID,
                (UINT64)&dg_forthcallcppmemberfunction);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
                dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
				dg_pusherror(pBHarrayhead, dg_forthodoctcppmembername);
				return;
			}

			dg_forthcompilecallcore(pBHarrayhead);

			if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
			{
				dg_pusherror(pBHarrayhead, dg_forthodoctcppmembername);
				return;
			}
		}
		else
		{
			// unknown state do nothing
			dg_forthdrop(pBHarrayhead);
		}		
	}
}
*/


// Compiles a call to a subroutine with an unspecified prototype regardless of state.
//  User is responsible for making sure stack is set up and/or
//  aligned prior to the call.
void dg_forthdocompiletypecall (Bufferhandle* pBHarrayhead)
//      ( dataoffset databufid -- )
{
	// const char* state; // ignored...
	UINT64 compilebufid;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)) )
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - (2 * sizeof(UINT64)));


    if (pints[1] == DG_CORE_BUFFERID) // call to the core
    {        
        // compile a call to an absolute address
        dg_compilecalladdress (
            pBHarrayhead,
            pints[0]);
                
        *pbuflength -= (2 * sizeof(UINT64));
        
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
            return;
        }
        
        return;
    }


    compilebufid = dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        currentcompilebuffer);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthpcurrentcompilebuffername);
        dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
        return;
    }

    if (pints[1] == compilebufid)
    {
        //  compile a call to the same buffer
        dg_compilecalloffsetinsamebuffer (
            pBHarrayhead,
            pints[0]);
        
        *pbuflength -= (2 * sizeof(UINT64));  // just need data offset for this

        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
            return;
        }
        
        return;
    }

    // What if I checked to see if buffer was locked down and allowed calls to locked down buffers?
    //  For now just making it difficult so programmers will at least be aware of the issue.
    dg_pusherror(pBHarrayhead, (const char*)"Target function is in different buffer from caller. The issue is the buffer may not be locked down and it's address may change.");
    dg_pusherror(pBHarrayhead, dg_forthodocompiletypecallname);
    return;
}


const char dg_forthdocompiletypeftcolonname[] = "dg_forthdocompiletypeftcolon";

// Calls or compiles a call to a Diaperglu forth script subroutine
//  in the Diaperglu function table
// Diaperglu script subroutines have the c function prototype of:
//  void dg_forthfunctionname (Bufferhandle* pBHarrayhead)
void dg_forthdocompiletypeftcolon (Bufferhandle* pBHarrayhead)
//      ( dataoffset databufid -- )
{
	const char* state;
	UINT64 compilebufid;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints;
    
    UINT64 n;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)) )
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - (2 * sizeof(UINT64)));

	state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

	if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
	{
        dg_pusherror(pBHarrayhead, dg_forthstatename);
		dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
		return;
	}
    
    n = pints[0];
    
    // drop dataoffset and databufid
    *pbuflength -= (2 * sizeof(UINT64));
    
    if (n >= DG_NUMBEROFFUNCTIONTABLEIDS)
    {
        dg_pusherror(pBHarrayhead, dg_indexnotinarrayerror);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
		return;
    }
    
	if (state == dg_stateexecute)
	{
        dg_callcoreforthroutine (
            pBHarrayhead,
            dg_functiontable[n]);
		// no error reported from this on purpose
	}
	else 
	{
		if (state == dg_statecompile)
		{
            dg_compilecallftcolon (
                pBHarrayhead,
                n);
		}
		else
		{
			// unknown state - do nothing
		}
	}
}

const char dg_forthdocompiletypedpushbracketrbpplusnname[] = "dg_forthdocompiletypedpushbracketrbpplusn";

void dg_forthdocompiletypedpushbracketrbpplusn(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
    const char* state;

    UINT64* pbuflength;
    unsigned char* pdatastack;

    UINT64* pints = NULL;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }

    pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

    if (pdatastack == (unsigned char*)badbufferhandle)
    {
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
        return;
    }

    if (*pbuflength < (2 * sizeof(UINT64)) )
    {
        dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypeftcolonname);
        return;
    }
    
    // could check for misaligned data stack here

    pints = (UINT64*)(pdatastack + *pbuflength - 2*sizeof(UINT64));

    state = (const char*)dg_getbufferuint64(
        pBHarrayhead,
        DG_DATASPACE_BUFFERID,
        statevariable);

    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthstatename);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
        return;
    }

    if (state == dg_stateexecute)
    {
        pints[1] = pints[0];
        pints[0] = dg_rbp;
        
        dg_pushdatastack(pBHarrayhead, 0);  // displacement size
        
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
            return;
        }
        
        dg_pushdatastack(pBHarrayhead, dg_isbasedisplacement);
        
        if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
        {
            dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
            return;
        }
    }
    else 
    {
        if (state == dg_statecompile)
        {
            *pbuflength -= sizeof(UINT64);
            
            dg_pushdatastack(pBHarrayhead, dg_rbp);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }

            dg_forthliteral(pBHarrayhead);

            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
            
            dg_forthliteral(pBHarrayhead);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
            
            dg_pushdatastack(pBHarrayhead, 0);  // displacement size
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
            
            dg_forthliteral(pBHarrayhead);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
            
            dg_pushdatastack(pBHarrayhead, dg_isbasedisplacement);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
            
            dg_forthliteral(pBHarrayhead);
            
            if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
            {
                dg_pusherror(pBHarrayhead, dg_forthdocompiletypedpushbracketrbpplusnname);
                return;
            }
        }
        else
        {
            // unknown state do nothing
            *pbuflength -= (2*sizeof(UINT64));
        }
    }
}

void dg_forthdocompiletypebrackettoorder(Bufferhandle* pBHarrayhead)
//     ( dataoffset databufid -- )
{
	const char* state;

	UINT64* pbuflength;
	unsigned char* pdatastack;

	UINT64* pints = NULL;

    UINT64 olderrorcount = dg_geterrorcount(pBHarrayhead);
    
    if (baderrorcount == olderrorcount)
    {
        return;
    }


	pdatastack = dg_getpbuffer(
        pBHarrayhead,
        DG_DATASTACK_BUFFERID,
        &pbuflength);

	if (pdatastack == (unsigned char*)badbufferhandle)
	{
        dg_pusherror(pBHarrayhead, dg_forthdatastackbufferidname);
		dg_pusherror(pBHarrayhead, dg_forthdocompiletypebrackettoordername);
		return;
	}

	if (*pbuflength < (2 * sizeof(UINT64)))
	{
		dg_pusherror(pBHarrayhead, dg_datastackunderflowerror);
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypebrackettoordername);
		return;
	}

	// could check for misaligned data stack here

	pints = (UINT64*)(pdatastack + *pbuflength - 2*sizeof(UINT64));
 
    dg_pushbufferuint64(
        pBHarrayhead,
        DG_SEARCHORDERSTACK_BUFFERID,
        pints[0]);
        
    if (dg_geterrorcount(pBHarrayhead) != olderrorcount)
    {
        dg_pusherror(pBHarrayhead, dg_forthdocompiletypebrackettoordername);
        return;
    }

    *pbuflength -= 2*sizeof(UINT64);	
}