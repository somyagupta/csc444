/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2009, Tomasz Sowa
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *    
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *    
 *  * Neither the name Tomasz Sowa nor the names of contributors to this
 *    project may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef bigtypesheader
#define bigtypesheader

#include "compileconfig.h"


//#define TTMATH_RELEASE
#include <ttmath/ttmath.h>

#ifdef _MSC_VER
//"warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)"
// (ttmath set it to default)
#pragma warning (disable : 4800)
#endif


#ifndef TTCALC_PORTABLE

	/*
	// old ones
	ttmath::Big<1,3>
	ttmath::Big<2,6>
	ttmath::Big<3,9>
	*/

	/*
	typedef ttmath::Big<1,3>  TTMathBig1;
	typedef ttmath::Big<2,9>  TTMathBig2;
	typedef ttmath::Big<4,27> TTMathBig3;
	typedef TTMathBig3        TTMathBigMax;
	*/

	typedef ttmath::Big<TTMATH_BITS(32),  TTMATH_BITS(96)  > TTMathBig1;
	typedef ttmath::Big<TTMATH_BITS(64),  TTMATH_BITS(512) > TTMathBig2;
	typedef ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(1024)> TTMathBig3;
	typedef TTMathBig3 TTMathBigMax;

#else
	
	/* portable */

	typedef ttmath::Big<TTMATH_BITS(32), TTMATH_BITS(96)> TTMathBig1;
	typedef TTMathBig1 TTMathBigMax;

#endif





#endif
