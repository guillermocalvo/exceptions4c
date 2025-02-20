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
#include <stdio.h>
#include <exceptions4c.h>

typedef enum pet_status { ERROR, UNKNOWN, AVAILABLE, PENDING, SOLD } pet_status;

typedef const struct pet *Pet;

struct pet {
  int id;
  const char *name;
  pet_status status;
};

//! [exception_types]
/* Generic errors */
const struct e4c_exception_type NOT_ENOUGH_MEMORY = {NULL, "Not enough memory"};
const struct e4c_exception_type TIMED_OUT = {NULL, "Timed out"};

/* Base exception for all pet-related errors */
const struct e4c_exception_type PET_ERROR = {NULL, "Pet error"};

/* Specific types of pet errors */
const struct e4c_exception_type PET_NOT_FOUND = {&PET_ERROR, "Pet not found"};
const struct e4c_exception_type PET_STORE_CLOSED = {&PET_ERROR, "Pet store closed"};
//! [exception_types]

// Available pets in the store
static struct pet pets[] = {
  {.id = 0, .name = "Rocky", .status = AVAILABLE},
  {.id = 1, .name = "Garfield", .status = PENDING},
  {.id = 2, .name = "Rantanplan", .status = SOLD}
};

static void pet_free(const void *pet) {
  (void) pet;
}

static Pet pet_clone(const int id) {
  for (int index = 0; index < sizeof(pets) / sizeof(pets[0]); index++) {
    if (pets[index].id == id) {
      return &pets[index];
    }
  }
  return NULL;
}

static bool pet_store_is_closed() {
  return false;
}

//! [throw]
/* Returns a pet by id */
Pet pet_find(int id) {
  Pet pet = pet_clone(id);
  if (!pet) {
    THROW(PET_NOT_FOUND, "Pet %d not found", id);
  }
  return pet;
}
//! [throw]

/* Returns a pet by id */
Pet pet_find_retry(int id) {
  static int last_id = -100;
  static int attempts = 0;
  if (last_id != id) {
    last_id = id;
    THROW(TIMED_OUT, "First attempt to find %d", id);
  }
  attempts++;
  Pet pet = pet_clone(id);
  if (!pet) {
    THROW(PET_NOT_FOUND, "Pet %d not found", id);
  }
  return pet;
}

#define get_pet_status get_pet_status_try
//! [try]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  }
  return status;
}
//! [try]
#undef get_pet_status

#define get_pet_status get_pet_status_catch
//! [catch]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH (PET_ERROR) {
    status = UNKNOWN;
  } CATCH (NOT_ENOUGH_MEMORY) {
    abort();
  }
  return status;
}
//! [catch]
#undef get_pet_status

#define get_pet_status get_pet_status_catch_all
//! [catch_all]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH_ALL {
    status = UNKNOWN;
  }
  return status;
}
//! [catch_all]
#undef get_pet_status

#define get_pet_status get_pet_status_catch_all_get_exception
//! [catch_all_get_exception]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH_ALL {
    if (e4c_get_exception()->type == &NOT_ENOUGH_MEMORY) {
      abort();
    }
    status = UNKNOWN;
  }
  return status;
}
//! [catch_all_get_exception]
#undef get_pet_status

#define get_pet_status get_pet_status_retry
#define pet_find pet_find_retry
//! [retry]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH (TIMED_OUT) {
    RETRY(3, PET_NOT_FOUND, "Timed out too many times");
  }
  return status;
}
//! [retry]
#undef pet_find
#undef get_pet_status

#define get_pet_status get_pet_status_finally
//! [finally]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  TRY {
    pet = pet_find(id);
    status = pet->status;
  } CATCH (PET_NOT_FOUND) {
    status = UNKNOWN;
  } FINALLY {
    pet_free(pet);
  }
  return status;
}
//! [finally]
#undef get_pet_status

#define get_pet_status get_pet_status_using
//! [using]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  USING (pet = pet_find(id), pet != NULL, pet_free(pet)) {
    status = pet->status;
  }
  return status;
}
//! [using]
#undef get_pet_status

#define get_pet_status get_pet_status_using_catch
//! [using_catch]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  USING (pet = pet_find(id), pet != NULL, pet_free(pet)) {
    status = pet->status;
  } CATCH (PET_NOT_FOUND) {
    status = UNKNOWN;
  }
  return status;
}
//! [using_catch]
#undef get_pet_status

#define get_pet_status get_pet_status_with_use
//! [with_use]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  WITH (pet_free(pet)) {
    if (pet_store_is_closed()) {
      THROW(PET_STORE_CLOSED, NULL);
    }
    pet = pet_find(id);
  } USE (pet != NULL) {
    status = pet->status;
  } CATCH (PET_NOT_FOUND) {
    status = UNKNOWN;
  }
  return status;
}
//! [with_use]
#undef get_pet_status

#define get_pet_status get_pet_status_reacquire
#define pet_find pet_find_retry
//! [reacquire]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  USING (pet = pet_find(id), pet != NULL, pet_free(pet)) {
    status = pet->status;
  } CATCH (TIMED_OUT) {
    REACQUIRE(3, PET_NOT_FOUND, "Timed out too many times");
  }
  return status;
}
//! [reacquire]
#undef pet_find
#undef get_pet_status

void process_data(int argc, char * argv[]) { /* NOSONAR */
  printf("Processing data\n");
}

#define main main_is_uncaught
//! [is_uncaught]
int main(int argc, char * argv[]) {
  TRY {
    process_data(argc, argv);
  } FINALLY {
    if (e4c_is_uncaught()) {
      fprintf(stderr, "Fatal error while processing data.\n");
    } else {
      fprintf(stdout, "Data processed successfully.\n");
    }
  }
  return EXIT_SUCCESS;
}
//! [is_uncaught]
#undef main

# define FAIL \
  ((void) fprintf(stderr, "Error: %s:%d\n", __FILE__, __LINE__), abort())

int main(void) { /* NOSONAR */
  if (get_pet_status_try(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_try(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_try(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_catch(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_catch(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_catch(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_catch(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_catch_all(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_catch_all(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_catch_all(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_catch_all(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_catch_all_get_exception(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_catch_all_get_exception(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_catch_all_get_exception(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_catch_all_get_exception(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_finally(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_finally(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_finally(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_finally(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_retry(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_retry(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_retry(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_using_catch(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_using_catch(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_using_catch(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_using_catch(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_with_use(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_with_use(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_with_use(2) != SOLD) {
    FAIL;
  }

  if (get_pet_status_with_use(-1) != UNKNOWN) {
    FAIL;
  }

  if (get_pet_status_reacquire(0) != AVAILABLE) {
    FAIL;
  }

  if (get_pet_status_reacquire(1) != PENDING) {
    FAIL;
  }

  if (get_pet_status_reacquire(2) != SOLD) {
    FAIL;
  }

  return EXIT_SUCCESS;
}
