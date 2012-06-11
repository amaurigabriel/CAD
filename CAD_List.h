/*
 * Implementation of a doubly-linked list, like the STL list for c++. 
 * Here we have the definition of structs and functions for manipuling the lists.
 */

#ifndef CAD_list_h
#define CAD_list_h

#include<stdlib.h>
#include<stdio.h>

// include for basic definitions. should be included on every CAD files.
#include "CAD.h"

/*
 * An node contains some data, and points to the members after and before it, if they exists.
 */
struct TCAD_Node{
    void* data;
    size_t size;
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
    return 1;
}

/*
 * Insert an node before the first node of the list. Return 0 if the node allocation fails.
 */
CAD_Node * CAD_ListPushFront(CAD_List* list, void* data){
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
 */
void CAD_ListDropNode(CAD_List* list, CAD_Node* node){
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
        free(node);
    }
}

/*
 * Removes from the list the node that contains the data passed to the function, if exists
 */
void CAD_ListRemoveNode(CAD_List* list, void* data, int compare(const void*a, const void*b)){
    CAD_Node* node = CAD_ListFind(list, data, compare);
    CAD_ListDropNode(list, node);
}

/*
 * Remove from the list its first node
 */
void CAD_ListPopFront(CAD_List* list){
    CAD_ListDropNode(list, list->first_node);
}

/*
 * Remove from the list its last node
 */
void CAD_ListPopBack(CAD_List* list){
    CAD_ListDropNode(list, list->last_node);
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

#endif
