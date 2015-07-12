Brainfuck
=========

This repository contains various [brainfuck](http://esolangs.org/wiki/Brainfuck) stuff.

The classic description of BF is the C translation below, the opcode column is the direct translation used by my Tritium interpreter.

| Opcode | Brainfuck   | C                  |
| -------|:-----------:|--------------------|
| T_MOV  | `>`         | ++ptr;             |
| T_MOV  | `<`         | --ptr;             |
| T_ADD  | `+`         | ++*ptr;            |
| T_ADD  | `-`         | --*ptr;            |
| T_WHL  | `[`         | while (*ptr) {     |
| T_END  | `]`         | }                  |
| T_PRT  | `.`         | putchar(*ptr);     |
| T_INP  | `,`         | *ptr = getchar();  |

There are several BF interpreters and tools in this repository.

1. The BF program "bitwidth.b"; this is something of a torture test, if your interpreter correctly executes this anything else is likely to be dead easy. 

  But the official output of the Bitwidth program is a display of the cell size ie:

  <dl><dt>8 Bit cells<dd>Hello World! 255<dt>16 Bit cells<dd>Hello world! 65535<dt>32 bit cells<dd>Hello, world!</dl>

  Though before you try it you might like to try this slightly less nasty Hello World! 

  ```brainfuck
  1 []><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[]
  2 []>+>+>++>++<[>[->++++<<+++>]<<]>----.>->+.+++++++..+++.<+[]
  3 [ This is hellbox, a 104 command Hello World               ]
  4 [   >+>+>++>++<[>[->++++<<+++>]<<]>----.>>+.+++++++..+++   ]
  5 [   .>.<<<+++++++++++++++.>>.+++.------.--------.>+.>++.   ]
  6 [ -- Robert de Bath -- 2014                                ]
  7 []>>.<<<+++++++++++++++.>>.+++.------.--------.>+.+>++.<<<[]
  8 []><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[][]><[]
  ```
  
  Lines 2 and 7 are the code that should be run, lines 1 and 8 are decorative and lines 3 to 6 can contain anything where each line is enclosed in [] and contains balanced brackets.

2. VIM Syntax highlighting file for brainfuck.

  This file highlights the eight BF command characters in four (reasonable) classes. This is the simple part.

  In addition it highlights SOME of the other characters as comments.  Generally it tries (with moderate success) to distinguish between proper comments and sequences that are probably supposed to be comments but actually contain active BF command characters. In addition it tries to identify the 'dead code' style comment loops highlighting any BF command characters within the loop in the 'PreProc' style to distinguish them from commands that may actually be executed.

3. Tritium (officially Ρ‴) this BF interpreter/compiler/JIT runner makes other programs look slow. It is simply the fastest BF interpreter you'll find.

4. Brainfuck to anything. Well not exactly anything but the list includes ...
  * run -- a direct interpreter -- blisteringly quick too for one without JIT.
  * jit -- OTOH this one uses LuaJIT's Dynasm, it's the fastest bf2any program.
  * crun -- Convert to C and run using libtcc or libdl. TCC quicker than bf2run ... just, GCC is a lot quicker, even without GCC doing any optimisation.
  * bf -- Ook, Blub, fuck fuck, "there once was a fish named Fred" and similar transliterations. Also includes Cell doubler (and quad) mappings. Some can be compiled as C (most are *deoptimised* but -rle is not.) The optimiser can still run so this can be used as a BF->BF optimiser. For BF->BF optimisation best results are got with -Obf.
  * asmjs -- Convert to the "asm.js" dialect of javascript
  * awk	-- Code for (almost) any version of AWK.
  * bash -- GNU bash, uses arrays, arithimetic etc. (NO external programs or subshells used)
  * basic -- A couple of very random BASIC interpreters.
  * cgmp -- C using the Gnu MP library. This uses a linked list so the pointer movements are slower. 
  * clojure -- Not a very nice conversion though. *no-opt*
  * cmd -- Windows batch files ... far too slow for testing *no-opt*
  * dc -- The unix command, has a -r (run) option that uses a special filter to allow character input. This is too complex for dc.sed.
  * d -- The C replacement originally by 'Digital Mars'
  * elf -- Direct production of a 32bit Linux executable. *no-opt*
  * gas -- x64 or x86 assembler. Use gcc to assemble and link: "gcc -o bfp bfout.s" *no-opt*
  * go -- Google's modern language
  * julia -- [A modern language using LLVM](http://julialang.org)
  * lua
  * neko -- [Neko programming language VM](http://nekovm.org)
  * navision -- Now called Microsoft DynamicsNAV
  * pascal -- Free pascal.
  * perl
  * php
  * ps1 -- That's right MS Powershell
  * python
  * rc -- The Plan9 shell rc(1) (Can't input without external programs.) -- *deoptimised*
  * ruby
  * tcl
  * s-lang
  * sh -- Bourne shell without bash extensions, not Unix v7 but later should be fine.
  * v7sh -- Bourne shell from Unix v7. -- *deoptimised*

  Most are heavily optimised (for Brainfuck) and most work in both 8 bit and the native size of the generated code.
  The ones marked *no-opt* are not properly optimised as the BE can't generate all the needed op-codes. The ones marked *deoptimised* have even the RLE reverted. The FE can still optimise in all cases, but without the BE optimisation only constant folding will occur.
  They have all been tested using many of the BF programs from the [Esoteric Files Archive](https://github.com/graue/esofiles/tree/master/brainfuck/src) (And of course tortured!)

5. The 'extras' subdirectory.

6. The 'umueller' directory contains the second brainfuck compiler written by Urban Müller.
  It was written in June 1993.  The previous one was 296 bytes long, used longword cells (That's 32bits on the Amiga) and did not require 'Amiga OS 2.0'.

Copyrights
==========

The bf.vim, bf2any and Tritium programs are licensed under the GPLv2 or later versions if you prefer.

The programs bf2bash.sh, bf2c.awk and all brainfuck source code, including any
embedded in the C code are explicitly granted public domain status.

The code in the tools and the testing directories have their own licensing.
