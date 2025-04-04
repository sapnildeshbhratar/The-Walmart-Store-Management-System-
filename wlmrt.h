

#ifndef DSPD2_ASSIGNMENT_WALMART_H
#define DSPD2_ASSIGNMENT_WALMART_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strbuild.h"
#include <limits.h>

#define __ORDER__ 5
#define __THRESHOLD__   6
#define __MIN_ORDER   __ORDER__ / 2

#define ID_FORMAT   10000
#define MAX_AISLES    100

typedef struct _item {
    int item_id;
    char item_name[100];
    int quantity;
    char expire_date[100];
    int threshold_quantity;
    struct _item *next;
}Item_node;

typedef struct _bplus_ {
    size_t count;
    union _Data {
        int idx[__THRESHOLD__ - 1];
        Item_node *val[__ORDER__ - 1];
    }data;
    bool is_root : 1;
    bool is_leaf : 1;
    union _Pointer_T {
        struct _bplus_ *child[__THRESHOLD__];
        struct _local {
            struct _bplus_ *next;
            struct _bplus_ *prev;
        }local;

    }ptr;
}bPLUS;

void wlmrt_load_database(const char* fBuf);
Item_node* wlmrt_parse_item(int id, char* name, int quantity, char* date, int threshold);
Item_node* wlmrt_get_item_by_id(int id);
void wlmrt_free_item(Item_node* item);
void wlmrt_add_new_item(Item_node* item);
void wlmrt_refresh_product_list();
void wlmrt_delete_item_by_id(int id);
void wlmrt_display_all_items();
void wlmrt_update_item_by_id(int id);
void wlmrt_init_aisles_table();
void wlmrt_delete_aisle(int aisle_no);
bool wlmrt_check_availability(int id, int quantity, char* expiry);
void wlmrt_display_threshold_alert();
void wlmrt_display_item(Item_node *pItem);
void wlmrt_display_read_to_eat_expiry();
void wlmrt_display_dairy_expiry_oneDay();
void wlmrt_merge_aisles(int aisles1, int aisles2);
void wlmrt_add_aisles(int size);

typedef struct __attribute__((packed)) _item_list  {
    int item_id;
    int count;
    int size;
}Cart;

typedef struct _consumer{
    bPLUS * front;
    char name[100];
    struct _consumer *next;
}Customer;

void wlmrt_add_customer(const char* name, Item_node * cart);
Item_node * wlmrt_purchase_new_item(Cart* items, int no_of_items);
void wlmrt_item_more_often_bought(int item_id,...);
void wlmrt_generate_invoice(const char *name, Item_node *cart);
void wlmrt_write_products_list();
void wlmrt_display_range_of_items(int item_id_1, int item_id_2);

#endif //DSPD2_ASSIGNMENT_WALMART_H

