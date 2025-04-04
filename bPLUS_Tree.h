

#ifndef BPLUSTREE_H
#define BPLUSTREE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "wlmrt.h"
#define  bPLUS_STDCALL __stdcall
#ifdef _WIN64
#define C_STANDARD_PUBLIC_INTERFACE(type)   __declspec(dllexport) type  bPLUS_STDCALL __attribute__((unused))
#elif __GNUC__
#define C_STANDARD_PUBLIC_INTERFACE(type)   __attribute__((visibility("default"))) type bPLUS_STDCALL __attribute__((unused))
#endif

#define null NULL
#define bPLUS_DEPRECATED    __attribute__((__deprecated__))



C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_CreateNodeLeaf() {
    bPLUS *node = malloc(sizeof(bPLUS));
    node->is_leaf = true;
    for (int i = 0; i < __ORDER__ - 1; ++i) {
        node->data.val[i] = null;
    }
    node->ptr.local.next = null;
    node->ptr.local.prev = null;
    node->count = 0;
    return node;
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_CreateNodeInternal(int DEFAULT_VALUE) {
    bPLUS *node = malloc(sizeof(bPLUS));
    node->is_leaf = false;
    for (int i = 0; i < __ORDER__; ++i) {
        node->ptr.child[i] = null;
    }
    for (int i = 0; i < __ORDER__ - 1; ++i) {
        node->data.idx[i] = DEFAULT_VALUE;
    }
    node->count = 0;
    return node;
}

C_STANDARD_PUBLIC_INTERFACE(int)
bPLUS_InsertNodeSorted(bPLUS *root, Item_node *item) {
    int shift = root->count;
    while (shift  > 0 && root->data.val[shift - 1]->item_id > item->item_id) {
        root->data.val[shift] = root->data.val[shift- 1];
        --shift;
    }
    root->data.val[shift] = item;
    root->count++;
    return shift + 1;
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS *)
bPLUS_InsertNode(bPLUS *root, Item_node *item, int *value) {
    if (!root->is_leaf) {
        int i = 0;
        while ( i < root->count && root->data.idx[i] <= item->item_id) { ++i; }
        bPLUS *retVal = bPLUS_InsertNode(root->ptr.child[i], item, value);
        if (retVal == null) { return null; }
        else {
            if (root->count < __ORDER__ - 1) {
                root->data.idx[root->count++] = *value;
                root->ptr.child[root->count] =  retVal;
                return null;
            }
            else {
                bPLUS *newNode = bPLUS_CreateNodeInternal(INT_MAX);
                int midIdx = __ORDER__ / 2 ;
                int shift = root->count;
                while (shift > i) {
                    root->data.idx[shift] = root->data.idx[shift-1];
                    root->ptr.child[shift] = root->ptr.child[shift - 1];
                    --shift;
                }
                root->data.idx[i] = *value;
                root->ptr.child[i+1] = retVal;
                int move = midIdx + 1;
                while (move < __ORDER__) {
                    newNode->data.idx[newNode->count] = root->data.idx[move];
                    newNode->ptr.child[newNode->count++] = root->ptr.child[move];
                    root->data.idx[move] = INT_MAX;
                    root->ptr.child[move] = null;
                    ++move;
                }
                newNode->ptr.child[newNode->count] = root->ptr.child[move];
                root->count = midIdx;
                *value = root->data.idx[midIdx];
                root->data.idx[midIdx] = INT_MAX;
                return newNode;
            }
        }
    }
    else {
        if (root->count < __ORDER__ - 1) {
            bPLUS_InsertNodeSorted(root, item);
            return null;
        }
        else {
            bPLUS *newNode = bPLUS_CreateNodeLeaf();
            int midIdx = __ORDER__ / 2;
            for (int i = midIdx ; i < __ORDER__ - 1; ++i) {
                newNode->data.val[newNode->count++] = root->data.val[i];
                root->data.val[i] = null;
            }
            root->count = midIdx;
            *value = newNode->data.val[0]->item_id;
            if (item->item_id < *value) {
                bPLUS_InsertNodeSorted(root, item);
            }
            else {
                bPLUS_InsertNodeSorted(newNode, item);
            }
            root->ptr.local.next = newNode;
            newNode->ptr.local.prev = root;
            return newNode;
        }
    }
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_Offer(bPLUS *root, Item_node *item) {
    if (root == null) {
        root = bPLUS_CreateNodeInternal(INT_MAX);
        root->ptr.child[0] = bPLUS_CreateNodeLeaf();
    }
    int endptr;
    bPLUS *temp = bPLUS_InsertNode(root, item, &endptr);
    if (temp) {
        bPLUS *node = bPLUS_CreateNodeInternal(INT_MAX);
        node->ptr.child[0] = root;
        node->data.idx[0] = endptr;
        node->ptr.child[1] = temp;
        node->count++;
        root = node;
    }

    return root;
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_Search(bPLUS* root, Item_node *item) {
    if (!root->is_leaf) {

        int i = 0;
        while ( i < root->count && root->data.idx[i] <= item->item_id) { ++i; }
        return bPLUS_Search(root->ptr.child[i], item);
    }
    else {
        for (int i = 0; i < root->count; ++i) {
            if (root->data.val[i]->item_id == item->item_id) { return root; }
        }
        return null;
    }
}

C_STANDARD_PUBLIC_INTERFACE(void)
bPLUS_PrintFrom(bPLUS *root) {
    while (root) {
        for (int i = 0; i < root->count; ++i) {
            if (root->data.val[i]) {
                printf("%d ",root->data.val[i]->item_id);
            }
        }
        printf(" | ");
        root = root->ptr.local.next;
    }
    printf("\n");
}

C_STANDARD_PUBLIC_INTERFACE(Item_node*)
bPLUS_SeekFirst(bPLUS *root) {
    if (root == null) { return null; }
    if (!root->is_leaf) {
        return bPLUS_SeekFirst(root->ptr.child[0]);
    }
    return root->data.val[0];
}

C_STANDARD_PUBLIC_INTERFACE(int)
bPLUS_ParseFirst(Item_node *item) {
    if (item == null) { return INT_MAX; }
    return item->item_id;
}

C_STANDARD_PUBLIC_INTERFACE(Item_node*)
bPLUS_ParseObject(bPLUS *root, Item_node* itemNode) {
    if (root && root->is_leaf) {
        for (int i = 0; i < root->count; ++i) {
            if (root->data.val[i]->item_id == itemNode->item_id) { return root->data.val[i]; }
        }
    }
    return null;
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_GetFirst(bPLUS *root) {
    if (root==null) { return null; }
    if (!root->is_leaf) {
        return bPLUS_GetFirst(root->ptr.child[0]);
    }
    return root;
}


C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_Delete(bPLUS* root, Item_node *item) {
    if (!root->is_leaf) {
        // traverse
        int i =0 ;
        while (i < root->count && root->data.idx[i] <= item->item_id) { ++i; }
        bPLUS *retVal = bPLUS_Delete(root->ptr.child[i], item);
        if (retVal == null) {
            if (i>0) {
                root->data.idx[i-1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
            }
            return null;
        }
        if (retVal->is_leaf){
            if (i < root->count) { // right sibling exits
                // try to borrow from right
                if (root->ptr.child[i + 1]->count > __MIN_ORDER - 1) {
                    bPLUS_InsertNodeSorted(retVal, bPLUS_SeekFirst(root->ptr.child[i+1]));
                    bPLUS *advance = root->ptr.child[i+1];
                    int shift = 1;
                    while (shift < advance->count) {
                        advance->data.val[shift-1] = advance->data.val[shift];
                        advance->data.val[shift] = null;
                        ++shift;
                    }
                    advance->count--;
                    root->data.idx[i] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i+1]));
                    if (i-1 >= 0) {
                        root->data.idx[i - 1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
                    }
                    return null;
                }
            }
            if (i > 0) { // left sibling exits
                // try to borrow from left sibling
                if (root->ptr.child[i-1]->count > __MIN_ORDER - 1) {
                    bPLUS *back = root->ptr.child[i-1];
                    bPLUS_InsertNodeSorted(retVal, back->data.val[back->count-1]);
                    back->data.val[back->count-1] = null;
                    back->count--;
                    root->data.idx[i-1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
                    return null;
                }
            }
            if (i < root->count) { // right sibling exits
                // merge with right
                bPLUS *advance = root->ptr.child[i+1];
                for (int j = 0; j < advance->count; ++j) {
                    retVal->data.val[retVal->count++] = advance->data.val[j];
                }
                retVal->ptr.local.next = advance->ptr.local.next;
                if (advance->ptr.local.next) {
                    advance->ptr.local.next->ptr.local.prev = retVal;
                }
                free(advance);
                int shift = i + 1;
                while (shift < root->count) {
                    root->data.idx[shift-1] = root->data.idx[shift];
                    root->ptr.child[shift] = root->ptr.child[shift+1];
                    ++shift;
                }
                root->ptr.child[shift] = null;
                root->count--;
                if (i-1>=0) {
                    root->data.idx[i-1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
                }
                if (root->count < __MIN_ORDER - 1) {
                    return root;
                }
                else {
                    return null;
                }
            }
            if ( i > 0 ) { // left sibling exits
                // merge with left
                bPLUS *back = root->ptr.child[i-1];
                for (int j = 0; j < back->count; ++j) {
                    retVal->data.val[retVal->count++] = back->data.val[j];
                }
                retVal->ptr.local.prev = back->ptr.local.prev;
                if (back->ptr.local.prev) {
                    back->ptr.local.prev->ptr.local.next = retVal;
                }
                free(back);
                int shift = i;
                while (shift < root->count) {
                    root->data.idx[shift-1] = root->data.idx[shift];
                    root->ptr.child[shift-1] = root->ptr.child[shift];
                    ++shift;
                }
                root->ptr.child[shift-1] = root->ptr.child[shift];
                root->ptr.child[shift] = null;
                root->count--;
                if (i-2 >=0) {
                    root->data.idx[i - 2] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i-1]));
                }
                if (root->count < __MIN_ORDER - 1) {
                    return root;
                }
                else {
                    return null;
                }
            }
        }
        else {
            if (i < root->count) { // right sibling exits
                // try to borrow from right
                if (root->ptr.child[i + 1]->count > __MIN_ORDER - 1) {
                    bPLUS *advance = root->ptr.child[i+1]->ptr.child[0];
                    int idx = root->ptr.child[i+1]->data.idx[0];
                    int shift = 0;
                    while (shift < root->ptr.child[i+1]->count)  {
                        root->ptr.child[i+1]->data.idx[shift] = root->ptr.child[i+1]->data.idx[shift+1];
                        root->ptr.child[i+1]->ptr.child[shift] = root->ptr.child[i+1]->ptr.child[shift+1];
                        ++shift;
                    }
                    root->ptr.child[i+1]->ptr.child[shift] = null;
                    root->ptr.child[i+1]->count--;
                    retVal->data.idx[retVal->count++] = root->data.idx[i];
                    root->data.idx[i] = idx;
                    retVal->ptr.child[retVal->count] = advance;
                    root->data.idx[i] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i+1]));
                    return null;
                }
            }
            if (i > 0) { // left sibling exits
                // try to borrow from left
                if (root->ptr.child[i-1]->count > __MIN_ORDER - 1) {
                    bPLUS *back = root->ptr.child[i-1]->ptr.child[root->ptr.child[i-1]->count];
                    root->ptr.child[i-1]->ptr.child[root->ptr.child[i-1]->count] = null;
                    int idx = root->ptr.child[i-1]->data.idx[root->ptr.child[i-1]->count - 1];
                    root->ptr.child[i-1]->data.idx[root->ptr.child[i-1]->count - 1] = INT_MAX;
                    root->ptr.child[i-1]->count--;
                    int shift = retVal->count;
                    while (shift > 0) {
                        retVal->data.idx[shift] = retVal->data.idx[shift-1];
                        retVal->ptr.child[shift+1] = retVal->ptr.child[shift];
                        --shift;
                    }
                    retVal->ptr.child[shift+1] = retVal->ptr.child[shift];
                    retVal->data.idx[shift] = root->data.idx[i-1];
                    root->data.idx[i-1] = idx;
                    retVal->ptr.child[shift] = back;
                    retVal->count++;
                    root->data.idx[i-1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
                    return null;
                }
            }
            if (i < root->count) { //right sibling exits
                // merge from the right
                bPLUS *advance = root->ptr.child[i + 1];
                retVal->data.idx[retVal->count++] = root->data.idx[i];
                int shift = 0;
                while (shift < advance->count) {
                    retVal->data.idx[retVal->count] = advance->data.idx[shift];
                    retVal->ptr.child[retVal->count++] = advance->ptr.child[shift];
                    ++shift;
                }
                retVal->ptr.child[retVal->count] = advance->ptr.child[shift];

                shift = i + 1;
                while (shift < root->count) {
                    root->data.idx[shift - 1] = root->data.idx[shift];
                    root->ptr.child[shift] = root->ptr.child[shift + 1];
                    ++shift;
                }
                root->ptr.child[shift] = null;
                root->count--;
                root->data.idx[i] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i+1]));
                if (root->count < __MIN_ORDER - 1) {
                    return root;
                }
                else {
                    return null;
                }

            }
            if (i > 0) { // left sibling exits
                // merge from the left
                bPLUS *back = root->ptr.child[i-1];
                int shift = 0;

                back->data.idx[back->count++] = root->data.idx[i-1];
                while (shift < retVal->count) {
                    back->data.idx[back->count] = retVal->data.idx[shift];
                    back->ptr.child[back->count++] = retVal->ptr.child[shift];
                    ++shift;
                }
                back->ptr.child[back->count] = retVal->ptr.child[shift];
                shift = i;
                while (shift < root->count) {
                    root->data.idx[shift-1] = root->data.idx[shift];
                    root->ptr.child[shift] = root->ptr.child[shift+1];
                }
                root->ptr.child[shift] = null;
                root->count--;
                root->data.idx[i-1] = bPLUS_ParseFirst(bPLUS_SeekFirst(root->ptr.child[i]));
                if (root->count < __MIN_ORDER - 1) {
                    return root;
                }
                else {
                    return null;
                }
            }
        }
        return null;
    }
    else {
        // found leaf node
        int i = 0;
        while (i < root->count) {
            if (root->data.val[i]->item_id == item->item_id) {
                root->data.val[i] = null;
                int shift = i + 1;
                while (shift < root->count) {
                    root->data.val[shift-1] = root->data.val[shift];
                    root->data.val[shift] = null;
                    ++shift;
                }
                root->count--;
            }
            ++i;
        }
        if (root->count < __MIN_ORDER - 1) {
            return root;
        }
        else {
            return null;
        }
    }
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_Erase(bPLUS* root, Item_node* node) {
    bPLUS *tp = bPLUS_Delete(root, node);
    if (tp) {
        if (tp->count == 0 && tp->ptr.child[0]->is_leaf == false) {
            root = tp->ptr.child[0];
        }
    }
    return root;
}

C_STANDARD_PUBLIC_INTERFACE(bPLUS*)
bPLUS_Remove(bPLUS* root, int item_id) {
    Item_node *itemNode = malloc(sizeof(itemNode));
    itemNode->item_id = item_id;
    return bPLUS_Erase(root, itemNode);
}

#endif //BPLUSTREE_H
