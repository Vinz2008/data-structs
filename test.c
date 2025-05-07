#include "data-structs.h"

VECTOR_DEF(int)

HASHTABLE_DEF(char)

int main(){
    Vec_int v = vector_int_new(0);
    for (int i = 0; i < 100; i++){
        vector_int_push(&v, i * 2);
    }

    for (int i = 0; i < 100; i++){
        printf("v[%d] : %d\n", i, v.data[i]);
    }

    vector_int_destroy(v);

    printf("\n");

    hashtable_char_t h = hashtable_char_create(10);

    const char* keys[] = {"test_a", "test_b", "test_c", "test_d", "test_e", "test_f", "test_g", "test_h", "test_i", "test_j"};

    for (char i = 'a'; i < 'a' + 10; i++){
        hashtable_char_set(&h, keys[i - 'a'], i);
    }

    for (char i = 0; i < 10; i++){
        char* h_get = hashtable_char_get(h, keys[i]);
        printf("h[%s] = %c\n", keys[i], *h_get);
    }

    hashtable_char_destroy(h);
    return 0;
}