#include <stdio.h>
#include "system.h"
#include "wlmrt.h"
#include "map.h"

int main(void) {

    printf("\n\n------------------------------------  WELCOME TO WALMART STORE  ------------------------------------\n\n");

    wlmrt_init_aisles_table();

    wlmrt_load_database("database.json");
    int flag = 1;
    while (flag){

        wlmrt_display_all_items();

        printf("Enter 1 to Add / Update an Item.\n");
        printf("Enter 2 to Delete an Item.\n");
        printf("Enter 3 to Add new Aisles.\n");
        printf("Enter 4 to Delete an Aisles.\n");
        printf("Enter 5 to check availability of particular item for particular quantity and within the expiry date.\n");
        printf("Enter 6 to generate a message when any particular item goes beyond its defined threshold "
               "quantity.\n");
        printf("Enter 7 to generate a message on a certain ready-to-eat item if its expiry is within one "
               "week.\n");
        printf("Enter 8 to generate a message when certain dairy product is likely to expire within one day.\n");
        printf("Enter 9 to merge two aisles in a single aisle.\n");
        printf("Enter 10 to Bill an item\n");
        printf("Enter 11 to display items more often bought with a particular item\n");
        printf("Enter 12 for range search\n");
        printf("Enter 0 to exit\n");

        switch (system_readInt()) {
            case 1: {
                printf("Enter 0 to update existing item or 1 to add new item\n");
                if (system_readInt()) {
                    printf("Enter Threshold Quantity | Expiry date(DD-MM-YYYY) | Item quantity | Item name | Item ID\n");
                    wlmrt_add_new_item(
                            wlmrt_parse_item(
                                    system_readInt(),
                                    system_read(50),
                                    system_readInt(),
                                    system_read(20),
                                    system_readInt()
                            )
                    );
                } else {
                    printf("Enter Id");
                    wlmrt_update_item_by_id(system_readInt());
                }
            }
            break;

            case 2: {
                printf("Enter item ID to delete\n");                     
                wlmrt_delete_item_by_id(system_readInt());
            }
            break;

            case 3: {
                printf("Enter number of Aisles to add\n");
                wlmrt_add_aisles(system_readInt());
            }
            break;

            case 4: {
                printf("Enter aisles no. to delete\n");
                wlmrt_delete_aisle(system_readInt());
            }
            break;

            case 5: {
                printf("Enter item id, quantity, and expiry (DD-MM-YYYY)\n");
                int id = system_readInt();
                int quantity = system_readInt();
                char *expiry = system_read(20);
                if (wlmrt_check_availability(id, quantity, expiry)) {
                    printf("Available\n");
                } else {
                    printf("Not available\n");
                }
            }
            break;

            case 6: { wlmrt_display_threshold_alert(); }
            break;

            case 7: { wlmrt_display_read_to_eat_expiry(); }        //Expiry within 1 week
            break;

            case 8: { wlmrt_display_dairy_expiry_oneDay(); }
            break;

            case 9: {
                printf("Enter aisles number to merge and with whom \n");
                wlmrt_merge_aisles(system_readInt(), system_readInt());

            }
            break;

            case 10: {
                printf("Enter Customer name\n");
                char *name = system_read(50);
                printf("Enter number of items purchased\n");
                int no_item = system_readInt();
                Cart customer_cart[no_item];
                for (int iterator = 0; iterator < no_item; ++iterator) {
                    printf("Enter %d item id and its count : ",iterator+1);
                    customer_cart[iterator].item_id = system_readInt();
                    customer_cart[iterator].count = system_readInt();
                }
                Item_node *purchased_items = wlmrt_purchase_new_item(customer_cart, no_item);
                if (purchased_items) { wlmrt_add_customer(name, purchased_items); }
                wlmrt_generate_invoice(name, purchased_items);
            }
            break;

            case 11: {
                printf("Enter Item id \n");
                wlmrt_item_more_often_bought(system_readInt());
                printf("Enter 0 to print unordered List :\n");
                printf("Enter 1 to print ordered List : \n");
                if (system_readInt()) {
                    EntrySet *set = hashmap_pull_ordered_set();
                    for (int i = 0; set[i].count!=0 && i < HASH_TABLE_SIZE; ++i) {
                        printf("Item id : %d  | Item quantity : %d\n",set[i].id,set[i].count);
                    }
                }
                else {
                    hashmap_display();
                }


            }
            break;

            case 12 : {
                wlmrt_display_range_of_items(system_readInt(), system_readInt());
            }
            break;
            default:
                flag = 0;
        }
    }


    wlmrt_write_products_list();


}
