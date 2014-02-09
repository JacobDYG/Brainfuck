#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf2any.h"

/*
 * Lua translation from BF, runs at about 39,000,000 instructions per second.
 * LuaJIT translation from BF, runs at about 360,000,000 instructions per second.
 *
 * There is a limit on the size of a while loop imposed by the interpreter.
 * The while loop's jump has a limited range, so if there are lots of tokens
 * in the loop I make the loop body a function.
 */

#define MAXWHILE 2048

struct instruction {
    int ch;
    int count;
    int ino;
    int icount;
    struct instruction * next;
    struct instruction * loop;
} *pgm = 0, *last = 0, *jmpstack = 0;

void loutcmd(int ch, int count, struct instruction *n);

int do_input = 0;
int ind = 0;
#define I printf("%*s", ind*4, "")
static int lblcount = 0;
static int icount = 0;

int
check_arg(const char * arg)
{
    if (strcmp(arg, "-O") == 0) return 1;
    return 0;
}

void
outcmd(int ch, int count)
{
    struct instruction * n = calloc(1, sizeof*n), *n2;
    if (!n) { perror("bf2lua"); exit(42); }

    icount ++;
    n->ch = ch;
    n->count = count;
    n->icount = icount;
    if (!last) {
	pgm = n;
    } else {
	last->next = n;
    }
    last = n;

    if (n->ch == '[') {
	n->ino = ++lblcount;
	n->loop = jmpstack; jmpstack = n;
    } else if (n->ch == ']') {
	n->loop = jmpstack; jmpstack = jmpstack->loop; n->loop->loop = n;
    }

    if (ch != '~') return;

    printf("#!/usr/bin/lua\n");
    printf("local m = setmetatable({},{__index=function() return 0 end})\n");
    printf("local p = %d\n", BOFF);
    printf("local v = 0\n");
    printf("\n");

    if (icount < MAXWHILE) {
	for(n=pgm; n; n=n->next)
	    loutcmd(n->ch, n->count, n);
    } else {
	for(n=pgm; n; n=n->next) {
	    if (n->ch != ']') continue;
	    if (n->icount-n->loop->icount <= MAXWHILE) continue;
	    loutcmd(1000, 1, n->loop);
	    for(n2 = n->loop->next; n != n2; n2=n2->next) {
		if (n2->ch == '[' && n2->loop->icount-n2->icount > MAXWHILE) {
		    loutcmd(n2->ch, n2->count, n);
		    I; printf("bf%d()\n", n2->ino);
		    n2 = n2->loop;
		    loutcmd(n2->ch, n2->count, n);
		} else
		    loutcmd(n2->ch, n2->count, n2);
	    }
	    loutcmd(1001, 1, n);
	}

	for(n=pgm; n; n=n->next) {
	    if (n->ch != '[' || n->loop->icount-n->icount <= MAXWHILE)
		loutcmd(n->ch, n->count, n);
	    else {
		loutcmd(n->ch, n->count, n);
		I; printf("bf%d()\n", n->ino);
		n=n->loop;
		loutcmd(n->ch, n->count, n);
	    }
	}
    }

    while(pgm) {
	n = pgm;
	pgm = pgm->next;
	memset(n, '\0', sizeof*n);
	free(n);
    }
}

void
loutcmd(int ch, int count, struct instruction *n)
{
    switch(ch) {
    case 1000:
	printf("function bf%d()\n", n->ino);
	ind++;
	break;
    case 1001:
	ind--;
	printf("end\n\n");
	break;

    case '!':
	printf("function brainfuck()\n");
	ind++;
	break;

    case '=': I; printf("m[p] = %d\n", count); break;
    case 'B':
	if(bytecell) { I; printf("m[p] = m[p]%%256\n"); }
	I; printf("v = m[p]\n");
	break;
    case 'M': I; printf("m[p] = m[p]+v*%d\n", count); break;
    case 'N': I; printf("m[p] = m[p]-v*%d\n", count); break;
    case 'S': I; printf("m[p] = m[p]+v\n"); break;
    case 'Q': I; printf("if v ~= 0 then m[p] = %d end\n", count); break;
    case 'm': I; printf("if v ~= 0 then m[p] = m[p]+v*%d end\n", count); break;
    case 'n': I; printf("if v ~= 0 then m[p] = m[p]-v*%d end\n", count); break;
    case 's': I; printf("if v ~= 0 then m[p] = m[p]+v end\n"); break;

    case 'X': I; printf("error('Aborting Infinite Loop')\n"); break;

    case '+': I; printf("m[p] = m[p]+%d\n", count); break;
    case '-': I; printf("m[p] = m[p]-%d\n", count); break;
    case '>': I; printf("p = p+%d\n", count); break;
    case '<': I; printf("p = p-%d\n", count); break;
    case '.': I; printf("putch()\n"); break;
    case ',': I; printf("getch()\n"); do_input++; break;

    case '[':
	if(bytecell) { I; printf("m[p] = m[p]%%256\n"); }
	I; printf("while m[p]~=0 do\n");
	ind++;
	break;
    case ']':
	if(bytecell) { I; printf("m[p] = m[p]%%256\n"); }
	ind--; I; printf("end\n");
	break;

    case '~':
	ind--;
	printf("end\n");

	if (do_input) {
	    printf("\n");
	    printf("function getch()\n");
	    printf("    local Char = io.read(1)\n");
	    printf("    if Char then\n");
	    printf("        m[p] = string.byte(Char)\n");
	    printf("    end\n");
	    printf("end\n");
	}

	printf("\n");
	printf("function putch()\n");
	printf("    io.write(string.char(m[p]%%256))\n");
	printf("    io.flush()\n");
	printf("end\n");

	printf("\n");
	printf("brainfuck()\n");
	break;
    }
}
