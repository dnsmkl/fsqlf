// Test case/usecase/demo/tryout relevant for use in fsqlf
///
// Code taken (a bit modified) from
// http://troydhanson.github.io/uthash/userguide.html#_string_keys



#include <string.h>  // strcpy
#include <stdlib.h>  // malloc
#include <stdio.h>   // printf
#include "uthash.h"


struct User
{
    const char *name;   // key
    size_t name_len;
    int id;
    UT_hash_handle hh;  // makes this structure hashable
};


int main(int argc, char *argv[])
{
    // Head of hashmap that will map user-name to user-struct.
    struct User *map_users = NULL;


    // Fill the hashmap with values.
    const char *names[] = { "joe", "bobbby", "betty", NULL };
    const char **name_ptr;
    int i=0;
    for (name_ptr = names; *name_ptr != NULL; name_ptr++) {
        // .. create new user struct.
        struct User *newu;
        newu = (struct User*) malloc(sizeof(struct User));
        newu->name = *name_ptr;
        newu->id = i++;
        newu->name_len = strlen(newu->name);
        // .. add new user to the map.
        HASH_ADD_KEYPTR(hh, map_users, newu->name, newu->name_len, newu);
    }


    // Retrieve a value from map.
    struct User *match;
    HASH_FIND_STR(map_users, "betty", match);
    if (match) {
        printf("%s's id is %d\n", match->name, match->id);
        printf("%s's username length is %d\n", match->name, match->name_len);
    }


    // Iterate and remove.
    // alternative would be just: HASH_CLEAR(hh, users);
    struct User *user_iter;
    // seems `tmp` is needed by uthash internals
    // to provide deletion-safe iteration.
    struct User *tmp;
    HASH_ITER(hh, map_users, user_iter, tmp) {
        HASH_DEL(map_users, user_iter);
        free(user_iter);
    }
    return 0;
}
