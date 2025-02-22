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

#include <stdio.h>
#include <signal.h>
#include <exceptions4c.h>

#define main main_initialize_exception
//! [initialize_exception]
const struct e4c_exception_type PET_ERROR = {NULL, "Pet error"};

static void set_custom_data(struct e4c_exception * exception) {
  exception->data = "My custom data";
}

int main(void) {
  e4c_get_context()->initialize_exception = set_custom_data;
  TRY {
    THROW(PET_ERROR, "Bad dog");
  } CATCH_ALL {
    const char * data = e4c_get_exception()->data;
    printf("Custom data: %s\n", data);
  }
  return EXIT_SUCCESS;
}
//! [initialize_exception]
#undef main

#define main main_finalize_exception
//! [finalize_exception]
struct my_custom_data { int id; const char * msg; };

static void my_initializer(struct e4c_exception * exception) {
  struct my_custom_data data = {123, "Hello world!"}, * tmp;
  if ((tmp = exception->data = malloc(sizeof(data)))) *tmp = data;
}

static void my_finalizer(const struct e4c_exception * exception) {
  free(exception->data);
}

int main(void) {
  struct e4c_context * context = e4c_get_context();
  context->initialize_exception = my_initializer;
  context->finalize_exception = my_finalizer;
  TRY {
    THROW(PET_ERROR, "Bad dog");
  } CATCH_ALL {
    const struct my_custom_data * data = e4c_get_exception()->data;
    printf("ID: %d MSG: %s\n", data->id, data->msg);
  }
  return EXIT_SUCCESS;
}
//! [finalize_exception]
#undef main

#define main main_termination_handler
//! [termination_handler]
static void my_termination_handler(void) {
  exit(EXIT_SUCCESS);
}

int main(void) {
  e4c_get_context()->termination_handler = &my_termination_handler;
  THROW(PET_ERROR, "Bad dog");
}
//! [termination_handler]
#undef main

#define main main_set_context_supplier
//! [set_context_supplier]
static struct e4c_context my_custom_context = {
  .initialize_exception = my_initializer,
  .finalize_exception = my_finalizer
};

static struct e4c_context * my_context_supplier(void) {
  return &my_custom_context;
}

int main(void) {
  e4c_set_context_supplier(my_context_supplier);
  TRY {
    THROW(PET_ERROR, "Bad dog");
  } CATCH_ALL {
    const struct my_custom_data * data = e4c_get_exception()->data;
    printf("MSG: %s\n", data->msg);
  }
  return EXIT_SUCCESS;
}
//! [set_context_supplier]
#undef main

int main(int argc, char * argv[]) {

//! [get_context]
struct e4c_context * context = e4c_get_context();
//! [get_context]

  printf("current context: %p\n", (void *) context);

  main_initialize_exception();
  main_finalize_exception();
  main_set_context_supplier();
  main_termination_handler();

  return EXIT_SUCCESS;
}
