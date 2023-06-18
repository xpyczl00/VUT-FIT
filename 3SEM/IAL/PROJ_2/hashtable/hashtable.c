/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
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

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  //colision
  ht_item_t *searched = (*table)[get_hash(key)];
  while (searched)
  {
    if (strcmp(key, searched->key) == 0)
      return searched;
    searched = searched->next;
  }

  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{

  ht_item_t *insert = ht_search(table, key);
  if (insert)
  {
    insert->value = value;
    return;
  }
  else
  {
    insert = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (!insert)
      return;
    insert->key = key;
    insert->value = value;
    insert->next = NULL;
    int hash_index = get_hash(key);

    insert->next = (*table)[hash_index];
    (*table)[hash_index] = insert;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *get = ht_search(table, key);

  return get ? &(get->value) : NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  int hash_index = get_hash(key);
  ht_item_t *tmp = (*table)[hash_index];
  ht_item_t *before = NULL, *after = NULL;

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
  return;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
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
