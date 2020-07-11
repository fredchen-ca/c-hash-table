/*
 * << Basic Hash Table Utility C Exercise >>
 *
 * Description:
 *   Build a hash table with sorted bucket link list in ascending order,
 * duplicated key will be discarded.
 *
 * Supported Functions:
 *  - Key Insertion with User Data
 *  - Key Deletion
 *  - Key Seeking with User Data retrieval
 *  - All Keys Listing
 *
 * Future Expansion:
 *  - Multiple Hash Table Instances
 *  - Table Locking
 *  - Optional User Data
 *
 * Author: Frederic Chen
 * E-mail: fredchen@shaw.ca
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define     BUCKET_SIZE         50
#define     HASH_MULTIPLIER     31      // a prime number
#define     HASH_INIT_VALUE     5381    // also a prime number


// bucket link list node
typedef struct node_s {
    char    *key;
    char    *user_data;
    size_t   data_size;
    struct node_s *next;
} node_t;

// hash table bucket header
node_t *bucket[BUCKET_SIZE];


/*
 * Derive the hash value (bucket index) out of input key string
 *
 * input: key, input string
 * return: bucket index (0 ~ (BUCKET_SIZE-1))
 */
int hash(char *key) {
    char    *ch;
    int     index;
    unsigned int value;

    /*
     * to randomize the sum with each character of input key string
     * by calculated with the prime number
     */
    value = HASH_INIT_VALUE;
    ch = key;
    while (*ch != '\0') {
        value = value * HASH_MULTIPLIER + *ch++;
        value = (value << 16) + (value >> 16); // swap high and low order word
    }

    // determine the bucket index
    index = (int)(value % BUCKET_SIZE);
    return index;
}


/*
 * Add a new hash key into hash table with user data
 *
 * Note:
 *  - duplicated key will be detected and discarded.
 *
 * input: key,
 *        user_data,
 *        data_size,
 * return:
 *      0: success
 *      1: fail
 *
 */
int add_hash_key(char *key, char *user_data, size_t data_size) {
    int     index, res;
    char    *data_p, *user_rec_p;
    node_t  *ptr, **prev_p, *node_p;

    // setup new node for input key with user data
    node_p = calloc(1, sizeof(node_t));
    data_p = malloc(strlen(key)+1);
    strcpy(data_p, key);
    node_p->key = data_p;
    user_rec_p = malloc(data_size);
    memcpy(user_rec_p, user_data, (int)data_size);
    node_p->user_data = user_rec_p;
    node_p->data_size = data_size;

    // determine the bucket list to store the key
    index = hash(key);
    prev_p = &bucket[index];

    /*
     *insert new node into the ascending order bucket list
     */
    while (*prev_p != NULL) {
        // get next node
        ptr = *prev_p;
        res = strcmp(key, ptr->key);
        if (!res) {
            printf("Warning: Duplicated key: %s discarded\n", key);
            return 1;
        } else if (res < 0) {
            // insert the input key before current node has bigger key
            node_p->next = ptr;
            *prev_p = node_p;
            printf("Key: %s inserted before key: %s at the bucket[%d]\n",(*prev_p)->key, ptr->key, index);
            return 0;
        }

        // continue to next node
        prev_p = &ptr->next;
    }

    // append new node to the bucket list
    *prev_p = node_p;
    printf("Key:%s appended to the bucket[%d]\n",(*prev_p)->key, index);
    return 0;
}


/*
 * find a specific key from hash table and return its user data
 *
 * input: key,
 * output:
 *        user_data,
 *        data_size,
 * return:
 *      0: key found
 *      1: key not existed
 */
int find_hash_key(char *key, char *user_data, size_t *data_size) {
    int     index, res;
    node_t  *ptr;

    // determine the bucket list of the key stored
    index = hash(key);
    ptr = bucket[index];

    /*
     * search input key and return its user data and size from the ascending order bucket list
     */
    while (ptr != NULL) {
        // get next node
        res = strcmp(key, ptr->key);
        if (!res) {
            *data_size = ptr->data_size;
            memcpy(user_data, ptr->user_data, (int)*data_size);
            printf("Key: %s found at the bucket[%d] with user_data: %s, size: %ld\n",ptr->key, index, user_data, *data_size);
            return 0;
        } else if (res < 0) {
            // meet larger key value, search stop
            break;
        }

        // continue to next node
        ptr = ptr->next;
    }

    printf("Warning: key: %s not found\n", key);
    return 1;
}


