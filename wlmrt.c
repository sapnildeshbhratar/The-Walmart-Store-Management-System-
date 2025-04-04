

#include "wlmrt.h"
#include <string.h>
#include "cJSON.h"
#include "system.h"
#include "map.h"
#include "bPLUS_Tree.h"

static int aisles_table[MAX_AISLES] = {0};
static int AISLES = 5;
static Item_node *products[MAX_AISLES] = {0}; // Products and newProducts   ?
static bPLUS *new_products[MAX_AISLES] = {0};
struct _bill
{
    Customer *consumer;
    bool init;
} billList;
void wlmrt_load_bill_data();

void wlmrt_load_database(const char *fBuf)
{

    FILE *file = fopen(fBuf, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    fread(buffer, 1, fileSize, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    cJSON *item;
    cJSON_ArrayForEach(item, json)
    {
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON *itemName = cJSON_GetObjectItemCaseSensitive(item, "item_name");
        cJSON *quantity = cJSON_GetObjectItemCaseSensitive(item, "quantity");
        cJSON *expiryDate = cJSON_GetObjectItemCaseSensitive(item, "expiry_date");
        cJSON *threshold = cJSON_GetObjectItemCaseSensitive(item, "threshold");

        int item_id = id->valueint;
        char item_name[100];
        strcpy(item_name, itemName->valuestring);
        int item_quantity = quantity->valueint;
        char expiry[100];
        strcpy(expiry, expiryDate->valuestring);
        int item_threshold = threshold->valueint;
        wlmrt_add_new_item(
            wlmrt_parse_item(item_id,
                             item_name, item_quantity,
                             expiry,
                             item_threshold));
    }

    free(buffer);
    cJSON_Delete(json);
    wlmrt_load_bill_data();
}

Item_node *wlmrt_parse_item(int id, char *name, int quantity, char *date, int threshold)
{
    Item_node *nptr = (Item_node *)malloc(sizeof(Item_node));
    nptr->item_id = id;
    strcpy(nptr->item_name, name);
    strcpy(nptr->expire_date, date);
    nptr->quantity = quantity;
    nptr->threshold_quantity = threshold;
    nptr->next = NULL;
    return nptr;
}

Item_node *wlmrt_get_item_by_id(int id)
{
    int aisles_no = id / ID_FORMAT;
    aisles_no -= 1;
    aisles_no = aisles_table[aisles_no];
    bPLUS *temp2 = new_products[aisles_no];
    Item_node *node = malloc(sizeof(Item_node));
    node->item_id = id;
    bPLUS *leaf = bPLUS_Search(temp2, node);
    if (leaf)
    {
        printf("Product found");
        return bPLUS_ParseObject(leaf, node);
    }
    return null;
}

void wlmrt_free_item(Item_node *item)
{
    if (item != NULL)
    {
        free(item);
    }
}

void wlmrt_add_new_item(Item_node *item)
{
    int id = item->item_id;
    int aisles_no = id / ID_FORMAT;
    aisles_no -= 1;
    aisles_no = aisles_table[aisles_no];
    new_products[aisles_no] = bPLUS_Offer(new_products[aisles_no], item);
    //    Item_node *temp = products[aisles_no];
    //    Item_node *prev = products[aisles_no];
    //    while (temp && temp->item_id < item->item_id) {
    //        prev = temp;
    //        temp = temp->next;
    //    }
    //
    //    if (!prev) {
    //        products[aisles_no] = item;
    //    }
    //    else if (!temp) {
    //        prev->next = item;
    //    }
    //    else if (temp == prev) {
    //        item->next = products[aisles_no];
    //        products[aisles_no] = item;
    //    }
    //    else {
    //        item->next = temp;
    //        prev->next = item;
    //    }
}

void wlmrt_refresh_product_list()
{
    for (int iterable = 0; iterable < AISLES; ++iterable)
    {
        products[iterable] = NULL;
    }
}

void wlmrt_delete_item_by_id(int id)
{
    int aisles_no = id / ID_FORMAT;
    aisles_no -= 1;
    bPLUS *temp2 = new_products[aisles_table[aisles_no]];
    new_products[aisles_table[aisles_no]] = bPLUS_Remove(temp2, id);
    //    Item_node *temp = products[aisles_table[aisles_no]];
    //    Item_node *prev = NULL;
    //    while (temp && temp->item_id != id) {
    //        prev = temp; temp = temp->next;
    //    }
    //    if (!prev) { products[aisles_no] = temp->next; }
    //    else if (temp) { prev->next = temp->next; }
    //
    //    if (temp) { wlmrt_free_item(temp); }
}

void wlmrt_display_all_items()
{
    for (int i = 0; i < AISLES; ++i)
    {
        bPLUS *first = bPLUS_GetFirst(new_products[i]);
        printf("Aisles : %d\n\n", i + 1);
        while (first)
        {
            for (int j = 0; j < first->count; ++j)
            {
                wlmrt_display_item(first->data.val[j]);
            }
            first = first->ptr.local.next;
        }
        printf("\n");
    }
}

void wlmrt_update_item_by_id(int id)
{
    Item_node *nptr = wlmrt_get_item_by_id(id);
    if (nptr)
    {
        printf("Enter New Quantity : ");
        int quantity = system_readInt();
        printf("Enter New Expiry Date : ");
        char *expiry = system_read(100);
        printf("Enter New Threshold : ");
        int threshold = system_readInt();
        nptr->quantity = quantity;
        nptr->threshold_quantity = threshold;
        strcpy(nptr->expire_date, expiry);
        free(expiry);
    }
    else
    {
        printf("Item Does not Exits\n");
    }
}

void wlmrt_init_aisles_table()
{
    for (int iterable = 0; iterable < AISLES; ++iterable)
    {
        aisles_table[iterable] = iterable;
    }
}

void wlmrt_delete_aisle(int aisle_no)
{
    aisle_no -= 1;

    new_products[aisle_no] = NULL;
}

bool wlmrt_check_availability(int id, int quantity, char *expiry)
{
    int availability = true;
    Item_node *nptr = wlmrt_get_item_by_id(id);
    if (!nptr)
    {
        return false;
    }
    int expiry_ = date_get(expiry) + date_getMonth(expiry) * 100 + date_getYear(expiry) * 10000;
    int item_expiry = date_get(nptr->expire_date) + date_getMonth(nptr->expire_date) * 100 + date_getYear(nptr->expire_date) * 10000;

    if (nptr->quantity >= quantity)
    {
        if (expiry_ > item_expiry)
        {
            availability = false;
        }
    }
    else
    {
        availability = false;
    }
    return availability;
}

void wlmrt_display_threshold_alert()
{

    for (int aisles = 0; aisles < AISLES; ++aisles)
    {
        bPLUS *first = bPLUS_GetFirst(new_products[aisles]);
        while (first)
        {
            for (int i = 0; i < first->count; ++i)
            {
                if (first->data.val[i] && first->data.val[i]->quantity < first->data.val[i]->threshold_quantity)
                {
                    wlmrt_display_item(first->data.val[i]);
                }
            }
            first = first->ptr.local.next;
        }
    }
}

void wlmrt_display_item(Item_node *pItem)
{
    if (pItem)
        printf("{ Name: %s , Item ID: %d, Quantity: %d, Threshold Quantity: %d, Expiry: %s }\n",
               pItem->item_name, pItem->item_id, pItem->quantity, pItem->threshold_quantity, pItem->expire_date);
    else
        printf("{ NULL }\n");
}

void wlmrt_display_read_to_eat_expiry()
{
    int aisles_index = aisles_table[3];
    bPLUS *first = bPLUS_GetFirst(new_products[aisles_index]);
    int curr_date = system_currYear() * 10000 + system_currMonth() * 100 + system_currDate();
    while (first)
    {
        for (int i = 0; i < first->count; ++i)
        {
            Item_node *temp = first->data.val[i];
            int date_ = date_get(temp->expire_date) + date_getMonth(temp->expire_date) * 100 + date_getYear(temp->expire_date) * 10000;
            if ((date_ - curr_date) <= 7 && (date_ - curr_date) > -1)
            {
                wlmrt_display_item(temp);
            }
        }
        first = first->ptr.local.next;
    }
}

void wlmrt_display_dairy_expiry_oneDay()
{
    int aisles_index = aisles_table[0];
    int curr_date = system_currYear() * 10000 + system_currMonth() * 100 + system_currDate();
    bPLUS *first = bPLUS_GetFirst(new_products[aisles_index]);
    while (first)
    {
        for (int i = 0; i < first->count; ++i)
        {
            Item_node *temp = first->data.val[i];
            int date_ = date_getYear(temp->expire_date) * 10000 + date_getMonth(temp->expire_date) * 100 + date_get(temp->expire_date);
            if ((date_ - curr_date) == 1)
            {
                wlmrt_display_item(temp);
            }
        }
        first = first->ptr.local.next;
    }
}

void wlmrt_merge_aisles(int aisles1, int aisles2)
{
    aisles1 -= 1;
    aisles2 -= 1;
    bPLUS *temp = bPLUS_GetFirst(new_products[aisles1]);
    bPLUS *temp2 = bPLUS_GetFirst(new_products[aisles2]);
    bPLUS *root = bPLUS_CreateNodeInternal(INT_MAX);
    if (aisles1 < aisles2)
    {
        while (temp->ptr.local.next)
        {
            temp = temp->ptr.local.next;
        }
        temp->ptr.local.next = temp2;
        temp2->ptr.local.prev = temp;
        root->ptr.child[0] = new_products[aisles1];
        root->ptr.child[1] = new_products[aisles2];
        root->data.idx[0] = temp2->data.val[0]->item_id;
    }
    else
    {
        while (temp2->ptr.local.next)
        {
            temp2 = temp2->ptr.local.next;
        }
        temp2->ptr.local.next = temp;
        temp->ptr.local.prev = temp2;
        root->ptr.child[0] = temp2;
        root->ptr.child[0] = new_products[aisles2];
        root->ptr.child[1] = new_products[aisles1];
        root->data.idx[0] = temp->data.val[0]->item_id;
    }
    new_products[aisles1] = root;
    new_products[aisles2] = null;
}

// BILL SECTION

void add_to_bill(const char *name, bPLUS *cart);
void __init__bill()
{
    billList.consumer = NULL;
    billList.init = 1;
}

Item_node *wlmrt_purchase_new_item(Cart *items, int no_of_items)
{
    Item_node *res = malloc(sizeof(Item_node));
    res->next = NULL;
    Item_node *temp = res;
    for (int iterator = 0; iterator < no_of_items; ++iterator)
    {
        Cart item = items[iterator];
        Item_node *node = wlmrt_get_item_by_id(item.item_id);
        if (node)
        {
            int no = item.count;
            node->quantity -= no;
            if (node->quantity < 0)
            {
                printf("Sufficient no not available\n");
                node->quantity += no;
            }
            else
            {
                Item_node *purchased_item = malloc(sizeof(Item_node));
                purchased_item->item_id = item.item_id;
                strcpy(purchased_item->item_name, node->item_name);
                purchased_item->quantity = item.count;
                purchased_item->next = NULL;
                temp->next = purchased_item;
                temp = temp->next;
            }
        }
        else
        {
            printf("%d Item doesn't Exits\n", item.item_id);
        }
    }
    Item_node *front = res;
    res = res->next;
    free(front);
    return res;
}

void wlmrt_add_customer(const char *name, Item_node *cart)
{
    Customer *consumer = malloc(sizeof(Customer));
    strcpy(consumer->name, name);
    bPLUS *con = null;
    while (cart)
    {
        con = bPLUS_Offer(con, cart);
        cart = cart->next;
    }
    consumer->front = con;
    consumer->next = NULL;
    strcpy(consumer->name, name);
    if (!billList.init)
    {
        __init__bill();
    }
    Customer *temp = billList.consumer;
    if (temp)
    {
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = consumer;
    }
    else
    {
        billList.consumer = consumer;
    }
}

void wlmrt_generate_invoice(const char *name, Item_node *cart)
{
    bPLUS *con = null;
    Item_node *tp = cart;
    while (tp)
    {
        con = bPLUS_Offer(con, cart);
        tp = tp->next;
    }
    add_to_bill(name, con);
    string_builder *str = builder();
    str_append(str, "......WALMART STORE......\n");
    str_append(str, "Customer name : ");
    str_append(str, name);
    str_append(str, "\n");
    str_append(str, system_timeStamp());
    str_append(str, "\n");
    char file_name[50];
    strcpy(file_name, "../Invoice/");
    strcat(file_name, name);
    strcat(file_name, ".txt");
    str_append(str, "\n");
    while (cart)
    {

        str_append(str,
                   integer_parseString(cart->item_id));
        str_append(str,
                   " ");
        str_append(str,
                   integer_parseString(cart->quantity));
        str_append(str,
                   " ");
        str_append(str,
                   cart->item_name);

        printf("\n%s\n", str_get(str));
        str_serialize(str, file_name);
        str_clear(str);
        cart = cart->next;
    }
    printf("%s has been successfully billed\n", name);
}

void wlmrt_item_more_often_bought(int item_id, ...)
{
    map_use_hashmap();
    Customer *temp = billList.consumer;
    while (temp)
    {
        bPLUS *list = temp->front;
        bool check = 0;
        list = bPLUS_GetFirst(list);
        while (list && !check)
        {
            for (int i = 0; i < list->count; ++i)
            {
                if (list->data.val[i]->item_id == item_id)
                {
                    check = 1;
                    break;
                }
            }
            list = list->ptr.local.next;
        }
        if (check)
        {
            list = temp->front;
            list = bPLUS_GetFirst(list);
            while (list)
            {
                for (int i = 0; i < list->count; ++i)
                {
                    if (list->data.val[i]->item_id != item_id)
                    {
                        if (hashmap_contains_key(list->data.val[i]->item_id))
                        {
                            EntrySet set = hashmap_get(list->data.val[i]->item_id);
                            EntrySet mapSet = {list->data.val[i]->item_id, list->data.val[i]->quantity + set.count};
                            hashmap_add_to_map(mapSet);
                        }
                        else
                        {
                            EntrySet mapSet = {list->data.val[i]->item_id, list->data.val[i]->quantity};
                            hashmap_add_to_map(mapSet);
                        }
                    }
                }

                list = list->ptr.local.next;
            }
        }
        temp = temp->next;
    }
}

void wlmrt_add_aisles(int size)
{
    if (AISLES + size < MAX_AISLES)
    {
        AISLES += size;
        printf("<<< Aisles added successfully >>>\n");
    }
}

void add_to_bill(const char *name, bPLUS *cart)
{
    FILE *fp = fopen("../bill.json", "r");
    cJSON *root = NULL;

    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (file_size == 0)
        {
            root = cJSON_CreateArray();
        }
        else
        {
            char *file_content = (char *)malloc(file_size + 1);
            fread(file_content, 1, file_size, fp);
            fclose(fp);

            root = cJSON_Parse(file_content);
            free(file_content);
            if (root == NULL)
            {
                printf("Error parsing file\n");
                return;
            }
        }
    }
    else
    {
        root = cJSON_CreateArray();
    }

    cJSON *customer = cJSON_CreateObject();
    cJSON_AddStringToObject(customer, "customerName", name);

    cJSON *items_array = cJSON_CreateArray();
    cJSON_AddItemToObject(customer, "items", items_array);
    bPLUS *temp = bPLUS_GetFirst(cart);
    while (temp)
    {
        for (int i = 0; i < temp->count; ++i)
        {
            Item_node *item = temp->data.val[i];
            cJSON *item_object = cJSON_CreateObject();
            cJSON_AddNumberToObject(item_object, "itemId", item->item_id);
            cJSON_AddStringToObject(item_object, "itemName", item->item_name);
            cJSON_AddNumberToObject(item_object, "quantity", item->quantity);
            cJSON_AddItemToArray(items_array, item_object);
        }
        temp = temp->ptr.local.next;
    }

    cJSON_AddItemToArray(root, customer);

    char *json_str = cJSON_Print(root);

    FILE *fptr = fopen("../bill.json", "w");
    if (fptr == NULL)
    {
        fprintf(stderr, "Error opening file %s for writing\n", "../bill.json");
        free(json_str);
        cJSON_Delete(root);
        return;
    }

    fprintf(fptr, "%s", json_str);

    fclose(fptr);
    free(json_str);
    cJSON_Delete(root);
}

void wlmrt_load_bill_data()
{

    FILE *fp = fopen("../bill.json", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s for reading\n", "../bill.json");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size == 0)
    {
        fclose(fp);
        return; // File is empty, no data to load
    }

    char *file_content = (char *)malloc(file_size + 1);
    fread(file_content, 1, file_size, fp);
    fclose(fp);

    cJSON *root = cJSON_Parse(file_content);
    free(file_content);

    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
        }
        else
        {
            fprintf(stderr, "Error parsing JSON: Unknown error\n");
        }
        return;
    }

    cJSON *customer;
    cJSON_ArrayForEach(customer, root)
    {
        Item_node *customer_list = NULL;
        cJSON *customerName = cJSON_GetObjectItemCaseSensitive(customer, "customerName");
        cJSON *items_array = cJSON_GetObjectItemCaseSensitive(customer, "items");

        if (cJSON_IsString(customerName) && cJSON_IsArray(items_array))
        {
            char *name = cJSON_GetStringValue(customerName);

            cJSON *item;
            cJSON_ArrayForEach(item, items_array)
            {
                cJSON *itemId = cJSON_GetObjectItemCaseSensitive(item, "itemId");
                cJSON *itemName = cJSON_GetObjectItemCaseSensitive(item, "itemName");
                cJSON *quantity = cJSON_GetObjectItemCaseSensitive(item, "quantity");

                if (cJSON_IsNumber(itemId) && cJSON_IsString(itemName) && cJSON_IsNumber(quantity))
                {
                    int id = itemId->valueint;
                    char *item_name = itemName->valuestring;
                    int quant = quantity->valueint;

                    Item_node *new_item = (Item_node *)malloc(sizeof(Item_node));
                    new_item->item_id = id;
                    strcpy(new_item->item_name, item_name);
                    new_item->quantity = quant;
                    new_item->next = NULL;

                    if (customer_list == NULL)
                    {
                        customer_list = new_item;
                    }
                    else
                    {
                        Item_node *temp = customer_list;
                        while (temp->next != NULL)
                        {
                            temp = temp->next;
                        }
                        temp->next = new_item;
                    }
                }
            }
            wlmrt_add_customer(name, customer_list);
        }
    }

    cJSON_Delete(root);
}

