//
//  OCCharSet.cpp
//  oclex
//
//  Created by William Woody on 7/26/17.
//  Copyright © 2017 Glenview Software. All rights reserved.
//

#include "OCCharSet.h"
#include <string.h>

/************************************************************************/
/*																		*/
/*	Manipulation														*/
/*																		*/
/************************************************************************/

/*	OCCharSet::Clear
 *
 *		Clear
 */

void OCCharSet::Clear()
{
	memset(a,0,sizeof(a));
}

/*	OCCharSet::Invert
 *
 *		Negate bit array
 */

void OCCharSet::Invert()
{
	for (int i = 0; i < 8; ++i) {
		a[i] = ~a[i];
	}
}

/*	OCCharSet::ClearCharacter
 *
 *		Clear character
 */

void OCCharSet::ClearCharacter(unsigned char ch)
{
	a[0x07 & (ch >> 5)] &= ~(1UL << (0x1F & ch));
}

/*	OCCharSet::SetCharacter
 *
 *		Set character
 */

void OCCharSet::SetCharacter(unsigned char ch)
{
	a[0x07 & (ch >> 5)] |= 1UL << (0x1F & ch);
}

/*	OCCharSet::TestCharacter
 *
 *		Teset if character is set
 */

bool OCCharSet::TestCharacter(unsigned char ch)
{
	return	(a[0x07 & (ch >> 5)] & 1UL << (0x1F & ch)) ? true : false;
}

/************************************************************************/
/*																		*/
/*	Math																*/
/*																		*/
/************************************************************************/

/*	OCCharSet::operator &=
 *
 *		And and return result
 */

OCCharSet &OCCharSet::operator &= (const OCCharSet &cset)
{
	for (int i = 0; i < 8; ++i) {
		a[i] &= cset.a[i];
	}

	return *this;
}

/*	OCCharSet::operator |=
 *
 *		And and return result
 */

OCCharSet &OCCharSet::operator |= (const OCCharSet &cset)
{
	for (int i = 0; i < 8; ++i) {
		a[i] |= cset.a[i];
	}

	return *this;
}

/*	OCCharSet::operator -=
 *
 *		Return the set of characters in the source with the characters in
 *	the destination removed.
 *
 *		Equal to
 *
 *			a = a & ~b
 */

OCCharSet &OCCharSet::operator -= (const OCCharSet &cset)
{
	for (int i = 0; i < 8; ++i) {
		a[i] &= ~cset.a[i];
	}

	return *this;
}

