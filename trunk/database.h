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

#ifndef __DATABASE_H
#define __DATABASE_H

#ifndef SQLITE_OLD
	#include <sqlite3.h>
#else
	#include <sqlite.h>

	#define sqlite3 sqlite
	#define sqlite3_callback sqlite_callback
	#define sqlite3_exec sqlite_exec
	#define sqlite3_close sqlite_close
	//#define sqlite3_open(x,y) sqlite_open(x,y,NULL)
	#define sqlite3_errmsg(x) "(old sqlite, error unknown)"
	//sqlite_errmsg(x,NULL)
	#define sqlite3_vmprintf sqlite_vmprintf
	#define sqlite3_free free
#endif

void DBInit();
void DBDeinit();
bool dbTableExists(sqlite3 *db, const char *tablename);
int dbExecPrintf(
  sqlite3*,                     /* An open database */
  sqlite3_callback,             /* Callback function */
  void *,                       /* 1st argument to callback function */
  char **errmsg,                /* Error msg written here */
  const char *sql,              /* SQL to be executed */
  ...);

#define dbExec sqlite3_exec
bool dbLoadSetting(const char* settingname, char* valuetarget, int maxlen, const char *defaultval);
void dbSaveSetting(const char* settingname, const char* value);
const char* dbProcessRC(int rc);
extern sqlite3 *dbData, *dbUser;
#endif
