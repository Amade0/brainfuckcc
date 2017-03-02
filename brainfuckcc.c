/* brainfuckcc/brainfuckcc.c
 * 2 Mar 2017
 * vim: noet sw=8 ts=8 tw=78
 */

#include <stdio.h>
#include <stdlib.h>

FILE *src_fp, *dst_fp;

void open_fps(char *argv[])
{
	char *dst_fn;
	// open source file
	src_fp = fopen(argv[1], "r");
	if (NULL == src_fp) {
		printf("Fatal error: could not open %s for reading", argv[1]);
		exit(EXIT_FAILURE);
	}

	// open destination file (default: a.c)
	if (NULL != argv[2]) {
		dst_fn = argv[2];
	} else {
		dst_fn = "a.c";
	}
	dst_fp = fopen(dst_fn, "w");
	if (NULL == dst_fp) {
		printf("Fatal error: could not open %s for writing", dst_fn);
		exit(EXIT_FAILURE);
	}
}

void close_fps()
{
	fclose(src_fp);
	fclose(dst_fp);
}

void main_compilation()
{
	int brace_stack = 0;
	char c;
	while (EOF != (c = fgetc(src_fp))) {
		switch (c) {
			case '>':
				fputs("\tp++;\n", dst_fp);
				break;
			case '<':
				fputs("\tp--;\n", dst_fp);
				break;
			case '+':
				fputs("\t(*p)++;\n", dst_fp);
				break;
			case '-':
				fputs("\t(*p)--;\n", dst_fp);
				break;
			case '.':
				fputs("\tputchar(*p);\n", dst_fp);
				break;
			case ',':
				fputs("\t*p = getchar();\n", dst_fp);
				break;
			case '[':
				brace_stack++;
				fputs("\twhile(*p) {\n", dst_fp);
				break;
			case ']':
				brace_stack--;
				fputs("\t}\n", dst_fp);
				break;
		}
	}
	if (0 != brace_stack) {
		puts("Warning: number of [ and ] nonequal\n");
	}
}

void init_compilation()
{
	// include boilerplate for brainfuck environment
	fputs("#include <string.h>\n", dst_fp);
	fputs("#include <stdio.h>\n", dst_fp);
	fputs("char c[30000];\n", dst_fp);
	fputs("char *p = &c[15000];\n", dst_fp);
	fputs("int main()\n{\n", dst_fp);
	fputs("\tmemset(c, '\\0', 30000);\n", dst_fp);
	// interpret the source file
	main_compilation();
	// close the program
	fputs("\treturn 0;\n}", dst_fp);
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		puts("Usage: brainfuckcc input.bf [output.c]");
		exit(EXIT_SUCCESS);
	}

	open_fps(argv);
	init_compilation();
	close_fps();

	return 0;
}
