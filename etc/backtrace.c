/*
 * This is a good example of using e4c's custom_data field :)
 *
 * Original code contributed by Tal Liron <tal.liron@gmail.com>
 *
 * Notes:
 *
 *  - It relies on [gcc's backtrace functionality][1].
 *  - Still, the principle can be adapted to other compilers and stacktrace
 *    libraries, so I think it's a useful example.
 *  - To add function names to the backtrace, you need to link using the
 *   `-rdynamic` flag.
 *  - gcc doesn't support line numbers in the backtrace, but you can retrieve
 *    these using the external addr2line utility.
 *
 * [1]: http://www.gnu.org/software/libc/manual/html_node/Backtraces.html
 *
 * This is free software: you can redistribute it and/or modify it under the
 * terms of the **GNU Lesser General Public License** as published by the
 * *Free Software Foundation*, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * **WITHOUT ANY WARRANTY**; without even the implied warranty of
 * **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the
 * [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html)
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "e4c.h"
#include <execinfo.h>

#define BACKTRACE_MAX_SIZE 32

typedef struct struct_BackTrace {
	void *frames[BACKTRACE_MAX_SIZE];
	size_t count, skip;
} BackTrace;

void BackTrace_create(BackTrace *back_trace) {
	back_trace->count = backtrace(back_trace->frames, BACKTRACE_MAX_SIZE);
	/* we don't need to show this function and
	the internal e4c function that calls it */
	back_trace->skip = 2;
}

void *BackTrace_create_for_exception(const e4c_exception *e) {
	BackTrace *back_trace = malloc(sizeof(BackTrace));
	BackTrace_create(back_trace);
	return back_trace;
}

void BackTrace_destroy(void *back_trace) {
	free(back_trace);
}

void BackTrace_dump(BackTrace *back_trace) {
	char **strings = backtrace_symbols(back_trace->frames, back_trace->count);
	if (strings) {
		for (size_t i = back_trace->skip, c = back_trace->count; i < c; i++) {
			fprintf(stderr, "  %s\n", strings[i]);
		}
		free(strings);
	}
}

void myfunc() {
	throw(NotEnoughMemoryException, "False alarm!");
}

int main(int argc, char *argv[]) {
	int r = 0;
	e4c_using_context(E4C_FALSE) {
		e4c_context_set_handlers(NULL, NULL, BackTrace_create_for_exception, BackTrace_destroy);
		try {
			myfunc();
		}
		catch (RuntimeException) {
			const e4c_exception *e = e4c_get_exception();
			fprintf(stderr, "%s: %s", e->name, e->message);
			if (e->custom_data) {
				BackTrace_dump(e->custom_data);
			}
			r = 1;
		}
	}
	return r;
}
