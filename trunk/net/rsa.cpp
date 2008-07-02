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

#include <stdlib.h> // REMOVE ME as well
#include <string.h> // memset
#include "../util.h" // REMOVE ME after fixing bignum=>binary
#include "rsa.h"
#include "../fassert.h"
#include "../debugprint.h"
RSA::RSA()
{
    #ifdef HAVE_GMP_H
	mpz_init2(m_mod, 1024);
	mpz_init2(m_e, 1024);
	#endif
	m_keyset = false;
}

RSA::~RSA()
{
    #ifdef HAVE_GMP_H
	mpz_clear(m_e);
	mpz_clear(m_mod);
	#endif
}

void RSA::setPublicKey(const char* m, const char* e)
{
	m_keyset = true;
	#ifdef HAVE_GMP_H
	mpz_set_str(m_mod, m, 10);
	mpz_set_str(m_e, e, 10);
	#else
 	m_mod = m;
	m_e = e;
	#endif
}

bool RSA::encrypt(char* msg, int32_t size)
{

	ASSERT(m_keyset == true);

	#ifdef HAVE_GMP_H
	mpz_t plain,c;
	mpz_init2(plain, 1024);
	mpz_init2(c, 1024);

	mpz_import(plain, 128, 1, 1, 0, 0, msg);

	mpz_powm(c, plain, m_e, m_mod);

	size_t count = (mpz_sizeinbase(c, 2) + 7)/8;
	memset(msg, 0, 128 - count);
	mpz_export(&msg[128 - count], NULL, 1, 1, 0, 0, c);

	mpz_clear(c);
	mpz_clear(plain);
    #else
    BigInt plain(msg),c;

    c = t_power(plain, m_e);
    c = t_modulo(c, m_mod);

    /* FIXME (ivucica#2#) we need to add bignum=>binary (meaning, convert from internal base into base 256) */
    NativeGUIError("Please recompile with -lgmp and with #define HAVE_GMP_H. Internal bignums are not done yet. Thank you!", "Bignums not done");
    exit(EXIT_FAILURE);


    /*size_t count = (length(c, 2) + 7)/8;
	memset(msg, 0, 128 - count);
    memcpy(&msg[128 - count], c.toString().c_str(), 128 - count);*/
    #endif
	return true;
}
