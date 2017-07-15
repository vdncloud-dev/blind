/* See LICENSE file for copyright and license details. */
#include "common.h"

USAGE("alpha-stream")

static int first = 1;

#define PROCESS(TYPE)\
	do {\
		typedef TYPE pixel_t[4];\
		pixel_t *restrict clr = (pixel_t *)cbuf;\
		pixel_t *restrict alf = (pixel_t *)abuf;\
		pixel_t *img = (pixel_t *)output;\
		size_t i, n = colour->frame_size / sizeof(pixel_t);\
		TYPE a1, a2;\
		\
		if (first) {\
			memcpy(output, cbuf, colour->frame_size);\
			first = 0;\
			return;\
		}\
		\
		for (i = 0; i < n; i++, clr++, alf++, img++) {\
			a1 = (*img)[3];\
			a2 = (*clr)[3] * (*alf)[1] * (*alf)[3];\
			a1 *= (1 - a2);\
			(*img)[0] = (*img)[0] * a1 + (*clr)[0] * a2;\
			(*img)[1] = (*img)[1] * a1 + (*clr)[1] * a2;\
			(*img)[2] = (*img)[2] * a1 + (*clr)[2] * a2;\
			(*img)[3] = a1 + a2;\
		}\
		\
		(void) colour;\
		(void) alpha;\
	} while (0)

static void
process_lf(char *output, char *restrict cbuf, char *restrict abuf,
	   struct stream *colour, struct stream *alpha)
{
	PROCESS(double);
}

static void
process_f(char *output, char *restrict cbuf, char *restrict abuf,
	  struct stream *colour, struct stream *alpha)
{
	PROCESS(float);
}

int
main(int argc, char *argv[])
{
	struct stream colour, alpha;
	void (*process)(char *restrict output, char *restrict cbuf, char *restrict abuf,
			struct stream *colour, struct stream *alpha);

	ARGBEGIN {
	default:
		usage();
	} ARGEND;

	if (argc != 1)
		usage();

	eopen_stream(&colour, NULL);
	eopen_stream(&alpha, argv[0]);

	if (colour.encoding == DOUBLE)
		process = process_lf;
	else if (colour.encoding == FLOAT)
		process = process_f;
	else
		eprintf("pixel format %s is not supported, try xyza\n", colour.pixfmt);

	echeck_compat(&colour, &alpha);
	fprint_stream_head(stdout, &colour);
	efflush(stdout, "<stdout>");
	process_each_frame_two_streams(&colour, &alpha, STDOUT_FILENO, "<stdout>", process);
	return 0;
}
