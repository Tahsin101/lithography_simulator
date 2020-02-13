/* print.c */

#include "parse_print.h"
#include "parse_types.h"
#include "parse_new.h"
#include <stdio.h>
#include <stdarg.h>

/* global variables */

FILE *printGlobalStdOut;

/* functions */

print *printCreateFile( FILE *file )
{
  print *p = new( print );
  p->file  = file;
  p->close = false;
  return p;
}

print *printCreateFileName( char *name )
{
  print *p = new( print );
  if ( !(p->file  = fopen( name, "w" )) )
  {
    free( p );
    return NULL;
  }
  p->close = true;
  return p;
}

print *printCreateFileNameAppend( char *name )
{
  print *p = new( print );
  if ( !(p->file  = fopen( name, "a" )) )
  {
    free( p );
    return NULL;
  }
  p->close = true;
  return p;
}

void printFormatted( char *s, ... )
{
  va_list ap;
  va_start( ap, s );
  vfprintf( printGlobalStdOut, s, ap );
  va_end( ap );
}

void printDispose( print *p )
{
  if ( p->close )
    fclose( p->file );
  free( p );
}

/* end print.c */
