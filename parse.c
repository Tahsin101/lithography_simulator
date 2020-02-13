/* parse.c */

#include "parse.h"
#include "parse_new.h"
#include "parse_types.h"
#include "parse_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* macros */

#define BUFSIZE 300

/* functions */

static void parseInitJudgement( judgement judge )
{
  const char *oper    = "!@#$%^&*+-=|\\~:<>./";
  const char *bracket = "[]{}()";
  const char *quote   = "\"'`";
  char c;
  //int c;
  int i;
  
  for ( i = 0; i < 256; i++ )
    judge[i] = 0;
  
  for ( c = 'a'; c <= 'z'; c++ )
    judge[c] = judge[c] | catAlpha;
  
  for ( c = 'A'; c <= 'Z'; c++ )
    judge[c] = judge[c] | catAlpha;
  
  for ( c = '0'; c <= '9'; c++ )
    judge[c] = judge[c] | catDigit;
  
  for ( i = 0; c = oper[i], c != '\0'; i++ )
    judge[c] = judge[c] | catOper;
  
  for ( i = 0; c = bracket[i], c != '\0'; i++ )
    judge[c] = judge[c] | catBracket;
  
  for ( i = 0; c = quote[i], c != '\0'; i++ )
    judge[c] = judge[c] | catQuote;
  
  judge['_'] = judge['_'] | catUnderscore;
  judge['-'] = judge['-'] | catMinus;
}

static void parseStandardDisposeFunction( parse *p )
{
  free( p->buf );
  free( p->judge );
  free( p );
}

static bool parseStandardUnGetCharFunction( parse *p, char c )
{
  p->buf[p->bufptr] = c;
  
  if ( ++p->bufptr >= BUFSIZE )
  {
    p->bufptr--;
    return false;
  }
  
  if ( c == '\n' )
    p->line--;
  
  return true;
}

static void parseFileDisposeFunction( parse *p )
{
  free( p->buf );
  free( p->judge );
  fclose( p->file );
  free( p );
}

static bool parseFileGetCharFunction( parse *p, char *c )
{
  if ( p->bufptr )
    *c = p->buf[--p->bufptr];
  else
    *c = getc( p->file );
  
  if ( *c == '\n' )
    p->line++;
  
  return ( *c != EOF );
}

static bool parseStringGetCharFunction( parse *p, char *c )
{
  if ( p->bufptr )
    *c = p->buf[--p->bufptr];
  else if ( p->contents[p->ptr] != '\0' )
    *c = p->contents[p->ptr++];
  else
    return false;
  
  if ( *c == '\n' )
    p->line++;
  
  return true;
}

parse *parseCreateFile( FILE *file )
{
  parse *p     = new( parse );

  p->file      = file;
  p->buf       = array( char, BUFSIZE );
  p->bufptr    = 0;
  p->judge     = array( category, 256 );
  parseInitJudgement( p->judge );
  p->line      = 1;
  
  p->getChar   = parseFileGetCharFunction;
  p->unGetChar = parseStandardUnGetCharFunction;
  p->dispose   = parseStandardDisposeFunction;
  
  return p;
}

parse *parseCreateFileName( char *name )
{
  parse *p     = new( parse );
  //printf("new parse object %08X\n", (unsigned int) p);

  p->file      = fopen( name, "r" );
  if ( !p->file )
  {
    printf("Could not open file '%s' for parsing\n", name);
    free( p );
    return NULL;
  }

  p->buf       = array( char, BUFSIZE );
  p->bufptr    = 0;
  p->judge     = array( category, 256 );
  parseInitJudgement( p->judge );
  p->line      = 1;
  
  p->getChar   = parseFileGetCharFunction;
  p->unGetChar = parseStandardUnGetCharFunction;
  p->dispose   = parseFileDisposeFunction;
  
  return p;
}