/*
 * delete a specific key from hash table
 *
 * input: key,
 * return:
 *      0: key found
 *      1: key not existed
 */
int delete_hash_key(char *key) {
    int     index, res;
    node_t  *ptr, **prev_p;

    // determine the bucket list of the key stored
    index = hash(key);
    ptr = bucket[index];
    prev_p = &bucket[index];

    /*
     * search input key and return its user data and size from the ascending order bucket list
     */
    while (ptr != NULL) {
        // get next node
        res = strcmp(key, ptr->key);
        if (res == 0) {
            // remove current node
            *prev_p = ptr->next;
            // free up key, user data and node space
            if (ptr->data_size > 0) {
                free(ptr->user_data);
            }
            free(ptr->key);
            free(ptr);
            printf("Key: %s got deleted\n", key);
            return 0;
        } else if (res < 0) {
            // meet larger key value, search stop
            break;
        }

        // continue to next node
        prev_p = &ptr->next;
        ptr = ptr->next;
    }

    printf("Warning: key: %s not existed\n", key);
    return 1;
}


/*
 *  list all keys from the hash table
 */
void list_hash_keys(void) {
    int i, count;
    node_t *ptr;

    count = 0;
    for (i=0; i<BUCKET_SIZE; i++) {
        ptr = bucket[i];
        if (ptr == NULL) count++;
        while (ptr != NULL) {
            printf("Find Bucket[%d] key: %s, user_data: %s, size= %ld bytes\n", i, ptr->key, ptr->user_data, ptr->data_size);
            ptr = ptr->next;
        }
    }
    printf("End of hash keys listing, empty bucket:%d\n", count);
}

int main(int argc, char *argv[])
{
    int i;
    size_t  data_size;
    char user_data[256];
    char *key;

    char* data[] = {
	"Jack",
	"Oliver",
    	"Charlie",
    	"Harry",
    	"Jacob",
    	"Alfie",
    	"Noah",
    	"Oscar",
    	"George",
    	"James",
    	"Thomas",
    	"Leo",
    	"William",
    	"Joshua",
    	"Logan",
    	"Freddie",
    	"Archie",
    	"Max",
    	"Henry",
    	"Ethan",
    	"Mason",
    	"Lucas",
    	"Riley",
    	"Daniel",
    	"Joseph",
    	"Jake",
    	"Harrison",
    	"Theo",
    	"Alexander",
    	"Samuel",
    	"Isaac",
    	"Dylan",
    	"Finley",
    	"Edward",
    	"Adam",
    	"Benjamin",
    	"Lewis",
    	"Harley",
    	"Tyler",
    	"Sebastian",
    	"Luke",
    	"Toby",
    	"Jayden",
    	"Kian",
    	"Tommy",
    	"Alex",
    	"Ollie",
    	"Harvey",
    	"Matthew",
    	"Blake",
    };

    printf("Building hash table of buckets:%i ...\n", BUCKET_SIZE);

    /*
     * insert all keys with their tested user data into hash table
     */
    for (i=0; i<(sizeof(data)/sizeof(char *)); i++) {
        strcpy(user_data, data[i]);
        strcat(user_data,"_data"); // compose unique user_data
        add_hash_key(data[i], user_data, strlen(user_data)+1);
    }

    /*
     * Hash Key Validation
     */
    key = "Matthew";  // a existed key
    if (add_hash_key(key, NULL, 0) != 1) {
        printf("Duplicated key: %s insertion validation fails\n", key);
    }
    key = "Matthew";  // a existed key
    if (find_hash_key(key, user_data, &data_size) != 0) {
        printf("Existed key: %s searching validation fails\n", key);
    }
    key = "Dummy";  // a not existed key
    if (find_hash_key(key, user_data, &data_size) != 1) {
        printf("Non-existed key: %s searching validation fails\n", key);
    }
    key = "Dummy";  // a not existed key
    if (delete_hash_key(key) != 1) {
        printf("Non-existed key: %s deletion validation fails\n", key);
    }

    /*
     * List all keys and their user data from hash table
     */
    list_hash_keys();

    /*
     * delete all keys from hash table
     */
    for (i=0; i<(sizeof(data)/sizeof(char *)); i++) {
        delete_hash_key(data[i]);
    }

    return 0;
}