void wlmrt_write_products_list()
{
    FILE *fptr = fopen("../database.json", "w");
    if (fptr == NULL)
    {
        printf("Error opening File\n");
        return;
    }
    cJSON *array = cJSON_CreateArray();

    for (int iterator = 0; iterator < AISLES; ++iterator)
    {
        bPLUS *first = bPLUS_GetFirst(new_products[iterator]);
        Item_node *temp;
        while (first)
        {
            for (int i = 0; i < first->count; ++i)
            {
                temp = first->data.val[i];
                cJSON *object = cJSON_CreateObject();
                cJSON_AddNumberToObject(object, "id", temp->item_id);
                cJSON_AddStringToObject(object, "item_name", temp->item_name);
                cJSON_AddNumberToObject(object, "quantity", temp->quantity);
                cJSON_AddStringToObject(object, "expiry_date", temp->expire_date);
                cJSON_AddNumberToObject(object, "threshold", temp->threshold_quantity);
                cJSON_AddItemToArray(array, object);
            }
            first = first->ptr.local.next;
        }
    }

    char *json_str = cJSON_Print(array);
    fprintf(fptr, "%s", json_str);
    cJSON_Delete(array);
    free(json_str);
    fclose(fptr);
}

void wlmrt_display_range_of_items(int item_id_1, int item_id_2)
{
    if (item_id_1 / ID_FORMAT == item_id_2 / ID_FORMAT)
    {
        int min_id = (item_id_1 < item_id_2) ? item_id_1 : item_id_2;
        int max_id = (item_id_1 > item_id_2) ? item_id_1 : item_id_2;
        Item_node *node = malloc(sizeof(Item_node));
        node->item_id = min_id;
        bPLUS *first = bPLUS_Search(new_products[aisles_table[item_id_1 / ID_FORMAT - 1]], node);
        int found = 1, start = 0;
        while (first && found != 0)
        {
            for (int i = 0; i < first->count && found; ++i)
            {
                if (first->data.val[i]->item_id == min_id)
                {
                    start = 1;
                }
                if (start)
                {
                    wlmrt_display_item(first->data.val[i]);
                }
                if (first->data.val[i]->item_id == max_id)
                {
                    found = 0;
                }
            }
            first = first->ptr.local.next;
        }
    }
}