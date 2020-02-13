#ifndef __GLPREADER_H__
#define __GLPREADER_H__

#ifdef __cplusplus
extern "C" {
#endif


// shorthands for the parser
#define KWD(k) (parseThisString(p, k) && parseSpaceAndComments(p) )
#define NAT(n) (parseNumber(p,     &n) &&  parseSpaceAndComments(p) )
#define INT(n) ((KWD("-") && NAT(n) && assign(n = - n)) ||	\
		(KWD("+") && NAT(n) ) ||			\
		(NAT(n)))
#define STR(s) (parseSingleQuoted(p, s) && parseSpaceAndComments(p))
#define ID(id) (parseLexVariable(p,id) && parseSpaceAndComments(p))
#define END parseEnd(p)



int glpParseFile(char *fname,
	      int (*er)(char *l,int x, int y, int w, int h), 
	      int (*ep)(int (*er)(char *l,int x, int y, int w, int h), char *l,int n, int *xy), 
	      int (*el)(int (*er)(char *l,int x, int y, int w, int h), char *l,int w, int n, int* xy), 
	      int (*ea)(char *l,int x, int y, int w, int h, char *v)

		 );

#ifdef __cplusplus
}
#endif

#endif
