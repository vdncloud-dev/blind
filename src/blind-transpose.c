/* See LICENSE file for copyright and license details. */
#include "stream.h"
#include "util.h"

USAGE("")

static size_t srcw, srch, srcwps, srchps, ps;

#define PROCESS(TYPE)\
	do {\
		size_t x, i, n = ps / sizeof(TYPE);\
		char *src, *img;\
		for (x = 0; x < srchps; x += ps) {\
			img = row + x;\
			src = col + x * srcw;\
			for (i = 0; i < n; i++)\
				((TYPE *)img)[i] = ((TYPE *)src)[i];\
		}\
	} while (0)

static void process_long(char *row, char *col) {PROCESS(long);}
static void process_char(char *row, char *col) {PROCESS(char);}

int
main(int argc, char *argv[])
{
	struct stream stream;
	char *buf, *image;
	size_t y;
	void (*process)(char *row, char *col);

	UNOFLAGS(argc);

	eopen_stream(&stream, NULL);
	echeck_dimensions(&stream, WIDTH | HEIGHT, NULL);
	srch = stream.height, srchps = stream.col_size;
	srcw = stream.width,  srcwps = stream.row_size;
	stream.height = srcw;
	stream.width  = srch;
	fprint_stream_head(stdout, &stream);
	efflush(stdout, "<stdout>");

	buf   = emalloc(stream.frame_size);
	image = emalloc(srchps);

	process = ps % sizeof(long) ? process_char : process_long;
	while (eread_frame(&stream, buf)) {
		for (y = 0; y < srcwps; y += ps) {
			process(image, buf + y);
			ewriteall(STDOUT_FILENO, image, srchps, "<stdout>");
		}
	}

	free(buf);
	free(image);
	return 0;
}
