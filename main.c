// ============================================================================
// File: sortedll.c (Fall 2019)
// ============================================================================
// This program allows the user to manipulate the contents of a list of
// integers in a singly-linked list. The list items are maintained in sorted
// ascending order, and duplicate values are permitted.
// ============================================================================

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <ctype.h>
#include    "sortedll.h"


// ==== main ==================================================================
//
// ============================================================================

int     main(void)
{
    auto    int             intVal;
    auto    LNode           *headPtr = NULL;
    auto    char            buf[BUFLEN];
    auto    int             numItems;

        do  {
        // ask the user what they want to do and handle their choice
        DisplayMenu();
        printf("Please enter a selection: ");
        fgets(buf, BUFLEN, stdin);
        *buf = toupper(*buf);
        switch (*buf)
            {
            case   'A':
                printf("Please enter an integer value to add: ");
                fgets(buf, BUFLEN, stdin);
                if (1 != sscanf(buf, "%d", &intVal))
                    {
                    puts("Error reading the integer value...");
                    }
                else
                    {
                    headPtr = AddItem(headPtr, intVal);
                    }
                break;

            case   'R':
                printf("Please enter an integer value to remove: ");
                fgets(buf, BUFLEN, stdin);
                if (1 != sscanf(buf, "%d", &intVal))
                    {
                    puts("Error reading the integer value...");
                    }
                else
                    {
                    headPtr = RemoveItem(headPtr, intVal);
                    }
                break;

            case   'D':
                // display the current contents of the list
                numItems = DisplayList(headPtr);
                printf("*** main -- currently there %s %d node%s in the list...\n"
                                    , (numItems != 1) ? "are" : "is"
                                    , numItems
                                    , (numItems != 1) ? "s" : "");
                break;

            case   'F':
            case   'Q':
                // user wants to either clear the list or quit the program, so
                // release all nodes in the list
                numItems = FreeList(&headPtr);
                printf("*** main -- there %s %d node%s released...\n"
                                    , (numItems != 1) ? "were" : "was"
                                    , numItems
                                    , (numItems != 1) ? "s" : "");
                break;

            default:
                puts("Unrecognized option; please try again.");
                break;
            }

        // write out a blank line before next loop iteration
        puts("");

        } while ('Q' != *buf);

    puts("Thanks for playing!  Bye!!\n");
    return  0;

}  // end of "main"
// ==== AddItem ===============================================================
//
// This function inserts an item into the list by dynamically allocating a node
// to store the value and locating the correct position for the new item so
// that the list is maintained in ascending sorted order.
//
// Input:
//      headPtr [IN]        -- a pointer to the first node in the list
//
//      newItem [IN]        -- the new item to insert into the list
//
// Output:
//      A pointer to the head of the list.

// ============================================================================

LNode*  AddItem(LNode  *headPtr, int  newItem)
{
        // newNode creation
        // #1 dynamic memory allocation
        LNode* newNode = malloc(sizeof(LNode));
        // #2 set the value
        newNode -> value = newItem;
        // #3 set the next ptr to the NULL
        newNode -> next = NULL;

        // local variables
        LNode* leadPtr = headPtr;
        LNode* trailPtr;

        // case#1: empty node
        if (NULL == headPtr){
                headPtr = newNode;
                printf("*** %s -- %d added to the list...\n" , __func__, newItem);
                return headPtr;
        }
        // case#2: Insert in front
        if (leadPtr -> value > newItem){
                newNode -> next = leadPtr;
                headPtr = newNode;
                printf("*** %s -- %d added to the list...\n" , __func__, newItem);
                return headPtr;
        }
        // case#2: Insert in the middle
        while (NULL != leadPtr -> next){
                if (newNode -> value < leadPtr -> next -> value){
                        trailPtr = leadPtr;
                        leadPtr = leadPtr -> next;
                        newNode -> next = leadPtr;
                        trailPtr -> next = newNode;
                        printf("*** %s -- %d added to the list...\n" , __func__, newItem);
                        return headPtr;
                }
                leadPtr = leadPtr -> next;
        }
        // leadPtr -> next == NULL (leadPtr points the last node)

    // case#3: Insert in the end
    leadPtr -> next = newNode;
    printf("*** %s -- %d added to the list...\n" , __func__, newItem);
    return headPtr;

}  // end of "AddItem"