parse *parseCreateString( char *contents )
{
  parse *p     = new( parse );

  p->contents  = contents;
  p->ptr       = 0;
  p->buf       = array( char, BUFSIZE );
  p->bufptr    = 0;
  p->judge     = array( category, 256 );
  parseInitJudgement( p->judge );
  p->line      = 1;
  
  p->getChar   = parseStringGetCharFunction;
  p->unGetChar = parseStandardUnGetCharFunction;
  p->dispose   = parseStandardDisposeFunction;
  
  return p;
}

void parseSetJudgement( parse *p, char c, category cat )
{
  p->judge[c] = cat;
}

void parseAddJudgement( parse *p, char c, category cat )
{
  p->judge[c] = p->judge[c] | cat;
}

category parseGetJudgement( parse *p, char c )
{
  return p->judge[c];
}

bool parseError( parse *p, char *s )
{
  char rest[300];
  int i;
  
  for ( i = 0
      ; parseAChar( p, rest+i ) && rest[i] != '\n' && i < 50
      ; i++
      );
  
  rest[i]   = '\n';
  rest[i+1] = '\0';
  
  errorReportExt( ("parse error, line %d: %s\n... %s", p->line, s, rest) );
  return true;
}

bool parseEnd( parse *p )
{
  char c;
  return ( !parseAChar( p, &c )
        || !parseUnParseChar( p, c )
         );
}

bool parseAChar( parse *p, char *c )
{
  return p->getChar( p, c );
}

bool parseAnyChar( parse *p )
{
  char c;
  return parseAChar( p, &c );
}

bool parseThisChar( parse *p, char c )
{
  char d;
  if ( !p->getChar( p, &d ) )
    return false;
  
  if ( d == c )
    return true;
  
  parseUnParseChar( p, d );
  return false;
}

bool parseUnParseChar( parse *p, char c )
{
  return p->unGetChar( p, c );
}

bool parseThisString( parse *p, char *s )
{
  int i;
  char c;
  
  for ( i = 0
      ; s[i] != '\0' && parseThisChar( p, s[i] )
      ; i++
      );
  
  if ( s[i] == '\0' )
    return true;
  
  for ( i-- ; i >= 0; i-- )
    parseUnParseChar( p, s[i] );
  
  return false;
}

bool parseUnParseString( parse *p, char *s )
{
  int i;
  
  for ( i = strlen(s)-1; i >= 0; i-- )
    parseUnParseChar( p, s[i] );
  
  return true;
}

bool parseSpaceAndComments( parse *p )
{
  while ( 1 )
  {
    if ( parseThisChar( p, ' ' )             
      || parseThisChar( p, '\t' )
      || parseThisChar( p, '\n' )
       )
      ;
    
    else if ( parseThisString( p, "/*" ) )
      while ( !parseThisString( p, "*/" )
           && parseAnyChar( p )
            );
 
    else if ( parseThisString( p, "//" ) )
      while ( !parseThisChar( p, '\n' )
           && parseAnyChar( p )
            );

    else
      break;
  }
          
  return true;
}

bool parseDigit( parse *p, char *c )
{
  return ( parseAChar( p, c )
        && ( ('0' <= *c && *c <= '9')
          || parseUnParseChar( p, *c )
          && false
           )
         );
}

bool parseNumber( parse *p, int *n )
{
  int i = 0;
  char s[300];
  
  while ( parseDigit( p, &(s[i]) )
       && assign ( i++ )
        );
  
  return ( i > 0
        && assign( s[i] = '\0' )
        && assign( *n = atoi( s ) )
         );
}

bool parseSignedNumber( parse *p, int *n )
{
  return ( parseThisChar( p, '-' )
        && parseLexNumber( p, n )
        && assign( *n = -(*n) )
        
        || parseThisChar( p, '+' )
        && parseLexNumber( p, n )
        
        || parseNumber( p, n )
         );
}

bool parseLexNumber( parse *p, int *n )
{
  return ( parseSpaceAndComments( p )
        && parseNumber( p, n )
         );
}

