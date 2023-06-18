/**
* Project - Compiler for IFJ22
*
* @brief Tests for symtable for IFJ22 - Syntax analysis
*
* @author Matej Koreň <xkoren10> & Bohuslav Křena a.k.a. G.O.A.T.
* @file symtable_tests.c
*
**/

#include "../symtable.h"
#include <stdio.h>
#include <stdlib.h>

//--------------------
// Test Utility Header
//--------------------
#define TEST(NAME, DESCRIPTION)                                                \
  void NAME() {                                                                \
    printf("[%s] %s\n", #NAME, DESCRIPTION);                                   \
    ht_table_t *test_table;                                                    \
    init_test_table(&test_table);

#define ENDTEST                                                                \
  printf("\n");                                                                \
  ht_print_table(test_table);                                                  \
  ht_delete_all(test_table);                                                   \
  free(test_table);                                                            \
  printf("\n");                                                                \
  }

extern ht_item_t *uninitialized_item;
extern values_t * insert_values;

//--------------------
// Test Utility program
//--------------------
ht_item_t *uninitialized_item;


void ht_print_item_value(float *value) {
  if (value != NULL) {
    printf("%.2f\n", *value);
  } else {
    printf("NULL\n");
  }
}

void ht_print_item(ht_item_t *item) {
  if (item != NULL) {
    printf("(%s,%.2f)\n", item->key, item->content.item_value);
  } else {
    printf("NULL\n");
  }
}

void ht_print_table(ht_table_t *table) {
  int max_count = 0;
  int sum_count = 0;

  printf("------------HASH TABLE--------------\n");
  for (int i = 0; i < HT_SIZE; i++) {
    printf("%i: ", i);
    int count = 0;
    ht_item_t *item = (*table)[i];
    while (item != NULL) {
      printf("(%s,%.2f)", item->key, item->content.item_value);
      if (item != uninitialized_item) {
        count++;
      }
      item = item->next;
    }
    printf("\n");
    if (count > max_count) {
      max_count = count;
    }
    sum_count += count;
  }

  printf("------------------------------------\n");
  printf("Total items in hash table: %i\n", sum_count);
  printf("Maximum hash collisions: %i\n", max_count == 0 ? 0 : max_count - 1);
  printf("------------------------------------\n");
}

void init_uninitialized_item() {
  uninitialized_item = (ht_item_t *)malloc(sizeof(ht_item_t));
  uninitialized_item->key = "*UNINITIALIZED*";
  uninitialized_item->content.item_value = -1;
  uninitialized_item->next = NULL;
}

void init_test_table(ht_table_t **table) {
  (*table) = (ht_table_t *)malloc(sizeof(ht_table_t));
  for (int i = 0; i < MAX_HT_SIZE; i++) {
    (**table)[i] = uninitialized_item;
  };
}

void ht_insert_many(ht_table_t *table, ht_item_t items[], int count) {
  for (int i = 0; i < count; i++) {
    ht_insert(table, &items[i]);
  }
}



//--------------------
// Symtable Test 
//--------------------

#define INSERT_TEST_DATA(TABLE)                                                \
  ht_insert_many(TABLE, TEST_DATA, sizeof(TEST_DATA) / sizeof(TEST_DATA[0])-1);
 
const bool val = true;

ht_item_t TEST_DATA[16] = {
    {"Bitcoin", {53247.71}}, {"Ethereum", {3208.67}}, {"Binance Coin", {409.15}},
    {"Cardano", {1.82}},     {"Tether", {0.86}},      {"XRP", {0.93}},
    {"Solana", {134.50}},    {"Polkadot", {34.99}},   {"Dogecoin",{ 0.22}},
    {"USD Coin", {0.86}},    {"Uniswap", {21.68}},    {"Terra", {30.67}},
    {"Litecoin", {156.87}},  {"Avalanche", {47.03}},  {"Chainlink", {21.90}},
    {"Ethereum", {12.34}}};


// Throws missing initializers, nothing to worry about, Source: Trust me bro

void init_test() {
  printf("Hash Table - testing script\n");
  printf("---------------------------\n");
  int HT_SIZE = 13;
  printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
  printf("\n");
}

TEST(test_table_init, "Initialize the table")
ht_init(test_table);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
ht_init(test_table);
ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_insert_simple, "Insert a new item")
ht_init(test_table);
ht_insert(test_table, &TEST_DATA[1]);
ENDTEST

TEST(test_search_exist, "Search for an existing item")
ht_init(test_table);
ht_insert(test_table, &TEST_DATA[1]);
ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_insert_many, "Insert many new items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ENDTEST

TEST(test_search_collision, "Search for an item with colliding hash")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_search(test_table, "Terra");
ENDTEST

TEST(test_insert_update, "Update an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_insert(test_table, &TEST_DATA[15]);
ENDTEST

TEST(test_get, "Get an item's value")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_delete, "Delete an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Terra");
ENDTEST

TEST(test_delete_all, "Delete all the items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete_all(test_table);
ENDTEST



int main() {

  init_uninitialized_item();
  init_test();

  test_table_init();
  test_search_nonexist();
  test_insert_simple();
  test_search_exist();
  test_insert_many();
  test_search_collision();
  test_insert_update();

  test_get();
  test_delete();
  test_delete_all();
  free(uninitialized_item);
}