// ==== DisplayList ===========================================================
//
// This function displays all of the values contained in the linked list to the
// standard output stream.
//
// Input:
//      headPtr [IN]        -- a pointer to the first node in the list
//
// Output:
//      The total number of nodes displayed is returned.
//
// ============================================================================
int     DisplayList(const LNode  *nodePtr)
{
    auto    int         counter = 0;

    // if the list is empty, indicate as such and return zero
    if (NULL == nodePtr)
        {
        // puts("*** DisplayList -- the list is currently empty...");
        return 0;
        }

    while (NULL != nodePtr)
        {
        printf("%d\n", nodePtr->value);
        nodePtr = nodePtr->next;
        counter++;
        }

    return counter;

}  // end of "DisplayList"

// ==== DisplayMenu ===========================================================
//
// This function displays the menu to stdout.
//
// Input:
//      Nothing
//
// Output:
//      Nothing
//
// ============================================================================

void    DisplayMenu(void)
{
    // write out a leading blank line, followed by menu items
    puts("A)dd a value");
    puts("R)emove a value");
    puts("D)isplay the list");
    puts("F)ree the list");
    puts("Q)uit");

}  // end of "DisplayMenu"

// ==== FreeList ==============================================================
//
// This function traverses the linked list and releases the memory allocated
// for each individual node, and the caller's head node pointer is set to NULL.
//
// Input:
//      headPtr [IN/OUT]    -- the address of the caller's head node pointer
//
// Output:
//      The total number of nodes released is returned.
//
// ============================================================================
int     FreeList(LNode  **headPtr)
{
        // local variables
    // LNode* leadPtr = *headPtr;
    LNode* trailPtr;
    int counter = 0;

        // case#1: empty linked list
    if (NULL == *headPtr){
        return counter;
    }
    // case#2: non-empty linked list
    while (NULL != *headPtr){
        trailPtr = *headPtr;
        (*headPtr) = (*headPtr) -> next;
        free(trailPtr);
        ++counter;
    }

    return counter;

}  // end of "FreeList"

// ==== RemoveItem ============================================================
//
// This function removes an item from the list.  If the target item is located
// in the list, the memory allocated for its node is released and any adjacent
// nodes are joined so as not to break the list.
//
// Input:
//      headPtr [IN]        -- a pointer to the first node in the list
//
//      target [IN]         -- the item to remove
//
// Output:
//      A pointer to the (potentially new) head of the list
//
// ============================================================================
LNode*  RemoveItem(LNode  *headPtr, int  target)
{
    LNode* leadPtr = headPtr;
    LNode* trailPtr;

    if (NULL == leadPtr){
        return leadPtr;
    }
        //case #1 remove the target from the fist node
    if(target == leadPtr -> value){
        trailPtr = leadPtr -> next;
        free(leadPtr);
        headPtr = trailPtr;
        printf("*** %s -- %d removed from the list...\n" , __func__, target);
        return headPtr;
    }
    //case #2 remove the target in the middle
        while (NULL != leadPtr -> next){
                if (target == leadPtr -> next -> value){
                        trailPtr = leadPtr -> next;
                        leadPtr -> next = trailPtr -> next;
                        free(trailPtr);
                        printf("*** %s -- %d removed from the list...\n" , __func__, target);
                        return headPtr;
                }
                trailPtr = leadPtr;
                leadPtr = leadPtr -> next;
        }
        //case #3 remove the target in the end
        if (target == leadPtr -> value){
                trailPtr -> next = NULL;
                printf("*** %s -- %d removed from the list...\n" , __func__, target);
                free(leadPtr);
        }
        //case #4 the function was not able to locate the target in the list
        printf("*** %s -- %d was not found in the list...\n" , __func__, target);
        return headPtr;


}  // end of "RemoveItem"

