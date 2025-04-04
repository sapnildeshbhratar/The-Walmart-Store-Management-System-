

#ifndef DSPD_2_ASSIGNMENT_MAP_H
#define DSPD_2_ASSIGNMENT_MAP_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define HASH_TABLE_SIZE         1000
#define CONSTRAINED_CONSTANT    104729
#define RANDOM_A                24587
#define RANDOM_B                86931
#define C_MAP_PUBLIC(type)   __declspec(dllexport) __attribute__((unused)) type __stdcall


typedef struct _set {
    int id ;
    int count ;
}EntrySet;

#ifndef __hash_map
#define __hash_map
    C_MAP_PUBLIC(void) map_use_hashmap();
    C_MAP_PUBLIC(void) hashmap_add_to_map(EntrySet value);
    C_MAP_PUBLIC(EntrySet) hashmap_get(int key);
    C_MAP_PUBLIC(bool) hashmap_contains_key(int key);
    C_MAP_PUBLIC(void) hashmap_display();
    C_MAP_PUBLIC(EntrySet *) hashmap_pull_ordered_set();
#endif


typedef EntrySet* T;

typedef struct treeNode {
    T dataValue;
    struct treeNode *left;
    struct treeNode *right;
}TreeNode;

typedef struct treeSet {
    TreeNode *root;
}TreeMap;

#ifndef __tree_map
#define __tree_map
    C_MAP_PUBLIC(TreeMap*) map_use_treemap();
    C_MAP_PUBLIC(bool) treemap_contains_object(TreeMap* set, T value);
    C_MAP_PUBLIC(T*) treemap_get_ordered_set(TreeMap* set, int *returnSize);
    C_MAP_PUBLIC(void) treemap_add_entry(TreeMap* set, T value);

#endif

#endif //DSPD_2_ASSIGNMENT_MAP_H
