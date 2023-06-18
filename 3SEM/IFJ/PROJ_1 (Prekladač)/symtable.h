/**
 *  Project - Compiler for IFJ22
 *
 * @brief Table of symbols for IFJ22 - semantic analysis
 *
 * @author Lukasz Pycz (xpyczl00)
 * @file symtable.h
 *
 **/

#ifndef _symtable_
#define _symtable_

#include <stdbool.h>
#include "scanner.h"
/**
 * @brief Table Size definition
 * 
 */

#define MAX_HT_SIZE 1009
extern int HT_SIZE;

/**
 * @brief Table item content definiton
 * 
 */
typedef struct values {
float item_value;             // for integers and floating point numbers
char* item_string;            // string contents
char* item_id;                // name (id), also a key (??) //USELESS
Token_type item_type;        // type from scanner.h
struct values *next_item;     // pointer to next similar structure //USELSESS
      // to check previous definition

}values_t;




/**
 * @brief Table item for func and var
 * 
 */
typedef struct ht_item {
  char *key;             
  Token_type return_val_type;
  Token_type param_type;
  int input_param_count;     // name of a function / variable
  struct values *content;      // all possible contents listed above
  struct ht_item *next;       // pointer to next similar structure  
  struct values *first_par;
} ht_item_t;


/**
 * @brief Table definiton - table consists of items with content 
 * 
 */
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

/**
 * @brief Gives the given key an index from interval 0-HT_SIZE
 * @param key 
 * @return int 
 */
int get_hash(char *key);

void set_item_null(ht_item_t * item);

/**
 * @brief Table initialization
 * @param table 
 */
void ht_init(ht_table_t *table);


/**
 * @brief Searching for an item from the table, if not found returns NULL
 * @param table 
 * @param key 
 * @return ht_item_t* 
 */
ht_item_t *ht_search(ht_table_t *table, char *key);


/**
 * @brief Inserting an item, if item already exists the value is replaced with new given
 * @param table 
 * @param item
 */
void  ht_insert(ht_table_t *table, ht_item_t * item);


/**
 * @brief Getting a pointer of the item with given key, if not found returns NULL
 * @param table 
 * @param key 
 * @return float* 
 */
ht_item_t *ht_get(ht_table_t *table, char *key);


/**
 * @brief Deleting an item with given key
 * @param table 
 * @param key 
 */
void ht_delete(ht_table_t *table, char *key);


/**
 * @brief Disposing a whole table
 * @param table 
 */
void ht_delete_all(ht_table_t *table);

#endif