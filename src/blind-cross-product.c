/* See LICENSE file for copyright and license details. */
#include "common.h"

USAGE("right-hand-stream")

#define PROCESS(TYPE, SUFFIX)\
	static void\
	process_##SUFFIX(struct stream *left, struct stream *right, size_t n)\
	{\
		size_t i;\
		TYPE *lx, *ly, *lz, *la, *rx, *ry, *rz, *ra, x, y, z, a;\
		for (i = 0; i < n; i += 4 * sizeof(TYPE)) {\
			lx = ((TYPE *)(left->buf + i)) + 0, rx = ((TYPE *)(right->buf + i)) + 0;\
			ly = ((TYPE *)(left->buf + i)) + 1, ry = ((TYPE *)(right->buf + i)) + 1;\
			lz = ((TYPE *)(left->buf + i)) + 2, rz = ((TYPE *)(right->buf + i)) + 2;\
			la = ((TYPE *)(left->buf + i)) + 3, ra = ((TYPE *)(right->buf + i)) + 3;\
			x = *ly * *rz - *lz * *ry;\
			y = *lz * *rx - *lx * *rz;\
			z = *lx * *ry - *ly * *rx;\
			a = *la * *ra;\
			*lx = x;\
			*ly = y;\
			*lz = z;\
			*la = a;\
		}\
	}

PROCESS(double, lf)
PROCESS(float, f)

int
main(int argc, char *argv[])
{
	struct stream left, right;
	void (*process)(struct stream *left, struct stream *right, size_t n);

	UNOFLAGS(argc != 1);

	eopen_stream(&left, NULL);
	eopen_stream(&right, argv[0]);

	if (left.encoding == DOUBLE)
		process = process_lf;
	else if (left.encoding == FLOAT)
		process = process_f;
	else
		eprintf("pixel format %s is not supported, try xyza\n", left.pixfmt);

	fprint_stream_head(stdout, &left);
	efflush(stdout, "<stdout>");
	process_two_streams(&left, &right, STDOUT_FILENO, "<stdout>", process);
	return 0;
}
