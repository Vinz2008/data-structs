#include <stdlib.h>
#include <stdio.h>

#define VECTOR_DEF(element_type) \
    /* TODO : 64 bits capacity and len ? (do this to reduce the size if there are function pointers in it)*/ \
    /* use functions pointers or global funcs ? */ \
    typedef struct { \
        element_type* data; \
        int capacity; \
        int len; \
    } Vec_##element_type; \
            \
    Vec_##element_type vector_##element_type##_new(int capacity){ \
        return (Vec_##element_type){ \
            .capacity = capacity, \
            .len = 0, \
            .data = malloc(sizeof(element_type) * capacity) \
        }; \
    } \
      \
    void vector_##element_type##_push(Vec_##element_type* v, element_type e){ \
        if (v->len + 1 >= v->capacity){ \
            int new_capacity = v->capacity + v->capacity/2; /* v->capacity * 1.5*/ \
            if (new_capacity == 0){ /*for the case if capacity == 0*/ \
                new_capacity = 2; \
            } \
            element_type* new_mem = realloc(v->data, new_capacity * sizeof(element_type)); \
            if (!new_mem){ \
                /* failed realloc */ \
                printf("WARNING : realloc failed\n"); \
                free(v->data); /* when realloc fails, old mem is still valid, this is needed to prevent a memory leaks*/ \
                return; \
            } \
            v->data = new_mem; \
            v->capacity = new_capacity; \
        } \
          \
        v->data[v->len] = e; \
        v->len++; \
    } \
      \
    void vector_##element_type##_destroy(Vec_##element_type v){ \
        free(v.data); \
    }
