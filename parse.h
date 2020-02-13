#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdio.h>

#include "parse_types.h"
 
/* macros */

#define catVariable (catAlpha | catDigit | catUnderscore)
#define catKeyword  (catAlpha | catDigit | catMinus)

#define parseLexThisVariable(p,s) parseLexThisCategory(p,catVariable,s)
#define parseLexVariable(p,s)     parseLexCategory(p,catVariable,s)

#define parseLexThisKeyword(p,s)  parseLexThisCategory(p,catKeyword,s)
#define parseLexKeyword(p,s)      parseLexCategory(p,catKeyword,s)

#define parseLexThisOper(p,s)     parseLexThisCategory(p,catOper,s)
#define parseLexOper(p,s)         parseLexCategory(p,catOper,s)

#define assign(ass)               (ass,1)
#define done                      (1)
#define fail                      (0)




#ifdef __GNUC__

#define many(p)                   ({while((p));1;})

#define maybe(p)                  (p,1)

#endif

/* types */

typedef enum
{
  catAlpha      = 1,
  catDigit      = 2,
  catOper       = 4,
  catBracket    = 8,
  catQuote      = 16,
  
  catMinus      = 32,
  catUnderscore = 64
} category;

typedef
  category
  *judgement;

struct _parse;

typedef
  bool
  (parseGetCharFunction)( struct _parse *, char * );

typedef
  bool
  (parseUnGetCharFunction)( struct _parse *, char );

typedef
  void
  (parseDisposeFunction)( struct _parse * );

typedef struct _parse
{
  char      *buf;
  int       bufptr;
  judgement judge;
  int       line;

  /* for FILE implementations */
  FILE      *file;

  /* for string implementations */
  char      *contents;
  int       ptr;
  
  parseGetCharFunction   *getChar;
  parseUnGetCharFunction *unGetChar;
  parseDisposeFunction   *dispose;
} parse;

/* functions */

parse *parseCreateFile( FILE *file );
parse *parseCreateFileName( char *name );
parse *parseCreateString( char *s );
void parseDispose( parse *p );

void parseSetJudgement( parse *p, char c, category );
void parseAddJudgement( parse *p, char c, category );
category parseGetJudgement( parse *p, char c );

bool parseError( parse *p, char *s );
bool parseEnd( parse *p );

bool parseAChar( parse *p, char *c );
bool parseAnyChar( parse *p );
bool parseThisChar( parse *p, char c );
bool parseUnParseChar( parse *p, char c );

bool parseThisString( parse *p, char *s );
bool parseUnParseString( parse *p, char *s );

bool parseDigit( parse *p, char *c );
bool parseNumber( parse *p, int *n );
bool parseSignedNumber( parse *p, int *n );

bool parseCharCategory( parse *p, category cat, char *c );

bool parseSpaceAndComments( parse *p );

bool parseLexThisCategory( parse *p, category cat, char *s );
bool parseLexCategory( parse *p, category cat, char *s );
bool parseLexNumber( parse *p, int *n );
bool parseLexSignedNumber( parse *p, int *n );
bool parseLexThisString( parse *p, char *s );
bool parseLexThisChar( parse *p, char c );
bool parseLexEnd( parse *p );

bool parseLexIfKeyword( parse *p, char *s, bool *b );
bool parseLexIfNumber( parse *p, int *n );
bool parseLexIfSignedNumber( parse *p, int *n );
bool parseLexKeywordNumber( parse *p, char *s, int *n );
bool parseLexKeywordSignedNumber( parse *p, char *s, int *n );

bool parseQuoted( parse *p, char *s );
bool parseSingleQuoted( parse *p, char *s );
bool parseLexQuoted( parse *p, char *s );

#endif /* PARSE_H */
