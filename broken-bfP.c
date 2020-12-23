#include <stdio.h>
#include <string.h>
int jmpstk[200], sp = 0;

int
main(int argc, char **argv){
    static char pgm[BUFSIZ*1024];
    static unsigned char mem[65536];
    int m = 0;
    int p=0, ch;
    FILE * f = fopen(argv[1],"r");
    while((ch=getc(f)) != EOF) if(strchr("+-<>[].,",ch)) pgm[p++] = ch;
    pgm[p++] = 0;
    fclose(f);
    setbuf(stdout,0);
    for(p=0;pgm[p];p++) {
	switch(pgm[p]) {
	case '+': mem[m]++; break;
	case '-': mem[m]--; break;
	case '>': m++; if (m>30000) m -= 30000; break;
	case '<': m--; if (m<0) m += 30000; break;
	case '.': putchar(mem[m]); break;
	case ',': {int a=getchar(); if(a!=EOF) mem[m]=a;} break;
	case ']':
	    if (mem[m] == 0) {
		if(sp) sp--;
	    } else {
		if(sp) { sp--; p=jmpstk[sp]-1; } else p = -1;
	    }
	    break;
	case '[':
	    if (sp>sizeof(jmpstk)/sizeof(*jmpstk))
		return fprintf(stderr, "Stack overflow\n");
	    jmpstk[sp++] = p;
	    if(mem[m] == 0) {
		int i = 0;
		while((i+=(pgm[p]=='[')-(pgm[p]==']'))&&pgm[p+1])p++;
	    }
	    break;
	}
    }
}