
#include "testing.h"


void nest_try_block(int keep_nesting);


/**
 * Exceed maximum number of exception frames
 */
TEST_CASE{

    nest_try_block(E4C_MAX_FRAMES + 1 /* will overflow */);
}

void nest_try_block(int keep_nesting){

    if(keep_nesting){

        E4C_TRY{

            nest_try_block(--keep_nesting);
        }
    }
}
