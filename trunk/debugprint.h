#ifndef DEBUGPRINT
#include <string>


#define DEBUGPRINT /*DEBUG_LINE = __LINE__; DEBUG_FILE = __FILE__; */DEBUGPRINTx
// debugprint types
#define DEBUGPRINT_NORMAL 0
#define DEBUGPRINT_ERROR 1
#define DEBUGPRINT_WARNING 2
// debugprint levels
#define DEBUGPRINT_LEVEL_OBLIGATORY 0
#define DEBUGPRINT_LEVEL_USEFUL 1
#define DEBUGPRINT_LEVEL_DEBUGGING 2
#define DEBUGPRINT_LEVEL_JUNK 3
extern char debuglevel;
void DEBUGPRINTx (char msgdebuglevel, char type, char* txt, ...);
extern std::string DEBUG_FILE; extern int DEBUG_LINE;
void DEBUGMARKER (unsigned int size, const char *val);

#endif
