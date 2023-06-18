/**
 * Kostra pro cviceni operaci nad jednosmerne vazanym seznamem.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
/**
 * struktura Object
 */
typedef struct {
  int id;
  char *name;
} Object;
 
/**
 * Polozka seznamu
 */
typedef struct item Item;
struct item {
  Object data;
  Item *next;
};
 
/**
 * Typ seznam
 */
typedef struct {
  Item *first;
} List;
 
/**
 * Inicializace seznamu. Vytvori prazdny seznam.
 */
List list_ctor()
{

List *list;
list->first=NULL;
return *list;
 //2-3
}
 
/**
 * Inicializace polozky seznamu. Z objektu vytvori polozku bez naslednika.
 */
Item *item_ctor(Object data)
{
 //4,malloc
    Item *new_item=malloc(sizeof(data));
    //malloc somehow 
    if(new_item==NULL)
        return NULL;
    new_item->data=data;
    new_item->next=NULL;
    return new_item;
   


}
 
 
/**
 * Vlozi polozku na zacatek seznamu.
 */
void list_insert_first(List *list, Item *i)
{
 //2-3
 i->next=list->first;
 list->first=i;



}
 
/**
 * Vrati true, pokud je seznam prazdny.
 */
bool list_empty(List *list)
{
    if(list->first==NULL)
        return 1;
    else 
        return 0;
 //if NULL first

}
 
/**
 * Odstrani prvni prvek seznamu, pokud je.
 */
void list_delete_first(List *list)
{
 //3,free
 Item *tmp=list->first;
 list->first=list->first->next;
 free(tmp);



}
 
/**
 * Vrati pocet polozek seznamu.
 */
unsigned list_count(List *list)
{
    //4 
    unsigned int index=0;
     for (Item *i = list->first; i!=NULL; i = i->next)
        index++;
    return index;
}
 
 
/**
 * Najde polozku seznamu s nejmensim identifikatorem. Vraci NULL, pokud je
 * seznam prazdny.
 */
Item *list_find_minid(List *list)
{   Item *minid;
    if (list->first==NULL)
        return NULL;
    else
    {
        minid=list->first;
       for(Item *i=list->first;i!=NULL;i=i->next)
        {   
            if(i->data.id < minid->data.id){
                minid=i;

            }

        }
        
    }
    return minid;
 //6
}
 
/**
 * Najde polozku seznamu s odpovidajicim jmenem objektu. Vraci NULL, pokud
 * takova polozka v seznamu neni.
 */
Item *list_find_name(List *list, char *name)
{
 //5
    for(Item *i=list->first;i!=NULL;i=i->next)
    {
        if(strcmp(i->data.name,name)==0)
        {
        return i;
        }
    }
    return NULL;
}
 
/**
 * Uvolneni seznamu.
 */
void list_dtor(List *list)
{
 //2
 for(Item *i=list->first;i!=NULL;i=i->next)
    list_delete_first(list);
 
 
}
//vypis zoznamu; [idex];ID=[id];name=[name];
void print_list(List *list)
{
    if(list_empty(list))
        printf("seznam je prazdny");
    else{
        unsigned int index= 1;
    
        for(Item *i=list->first;i!=NULL;i=i->next)
        {
            
            printf("%d:ID= %d, name= %s\n",index,i->data.id,i->data.name);
            index++;
        } 
    }
} 
int main()
{
    printf("list_ctor...\n");
    List list = list_ctor();
 
    printf("list_empty...\n");
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
 
    Item *item;
 
    Object o1 = {42, "Honza"};
    printf("item_ctor...\n");
    item = item_ctor(o1);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
    print_list(&list);
     printf("list_count...\n");
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
 
    Object o2 = {2, "Malem"};
    item = item_ctor(o2);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    Object o3 = {0, "Kralem"};
    item = item_ctor(o3);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
    print_list(&list);
 
    printf("Odstraneni prvniho prvku ze seznamu \n");
    list_delete_first(&list);
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
    print_list(&list);
 
    // opetovne vlozeni objektu o1		
    item = item_ctor(o1);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
    print_list(&list);

    printf("list_find_minid...\n");
    item = list_find_minid(&list);
    if (item != NULL) {
        printf("Polozka s nejmensim identifikatorem: {%d, \"%s\"}\n",
            item->data.id, item->data.name);
	}    
	else
            printf("Polozka s nejmensim identifikatorem nenalezena\n");
 
    printf("list_find_name...\n");
    char* name = "Honza";
    item = list_find_name(&list, name);
    if (item != NULL) {
             printf("Polozka s daty %s nalezena\n", name);
	}    
	else
             printf("Polozka s daty %s nenalezena.\n",name);
 
    printf("list_dtor...\n");
    list_dtor(&list);
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
 
    return 0;
}