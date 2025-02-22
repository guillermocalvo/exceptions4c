/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <pthread.h>
#include <exceptions4c.h>

static void cancel_current_thread() {
    pthread_exit(PTHREAD_CANCELED);
}

struct e4c_context dummy = {
    ._innermost_block = NULL,
    .uncaught_handler = NULL,
    .termination_handler = cancel_current_thread,
    .initialize_exception = NULL,
    .finalize_exception = NULL
};

struct e4c_context *e4c_pthreads_context_supplier() {
    return &dummy;
}

//! [setup]
const struct e4c_exception_type OOPS = {NULL, "Oops"};

/* A Thread that throws an exception */
static void *my_thread(void *arg) {
    THROW(OOPS, "Oh no");
}

int main(void) {
    /* Set the thread-safe exception context supplier */
    e4c_set_context_supplier(&e4c_pthreads_context_supplier);

    /* Start the thread */
    pthread_t thread;
    pthread_create(&thread, NULL, my_thread, NULL);
    pthread_join(thread, NULL);

    /* The program was not terminated, only the thread was canceled */
    return EXIT_SUCCESS;
}
//! [setup]
