README - a brief documentation on the package (student version)
Jan. 2015
by Guoyong SHI: shiguoyong@ic.sjtu.edu.cn

This circuit parser provides an interface to MATLAB so that students can
directly use MATLAB for circuit simulation.

This package contains the following files:
	Makefile
	parse.lex
	parse.y
	parse_func.h[.c]
	Symbol_Table.h[.c]
	MNA_Matrix.h[.c]
	main.c  -- the Linux OS driver

You should have installed CYGWIN (or a Linux equivalent) to
compile the flex-bison programs and run makefile.
You should also have installed a version of MATLAB on Windows or Linux.

The programs "parse.lex" and "parse.y" are compiled in CYGWIN by running
"make".

Then in MATLAB, run the mex compiler as follows:
	mex -DMATLAB mex_parse.c parse_func.c Symbol_Table.c MNA_Matrix.c

To run the parser in Linux, compile as follows:

	make parse
	make runparse

Depending on your need, the reader is expected to add more C-programs.

The basic funtionalities are

"parse_func.h[.c]" provide the parsing functions that
interfacing from the parser "parse.y" and the symbol tables;

"Symbol_Table.h[.c]" manages the symbol tables. 

"MNA_Matrix.h[.c]" manages the matrices resulting from parsing a netlist.

This package is incomplete and only provides a code skeleton for
continuing development.
The details on the necessary components will be introduced in the course.

For more information on this package, please consult TA or instructor.


