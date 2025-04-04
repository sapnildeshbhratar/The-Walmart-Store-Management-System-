
#include "map.h"

EntrySet *hashArray[HASH_TABLE_SIZE];

inline C_MAP_PUBLIC(int)
_HashFunction_(const char* _valueString) {
    int defined = 0;
    int shifting, multiplier = 1;
    for (int iterator = 0; _valueString[iterator] != '\0'; ++iterator) {
        defined += _valueString[iterator] * multiplier;
        shifting = multiplier << 5;
        multiplier = shifting - multiplier;
    }
    return (((defined * RANDOM_A) + RANDOM_B) % CONSTRAINED_CONSTANT) % HASH_TABLE_SIZE;
}

inline C_MAP_PUBLIC(int)
_HashIndex_(int defined) {
    return (((defined * RANDOM_A) + RANDOM_B) % CONSTRAINED_CONSTANT) % HASH_TABLE_SIZE;
}


C_MAP_PUBLIC(void)
map_use_hashmap() {
    for (int iterator = 0; iterator < HASH_TABLE_SIZE; ++iterator) {
        hashArray[iterator] = NULL;
    }
}


C_MAP_PUBLIC(void)
hashmap_add_to_map(EntrySet value) {
    int idx = _HashIndex_(value.id);
    if(hashArray[idx]==NULL) {
        hashArray[idx] = malloc(sizeof(EntrySet));
    }
    hashArray[idx]->count = value.count;
    hashArray[idx]->id = value.id;
}

C_MAP_PUBLIC(EntrySet)
hashmap_get(int key) {
    int idx = _HashIndex_(key);
    if (hashArray[idx]->id != 0) {
        return *(hashArray[idx]);
    }
    else {
        EntrySet defaultEntrySet = {0, 0};
        return defaultEntrySet;
    }
}

C_MAP_PUBLIC(bool)
hashmap_contains_key(int key) {
    int idx = _HashIndex_(key);
    return hashArray[idx];
}

C_MAP_PUBLIC(void)
hashmap_display() {
    printf("{ ");
    for (int iterator = 0; iterator < HASH_TABLE_SIZE; ++iterator) {
        if (hashArray[iterator]) {
            printf("\n   Key : %d, Value : %d",hashArray[iterator]->id, hashArray[iterator]->count);
        }
    }
    printf("\n}\n");
}

C_MAP_PUBLIC(EntrySet *)
hashmap_pull_ordered_set() {
    EntrySet *set = malloc(sizeof(EntrySet)*HASH_TABLE_SIZE);
    int curr_idx = 0;
    for (int itr = 0; itr < HASH_TABLE_SIZE; ++itr) {
        EntrySet *curr_max_set = NULL;
        int max_sum = INT_MIN;
        int max_idx = -1;
        for (int jtr = 0; jtr < HASH_TABLE_SIZE; ++jtr) {
            if (hashArray[jtr] && hashArray[jtr]->count > max_sum) {
                max_sum = hashArray[jtr]->count;
                curr_max_set = hashArray[jtr];
                max_idx = jtr;
            }
        }
        if (curr_max_set) {
            set[curr_idx++] = *curr_max_set;
            hashArray[max_idx] = NULL;
            free(curr_max_set);
        }
    }
    return set;
}



TreeNode *_set_balance_tree_(TreeNode *treeNode);
TreeNode *_rotate_tree_right_(TreeNode *pNode);
TreeNode *_rotate_tree_left_(TreeNode *pNode);
bool _check_if_contains_(TreeNode *pNode, T value);

int _treeGetHeight(TreeNode* root) {
    if (root) {
        int leftHeight = _treeGetHeight(root->left);
        int rightHeight = _treeGetHeight(root->right);
        return 1 + ((leftHeight > rightHeight)? leftHeight : rightHeight) ;
    }
    return 0;
}
C_MAP_PUBLIC(TreeMap*)
map_use_treemap() {
    TreeMap *set = malloc(sizeof(TreeMap));
    set->root = NULL;
    return set;
}


TreeNode* _addElementRecursively(TreeNode* root, T value) {
    if (root==NULL) {
        root = malloc(sizeof(TreeNode));
        root->left = NULL;
        root->right = NULL;
        root->dataValue = value;
    }
    else {
        if (root->dataValue->id == value->id) {
            root->dataValue->count += value->count;
        }
        else if (root->dataValue->count < value->count) {
            root->right = _addElementRecursively(root->right, value);
        }
        else if(root->dataValue->count > value->count) {
            root->left = _addElementRecursively(root->left, value);
        }
    }

    return _set_balance_tree_(root);
}


TreeNode *_set_balance_tree_(TreeNode *treeNode) {
    int left = _treeGetHeight(treeNode->left);
    int right = _treeGetHeight(treeNode->right);

    if ((left - right) > 1) {
        if (_treeGetHeight(treeNode->left->left) - _treeGetHeight(treeNode->left->right) > 0) {
            return _rotate_tree_right_(treeNode);
        }
        else if (_treeGetHeight(treeNode->left->left) - _treeGetHeight(treeNode->left->right) < 0) {
            treeNode->left = _rotate_tree_left_(treeNode->left);
            return _rotate_tree_right_(treeNode);
        }
    }
    else if ((right - left) > 1) {
        if (_treeGetHeight(treeNode->right->left) - _treeGetHeight(treeNode->right->right) < 0) {
            return _rotate_tree_left_(treeNode);
        }
        else if (_treeGetHeight(treeNode->right->left) - _treeGetHeight(treeNode->right->right) > 0) {
            treeNode->right = _rotate_tree_right_(treeNode->right);
            return _rotate_tree_left_(treeNode);
        }
    }
    return treeNode;
}

TreeNode *_rotate_tree_left_(TreeNode *pNode) {
    TreeNode *newRoot = pNode->right;
    pNode->right = newRoot->left;
    newRoot->left = pNode;
    return newRoot;
}

TreeNode *_rotate_tree_right_(TreeNode *pNode) {
    TreeNode *newRoot = pNode->left;
    pNode->left = newRoot->right;
    newRoot->right = pNode;
    return newRoot;
}

C_MAP_PUBLIC(void)
treemap_add_entry(TreeMap* set, T value) {
    set->root = _addElementRecursively(set->root, value);
}

bool _check_if_contains_(TreeNode *pNode, T value) {
    if (pNode) {
        printf("%d\n",pNode->dataValue->count);
        if (pNode->dataValue->id == value->id) { return true; }
        else if (pNode->dataValue->id > value->id) { return _check_if_contains_(pNode->left, value); }
        else { return _check_if_contains_(pNode->right, value); }
    }
    return false;
}

C_MAP_PUBLIC(bool)
treemap_contains_object(TreeMap* set, T value) {
    return _check_if_contains_(set->root, value);
}

static int counter = 0;
void _inorderTraversal(TreeNode *root, T* buf) {
    if(root) {
        _inorderTraversal(root->left, buf);
        buf[counter++] = root->dataValue;
        _inorderTraversal(root->right, buf);
    }
}

C_MAP_PUBLIC(T*)
treemap_get_ordered_set(TreeMap* set, int *returnSize) {
    counter = 0;
    T* retVal = malloc(sizeof(T) * 1000);
    if (retVal) {
        _inorderTraversal(set->root, retVal);
    }
    *returnSize = counter;
    return retVal;
}