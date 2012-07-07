/*
 * Implementation of a doubly-linked list, like the STL list for c++. 
 * Here we have the definition of structs and functions for manipuling the lists.
 */

#ifndef CAD_list_h
#define CAD_list_h

#include<stdlib.h>
#include<stdio.h>

/*
 * An node contains some data, and points to the members after and before it, if they exists.
 */
struct TCAD_Node{
    void* data;
    struct TCAD_Node* next_node;
    struct TCAD_Node* prior_node;
};

typedef struct TCAD_Node CAD_Node;

/*
 * An list is, basically, a pointer to the first and the last node, that can be changed dynamically.
 */
typedef struct TCAD_List{
    CAD_Node* first_node;
    CAD_Node* last_node;
    int qtt_nodes;
}CAD_List;

/*
 * Initialize the list, clearing its first and last nodes.
 */

CAD_List* CAD_ListInitialize(){
    CAD_List* list = (CAD_List*)malloc(sizeof(CAD_List));

    if (list != NULL){
        list->first_node = NULL;
        list->last_node = NULL;
    }

    list->qtt_nodes = 0;

    return list;
} 

/*
 * returns true if list doesn't have any node.
 */
byte CAD_ListIsEmpty(CAD_List* list){
    return list->first_node == NULL;
}

/*
 * Returns the node after the one passed to the function, if exists
 */
CAD_Node* CAD_ListNextNode(CAD_Node* node){
    if (node == NULL) return NULL;

    return node->next_node;
}

/*
 * Returns the node before the one passed to the function, if exists
 */
CAD_Node* CAD_ListPriorNode(CAD_Node* node){
    if (node == NULL) return NULL; 

    return node->prior_node;
}

/*
 * Insert an node after the last node of the list. Return 0 if the node allocation fails.
 */
byte CAD_ListPushBack(CAD_List* list, void* data){
    CAD_Node* new_node = (CAD_Node*)malloc(sizeof(CAD_Node));
    if (new_node == NULL) return 0;

    new_node->data = data;
    new_node->next_node = NULL;

    if (CAD_ListIsEmpty(list)){
        list->first_node = new_node;
        new_node->prior_node = NULL;
    }
    else{
        CAD_Node* last_node = list->last_node;
        last_node->next_node = new_node;
        new_node->prior_node = last_node;
    }

    list->last_node = new_node;
    list->qtt_nodes++;

    return 1;
}

/*
 * Insert an node before the first node of the list. Return 0 if the node allocation fails.
 */
byte CAD_ListPushFront(CAD_List* list, void* data){
    CAD_Node* new_node = (CAD_Node*)malloc(sizeof(CAD_Node));
    if (new_node == NULL) return 0;

    new_node->data = data;
    new_node->prior_node = NULL;

    if (CAD_ListIsEmpty(list)){
        list->last_node = new_node;
        new_node->next_node = NULL;
    }
    else{
        new_node->next_node = list->first_node;
        list->first_node->prior_node = new_node;
    }
    list->first_node = new_node;
    list->qtt_nodes++;

    return 1;
}

/*
 * Return the node from the list that contains the data passed to the function, if exists. 
 * The function compare should return true if both the nodes are considered equals.
 */
CAD_Node* CAD_ListFind(CAD_List* list, void* data, int compare(const void*a, const void*b)){
    CAD_Node* it = list->first_node;

    while(it != NULL && !compare(it->data, data)){
        it = it->next_node;
    }

    return it;
}

/*
 * Remove from the list a node from the list
 * if flag = FREE_DATA, this functions also free memory from the "data" field of 
 *          the node.
 */
void CAD_ListDropNode(CAD_List* list, CAD_Node* node, byte flag){
    if (node != NULL){
        if (node->prior_node != NULL){
            node->prior_node->next_node = node->next_node;
        }

        if  (node->next_node != NULL){
            node->next_node->prior_node = node->prior_node;
        }

        if (node == list->last_node){
            list->last_node = node->prior_node;
        }

        if (node == list->first_node){
            list->first_node = node->next_node;
        }

        if (flag == FREE_DATA) free(node->data);
        free(node);
        list->qtt_nodes--;
    }
}

