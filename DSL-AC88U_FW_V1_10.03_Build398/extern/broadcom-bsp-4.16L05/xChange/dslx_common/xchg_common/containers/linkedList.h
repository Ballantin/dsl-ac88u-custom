/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: linkedList.h
*    Creation Date: 17 September 2001 (v0.00)
*    VSS Info:
*        $Revision: 2 $
*        $Date: 1/13/03 11:15a $
*
****************************************************************************
*    Description:
*
*     Interface for a linked list.  The implementation is not thread-safe,
*     it is the client's responsibility to provide any necessary critical
*     section protection.
*
****************************************************************************/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

typedef struct LINKED_LIST_ELEMENT
{
   void                         *data;
   struct LINKED_LIST_ELEMENT   *next;

} LINKED_LIST_ELEMENT;


typedef struct LINKED_LIST
{
   LINKED_LIST_ELEMENT    *head;
   LINKED_LIST_ELEMENT    *tail;
   unsigned int            numElements;

} LINKED_LIST;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   linkedListCreate
**
** PURPOSE:    To create a linked-list container object
**
** PARAMETERS: list  (out) List object to initialize
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListCreate( LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListDestroy
**
** PURPOSE:    To destroy a previously create linked-list
**
** PARAMETERS: list  (mod) List to de-allocate
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListDestroy( LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListGetSize
**
** PURPOSE:    Return current size of list.
**
** PARAMETERS: list  (in)  Linked list.
**
** RETURNS:    Size of list.
**
** NOTE:
*****************************************************************************
*/
unsigned int linkedListGetSize( const LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListIsEmpty
**
** PURPOSE:    Determine is list is empty.
**
** PARAMETERS: list  (in)  Linked list.
**
** RETURNS:    1 if empty, 0 if no empty.
**
** NOTE:
*****************************************************************************
*/
unsigned int linkedListIsEmpty( const LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   ListReset
**
** PURPOSE:    Reset the state of a linked-link object
**
** PARAMETERS: list  (mod) List to reset
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListClear( LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListAddToBack
**
** PURPOSE:    Add a list element to the back of the specified list.
**
** PARAMETERS: list     (mod) List to add element to
**             element  (mod) List element to add
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListAddToBack( LINKED_LIST *list, LINKED_LIST_ELEMENT *element );


/*
*****************************************************************************
** FUNCTION:   linkedListAddToFront
**
** PURPOSE:    Add a list element to the front of the specified list.
**
** PARAMETERS: list     (mod) List to add element to
**             element  (mod) List element to add
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListAddToFront( LINKED_LIST *list, LINKED_LIST_ELEMENT *element );


/*
*****************************************************************************
** FUNCTION:   linkedListPeekBack
**
** PURPOSE:    Get (but not remove) list element at the back of the specified list.
**
** PARAMETERS: list     (in) Linked list.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
LINKED_LIST_ELEMENT* linkedListPeekBack( const LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListPeekBack
**
** PURPOSE:    Get (but not remove) list element at the front of the specified list.
**
** PARAMETERS: list     (in) Linked list.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
LINKED_LIST_ELEMENT* linkedListPeekFront( const LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListRemoveFromFront
**
** PURPOSE:    Remove a list element from the front of the specified list.
**
** PARAMETERS: list  (mod) List to remove from
**
** RETURNS:    List element that was removed from the list. NULL if there are
**             no elements to remove.
**
** NOTE:
*****************************************************************************
*/
LINKED_LIST_ELEMENT* linkedListRemoveFromFront( LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListRemoveFromBack
**
** PURPOSE:    Remove a list element from the back of the specified list.
**
** PARAMETERS: list  (mod) List to remove from
**
** RETURNS:    List element that was removed from the list. NULL if there are
**             no elements to remove.
**
** NOTE:
*****************************************************************************
*/
LINKED_LIST_ELEMENT* linkedListRemoveFromBack( LINKED_LIST *list );


/*
*****************************************************************************
** FUNCTION:   linkedListRemoveElement
**
** PURPOSE:    To remove a specific list element from the queue
**
** PARAMETERS: list  (mod) List to remove element from
**             data  (in)  Pointer to list element data to remove
**
** RETURNS:    List element that was removed from the list. NULL if element does
**             not exist in list.
**
**
** NOTE:
*****************************************************************************
*/
LINKED_LIST_ELEMENT* linkedListRemoveElement( LINKED_LIST *list, const void *data );


/*
*****************************************************************************
** FUNCTION:   linkedListInsertAfter
**
** PURPOSE:    Add a list element to the back of the specified list.
**
** PARAMETERS: list        (mod) List to add element to
**             after       (mod) List element to insert after
**             newElement  (mod) List element to insert
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void linkedListInsertAfter
(
   LINKED_LIST         *list,
   LINKED_LIST_ELEMENT *after,
   LINKED_LIST_ELEMENT *newElement
);


#ifdef __cplusplus
    }
#endif

#endif  /* LINKED_LIST_H  */
