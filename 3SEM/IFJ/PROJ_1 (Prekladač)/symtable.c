/**
 *  Project - Compiler for IFJ22
 *
 * @brief Table of symbols for IFJ22 - semantic analysis
 *
 * @author Lukasz Pycz (xpyczl00)
 * @file symtable.c
 *
 **/

#include "symtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/**
 * @brief Gives the given key an index from interval 0-HT_SIZE
 *
 * @param key
 * @return int
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/**
 * @brief Table initialization
 *
 * @param table
 */
void ht_init(ht_table_t *table)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  }
}

/**
 * @brief Default values for table item. Might be used
 *
 * @param item
 */
void set_item_null(ht_item_t *item)
{
  item->content = malloc(sizeof(ht_item_t));

  // Set contents
  item->content->item_id = "";
  item->content->item_string = "";
  item->content->item_value = 0.0;
  item->content->item_type = EMPTY_TOKEN;
  item->content->next_item = NULL;
  
  // Set item
  item->first_par = NULL;
  item->return_val_type = EMPTY_TOKEN;
  item->param_type = EMPTY_TOKEN;
  item->input_param_count = 0;
  item->key = "";
  item->next = NULL;  

  //fprintf(stdout,"itesm id %s \nitems string %s \nitesm value %f \nitesm type %d \nnext item %d \n",item->content->item_id,
  //item->content->item_string,
  //item->content->item_value,
  //item->content->item_type,
  //item->content->next_item);
  
}


/**
 * @brief Searching for an item from the table, if not found returns NULL
 *
 * @param table
 * @param key
 * @return ht_item_t*
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  if (table && key)
  {
    ht_item_t *searched = (*table)[get_hash(key)];
    while (searched)
    {
      if (strcmp(key, searched->key) == 0)
        return searched;
      searched = searched->next;
    }
  }
  return NULL;
}

/**
 * @brief Inserting an item, if item already exists the value is replaced with new given
 *
 * @param table
 * @param item
 */
void ht_insert(ht_table_t *table, ht_item_t *item)
{

  ht_item_t *tmp, *act = NULL;

  if (table && item->key)
  {

    if ((act = (ht_search(table, item->key))))
    {
      // Ak vkladaný prvok existuje, prepíšeme len jeho hodnotu
      act->content->item_value = item->content->item_value;
      act->content->item_string = item->content->item_string;
      act->content->item_id = item->content->item_id;
      act->content->item_type = item->content->item_type;
      act->content->next_item = item->content->next_item;
      act->param_type = item->param_type;
      act->return_val_type = item->return_val_type;
      act->input_param_count= item->input_param_count;
      act->first_par = item->first_par;


      
      }
    else
    {
      
      tmp =malloc(sizeof(ht_item_t));

      if (tmp)
      {
        tmp->content = malloc(sizeof(ht_item_t));
        tmp->key = item->key;
        
        tmp->content->item_value = item->content->item_value;
        tmp->content->item_string = item->content->item_string;
        
        tmp->content->item_id = item->content->item_id;
        
        tmp->content->item_type = item->content->item_type;
        tmp->content->next_item = item->content->next_item;
        
        tmp->first_par = item->first_par;
        tmp->input_param_count = item->input_param_count;
        tmp->return_val_type = item->return_val_type;
        tmp->param_type = item->param_type;



        tmp->next = NULL; // Po podarenej alokácii nastavíme hodnoty

        if ((act = (*table)[get_hash(item->key)])) // Vyrátame si hash podľa kľúča, ak už k nemu existujú synonymá,
          tmp->next = act;                   // pridáme ich za nový prvok

        (*table)[get_hash(item->key)] = tmp; // a uložíme ho
      }
    }
  }
  return;
}

/**
 * @brief Deleting an item with given key
 *
 * @param table
 * @param key
 */

void ht_delete(ht_table_t *table, char *key)
{
  int hash_index = get_hash(key);
  ht_item_t *tmp = (*table)[hash_index];
  ht_item_t *before = NULL, *after = NULL;
  if (table && key)
  {
    while (tmp)
    {
      after = tmp->next;

      if (strcmp(key, tmp->key) == 0)
      {
        free(tmp);
        if (!before)
        {
          (*table)[hash_index] = after;
          return;
        }
        before->next = after;
      }

      before = tmp;
      tmp = tmp->next;
    }
  }
  return;
}

/**
 * @brief Disposing a whole table
 *
 * @param table
 */

void ht_delete_all(ht_table_t *table)
{
  ht_item_t *tmp, *delete;
  for (int i = 0; i < HT_SIZE; i++)
  {
    tmp = (*table)[i];
    while (tmp)
    {
      delete = tmp;
      tmp = tmp->next;
      free(delete);
    }
    (*table)[i] = NULL;
  }
}
