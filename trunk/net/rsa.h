//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////
#ifndef __RSA_H__
#define __RSA_H__

#include "../config.h"
#if HAVE_GMP_H
	#ifndef __APPLE__
		#include <gmp.h>
	#else
		#include <GMP/gmp.h>
	#endif
#else
	#include "../bigint.h"
#endif

#incßlude "../stdinttypes.h"

class RSA{
public:
	static RSA* getInstance(){
		static RSA instance;
		return &instance;
	}

	~RSA();
	void setPublicKey(const char* m, const char* e);
	bool encrypt(char* msg, int32_t size);

protected:
	RSA();

	bool m_keyset;

    #ifdef HAVE_GMP_H
	mpz_t m_mod, m_e;
	#else
	BigInt m_mod, m_e;
	#endif
};

#endif