/*
 * Removes from the list the node that contains the data passed to the function, if exists
 * if flag = FREE_DATA, this function also free memory from the "data" field of 
 *          the node.
 * */

void CAD_ListRemoveNode(CAD_List* list, void* data, int compare(const void*a, const void*b), byte flag){
    CAD_Node* node = CAD_ListFind(list, data, compare);
    CAD_ListDropNode(list, node, flag);
}

/*
 * Remove from the list its first node
 * if flag = FREE_DATA, this function also free memory from the "data" field of 
 *          the node.
 */

void CAD_ListPopFront(CAD_List* list, byte flag){
    CAD_ListDropNode(list, list->first_node, flag);
}

/*
 * Remove from the list its last node
 * if flag = FREE_DATA, this function also free memory from the "data" field of 
 *          the node.
 */

void CAD_ListPopBack(CAD_List* list, byte flag){
    CAD_ListDropNode(list, list->last_node, flag);
}

/*
 * Returns the first node from the list
 */
CAD_Node* CAD_ListBegin(CAD_List* list){
    return list->first_node;
}

/*
 * Return the last node from the list
 */
CAD_Node* CAD_ListEnd(CAD_List* list){
    return list->last_node;
}

/*
 * Return how many nodes are on the list
 */

int CAD_ListSize(CAD_List* list){
    return list->qtt_nodes;
}

/*
 * Remove all nodes from de list, freeing its memory
 * if flag = FREE_DATA, this function also free memory from the "data" field of 
 *           the nodes.
 */

void CAD_ListClear(CAD_List* list, byte flag){
    while(list->first_node != NULL){
        CAD_ListDropNode(list, list->first_node, flag);
    }
}

/*
 * Create a list from an array.
 * The 0-index element of the array will be the first node, 
 * and the n-1 index will be the last one.
 * Returns NULL on failure, 1 on success.
 * On failure, no memory will remains allocated.
 */

CAD_List* CAD_ListCreateFromArray(void* array[], int N){
    CAD_List* list = CAD_ListInitialize();
    if (list == NULL){
        return NULL;
    }

    int i;
    for (i=0;i<N;i++){
        if (!(CAD_ListPushBack(list, array[i]))){
            CAD_ListClear(list, FREE_DATA);
            free(list);
            return NULL;
        }
    }

    return list;
}

/*
 * Returns a pointer for the node that is N nodes closer to the last
 * node, if exists, or NULL.
 */

CAD_Node* CAD_ListForward(CAD_Node* node, int N){
    CAD_Node* next = node;
    int i = 0;
    while(next != NULL && i != N){
        next = CAD_ListNextNode(next);
        i++;
    }

    return next;
} 

/*
 * Returns a pointer for the node that is N nodes closer to the first
 * * node, if exists, or NULL.
 */

CAD_Node* CAD_ListRewind(CAD_Node* node, int N){
    CAD_Node* next = node;
    int i = 0;
    while(next != NULL && i != N){
        next = CAD_ListPriorNode(next);
        i++;
    }
    return next;
}

/*
 * Apply a function for each node on a closed interval. 
 * stops if function returns false;
 *
 * return: 
 *      0 if function returned 0 on some node;
 *      1 otherwise;
 */

int CAD_ListForeachInterval(CAD_Node* first, CAD_Node* last, int function(void*a)){
    CAD_Node* node_1 = first;
    CAD_Node* node_2 = last->next_node;
    do{
        if (!function(node_1->data)) return 0;
        node_1 = node_1->next_node;
    }while(node_1 != node_2);

    return 1;
}

/*
 * Apply a function for each node on the list.
 * stops if function returns false
 */

int CAD_ListForeach(CAD_List* list, int function(void*a)){
    return CAD_ListForeachInterval(list->first_node, list->last_node, function);
}

#endif
