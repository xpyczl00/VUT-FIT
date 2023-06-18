/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{

  while (tree != NULL)
  {

    if (key > tree->key)
      tree = tree->right;

    else if (key < tree->key)
      tree = tree->left;
    else
    {
      *value = tree->value;
      return true;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
  bst_node_t **tmp = tree;

  while (*tmp != NULL)
  {
    if (key > (*tmp)->key)
    {

      tmp = &(*tmp)->right;
    }
    else if (key < (*tmp)->key)
    {

      tmp = &(*tmp)->left;
    }
    else
    {
      (*tmp)->value = value;
      return;
    }
  }
  bst_node_t *add = calloc(2, sizeof(bst_node_t));
  add->value = value;
  add->key = key;
  add->right = NULL;
  add->left = NULL;
  *tmp = add;
  tree = tmp;
  return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *rightest = *tree;
  bst_node_t *before_rightest = target; // target
  bst_node_t *temp = NULL;
  if (target->right)
    temp = target->right;
  while (rightest->right != NULL)
  {
    /* if (rightest->right != NULL) */
    before_rightest = rightest;
    rightest = rightest->right;
  }

  target->key = rightest->key;
  target->value = rightest->value;
  if (rightest->left == NULL)
    before_rightest->right = NULL;
  else
    before_rightest->right = rightest->left;
  free(rightest);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *tmp = *tree;
  bst_node_t *parent = NULL;
  int nodes = -2;
  //0 nula podstromov
  // -1 lavy podstrom z pravy podstrom
  //2 oba
  bool l = false, r = false;
  while (tmp != NULL)
  {
    if (key > tmp->key)
    {

      if (tmp->key != key)
        parent = tmp;

      tmp = tmp->right;
      l = false;
      r = true;
    }
    else if (key < tmp->key)
    {

      if (tmp->key != key)
        parent = tmp;

      tmp = tmp->left;
      l = true;
      r = false;
    }
    else
    {
      //ak je koren

      if (tmp->left == NULL && tmp->right == NULL)
      {
        nodes = 0;
      }
      else if (!tmp->left && tmp->right)
      {

        nodes = 1;
      }
      else if (tmp->left && !tmp->right)
      {

        nodes = -1;
      }
      else
        nodes = 2;
      break;
    }
  }
  switch (nodes)
  {
  case 0:
    if (!parent)
    {
      *tree = NULL;
      free(tmp);
      return;
    }
    else
    {
      if (l)
      {
        parent->left = NULL;
        free(tmp);
      }
      else if (r)
      {
        parent->right = NULL;
        free(tmp);
      }
      return;
    }
  case -1:

    if (!parent)
    {

      parent = tmp->left;
      free(tmp);
      *tree = parent;
      return;
    }
    else
    {
      if (l)
      {
        parent->left = tmp->left;
        free(tmp);
      }
      else if (r)
      {
        parent->right = tmp->left;
        free(tmp);
      }
      return;
    }

  case 1:
    if (!parent)
    {
      parent = tmp->right;
      free(tmp);
      *tree = parent;
      return;
    }
    else
    {
      if (l)
      {
        parent->left = tmp->right;
        free(tmp);
      }
      else if (r)
      {
        parent->right = tmp->right;
        free(tmp);
      }
      return;
    }
  case 2:

    bst_replace_by_rightmost(tmp, &tmp->left);

    break;

  default:
    break;
  }
  return;
}
/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
  bst_node_t *ptr = *tree;
  stack_bst_t stack;
  stack_bst_init(&stack);
  do
  {
    if (ptr == NULL)
    {
      if (!stack_bst_empty(&stack))
      {
        ptr = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    }
    else
    {
      if (ptr->right != NULL)
      {
        stack_bst_push(&stack, ptr->right);
        ptr->right = NULL;
      }
      bst_node_t *act = ptr;
      ptr = ptr->left;
      act->left = NULL;
    }
  } while ((ptr != NULL) || (!stack_bst_empty(&stack)));
  *tree = NULL;
  free(*tree);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree)
  {
    bst_print_node(tree);
    if (tree->right)
      stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);

  do
  {
    bst_leftmost_preorder(tree, &stack);
    tree = stack_bst_top(&stack);
    tree = tree->right;
    stack_bst_pop(&stack);
  } while (!stack_bst_empty(&stack));

  bst_print_node(tree);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);

  do
  {
    bst_leftmost_inorder(tree, &stack);
    tree = stack_bst_top(&stack);
    if (tree->right != NULL)
    {
      tree->right;
    }
    else
    {
      bst_print_node(tree);
      stack_bst_pop(&stack);
      tree = stack_bst_top(&stack);
      bst_print_node(tree);
      stack_bst_pop(&stack);
      tree = tree->right;
    }
  } while (!stack_bst_empty(&stack));
  bst_print_node(tree);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while (tree)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bool_t bstack;
  stack_bst_init(&stack);
  stack_bool_init(&bstack);

  bst_leftmost_postorder(tree, &stack, &bstack);

  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);

    if (!stack_bool_top(&bstack))
    {

      bst_print_node(tree);
      stack_bst_pop(&stack);
      stack_bool_pop(&bstack);
    }
    else if (stack_bool_top(&bstack))
    {

      if (tree->right)
      {
        tree = tree->right;
        stack_bool_pop(&bstack);
        stack_bool_push(&bstack, false);
        bst_leftmost_postorder(tree, &stack, &bstack);
      }
      else
      {
        bst_print_node(tree);
        stack_bst_pop(&stack);
        stack_bool_pop(&bstack);
      }
    }
  }
}