bool parseLexSignedNumber( parse *p, int *n )
{
  return ( parseSpaceAndComments( p )
        && parseSignedNumber( p, n )
         );
}

bool parseCharCategory( parse *p, category cat, char *c )
{
  return ( parseAChar( p, c )
        && ( ( cat & p->judge[*c] )
          || !parseUnParseChar( p, *c )
           )
         );
}

bool parseLexThisCategory( parse *p, category cat, char *s )
{
  int i;
  char c;
  
  parseSpaceAndComments( p );
  
  for ( i = 0
      ; s[i] != '\0' && parseThisChar( p, s[i] )
      ; i++
      );
  
  if ( s[i] == '\0' )
    if ( !parseAChar( p, &c )
      || parseUnParseChar( p, c )
      && !( cat & p->judge[c] )
       )
    {
      return true;
    }
  
  for ( i-- ; i >= 0; i-- )
    parseUnParseChar( p, s[i] );
  
  return false;
}

bool parseLexCategory( parse *p, category cat, char *s )
{
  int i = 0;
  
  parseSpaceAndComments( p );
  
  while ( parseCharCategory( p, cat, s+i )
       && assign ( i++ )
        );
  
  return ( i > 0
        && assign( s[i] = '\0' )
         );
}

bool parseLexThisString( parse *p, char *s )
{
  return ( parseSpaceAndComments( p )
        && parseThisString( p, s )
         );
}

bool parseLexThisChar( parse *p, char c )
{
  return ( parseSpaceAndComments( p )
        && parseThisChar( p, c )
         );
}

bool parseLexEnd( parse *p )
{
  return ( parseSpaceAndComments( p )
        && parseEnd( p )
         );
}

bool parseLexIfKeyword( parse *p, char *s, bool *b )
{
  return ( parseLexThisKeyword( p, s )
        && assign ( *b = true )
         );
}

bool parseLexIfNumber( parse *p, int *n )
{
  int m;
  return ( parseLexNumber( p, &m )
        && assign ( *n = m )
         );
}

bool parseLexIfSignedNumber( parse *p, int *n )
{
  int m;
  return ( parseLexSignedNumber( p, &m )
        && assign ( *n = m )
         );
}

bool parseLexKeywordNumber( parse *p, char *s, int *n )
{
  return ( parseLexThisKeyword( p, s )
        && ( parseLexNumber( p, n )
          || parseError( p, "no argument given." )
           )
         );
}

bool parseLexKeywordSignedNumber( parse *p, char *s, int *n )
{
  return ( parseLexThisKeyword( p, s )
        && ( parseLexSignedNumber( p, n )
          || parseError( p, "no argument given." )
           )
         );
}

bool parseQuoted( parse *p, char *name )
{
  char c;
  int i = 0;
  
  if ( parseLexThisChar( p, '"' ) )
  {
    while ( parseAChar( p, &c )
         && c != '"'
         && c != '\n'
         && assign ( name[i++] = c )
          );
    if ( c == '"' )
    {
      name[i] = '\0';
      return true;
    }
    else
      parseError( p, "Could not parse quoted string" );
  }
  
  return false;
}
bool parseSingleQuoted( parse *p, char *name )
{
  char c;
  int i = 0;
  
  if ( parseLexThisChar( p, '\'' ) )
  {
    while ( parseAChar( p, &c )
         && c != '\''
         && c != '\n'
         && assign ( name[i++] = c )
          );
    if ( c == '\'' )
    {
      name[i] = '\0';
      return true;
    }
    else
      parseError( p, "Could not parse single quoted string" );
  }
  
  return false;
}

bool parseLexQuoted( parse *p, char *s )
{
  return ( parseSpaceAndComments( p )
        && parseQuoted( p, s )
         );
}

void parseDispose( parse *p )
{
  p->dispose( p );
}

/* end parse.c */
