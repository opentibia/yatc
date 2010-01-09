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
    #ifdef HAVE_TOMMATH
	mp_init_size(&m_mod, 1024);
	mp_init_size(&m_e, 1024);
	#endif
	m_keyset = false;
}

RSA::~RSA()
{
    #ifdef HAVE_TOMMATH
	mp_clear(&m_e);
	mp_clear(&m_mod);
	#endif
}

void RSA::setPublicKey(const char* m, const char* e)
{
	m_keyset = true;
	#ifdef HAVE_TOMMATH
	mp_read_radix(&m_mod, m, 10);
	mp_read_radix(&m_e, e, 10);
	#else
 	m_mod = m;
	m_e = e;
	#endif
}

bool RSA::encrypt(char* msg, int32_t size)
{

	ASSERT(m_keyset == true);

	#ifdef HAVE_TOMMATH
	mp_int plain,c;
	mp_init_size(&plain, 1024);
	mp_init_size(&c, 1024);

	mp_read_unsigned_bin(&plain, (unsigned char*)msg, 128);

	mp_exptmod(&plain, &m_e, &m_mod, &c);

	//size_t count = (mpz_sizeinbase(c, 2) + 7)/8;
	//memset(msg, 0, 128 - count);
	mp_to_unsigned_bin(&c, (unsigned char*)msg);

	mp_clear(&c);
	mp_clear(&plain);
    #else
    BigInt plain(msg),c;

    c = t_power(plain, m_e);
    c = t_modulo(c, m_mod);

    /* FIXME (ivucica#2#) we need to add bignum=>binary (meaning, convert from internal base into base 256) */
    NativeGUIError("Please recompile with -lgmp and with #define HAVE_TOMMATH. Internal bignums are not done yet. Thank you!", "Bignums not done");
    exit(EXIT_FAILURE);


    /*size_t count = (length(c, 2) + 7)/8;
	memset(msg, 0, 128 - count);
    memcpy(&msg[128 - count], c.toString().c_str(), 128 - count);*/
    #endif
	return true;
}
