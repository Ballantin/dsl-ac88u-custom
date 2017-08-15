/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: linkedList.c
*    Creation Date: 17 September 2001 (v0.00)
*    VSS Info:
*        $Revision: 2 $
*        $Date: 1/13/03 11:15a $
*
****************************************************************************
*    Description:
*
*     Implementation for a linked list.  The implementation is not thread-safe,
*     it is the client's responsibility to provide any necessary critical
*     section protection.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <linkedList.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


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
void linkedListCreate( LINKED_LIST *list )
{
   XCHG_ASSERT( list != NULL );

   linkedListClear( list );
}


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
void linkedListDestroy( LINKED_LIST *list )
{
   XCHG_ASSERT( list != NULL );

   linkedListClear( list );
}


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
unsigned int linkedListGetSize( const LINKED_LIST *list )
{
   return ( list->numElements );
}


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
unsigned int linkedListIsEmpty( const LINKED_LIST *list )
{
   return ( linkedListGetSize( list ) == 0 );
}


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
void linkedListClear( LINKED_LIST *list )
{
   list->head           = NULL;
   list->tail           = NULL;
   list->numElements    = 0;
}


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
void linkedListAddToBack( LINKED_LIST *list, LINKED_LIST_ELEMENT *element )
{
   XCHG_ASSERT( ( list != NULL ) && ( element != NULL ) );

   /* Add to the tail */
   if ( list->tail != NULL )
   {
      list->tail->next = element;
   }
   else
   {
      list->head = element;
   }

   list->tail     = element;
   element->next  = NULL;

   list->numElements++;

}


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
void linkedListAddToFront( LINKED_LIST *list, LINKED_LIST_ELEMENT *element )
{
   XCHG_ASSERT( ( list != NULL ) && ( element != NULL ) );

   /* Add to the head */
   if ( list->head == NULL )
   {
      list->tail = element;
   }

   element->next  = list->head;
   list->head     = element;

   list->numElements++;
}


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
LINKED_LIST_ELEMENT* linkedListPeekBack( const LINKED_LIST *list )
{
   return( list->tail );
}


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
LINKED_LIST_ELEMENT* linkedListPeekFront( const LINKED_LIST *list )
{
   return ( list->head );
}


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
LINKED_LIST_ELEMENT *linkedListRemoveFromFront( LINKED_LIST *list )
{
   LINKED_LIST_ELEMENT *element;

   XCHG_ASSERT( list != NULL );

   element = list->head;
   if ( element != NULL )
   {
      list->head = element->next;

      if ( element == list->tail )
      {
         list->tail = list->head;
      }

      XCHG_ASSERT( list->numElements > 0 );
      list->numElements--;
   }

   return ( element );
}


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
LINKED_LIST_ELEMENT* linkedListRemoveFromBack( LINKED_LIST *list )
{
   LINKED_LIST_ELEMENT *element;
   LINKED_LIST_ELEMENT *prevElement;

   element     = list->head;
   prevElement = NULL;

   if ( list->head != NULL )
   {
      while ( element != list->tail )
      {
         prevElement = element;
         element     = element->next;
      }

      list->tail = prevElement;

      if ( prevElement != NULL )
      {
         prevElement->next = NULL;
      }
      else
      {
         list->head = NULL;
      }

      XCHG_ASSERT( list->numElements > 0 );
      list->numElements--;
   }

   return ( element );
}


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
LINKED_LIST_ELEMENT *linkedListRemoveElement( LINKED_LIST *list, const void *data )
{
   LINKED_LIST_ELEMENT *element;
   LINKED_LIST_ELEMENT *prevElement;

   XCHG_ASSERT( ( list != NULL ) && ( data != NULL ) );

   element = list->head;
   prevElement = NULL;
   while ( element != NULL )
   {
      if ( element->data == data )
      {
         if ( prevElement != NULL )
         {
            prevElement->next = element->next;
         }
         else
         {
            list->head = element->next;
         }

         if ( element == list->tail )
         {
            list->tail = prevElement;
         }

         XCHG_ASSERT( list->numElements > 0 );
         list->numElements--;
         return ( element );
      }

      prevElement = element;
      element = element->next;
   }

   return ( NULL );
}


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
)
{
   XCHG_ASSERT( ( list != NULL ) && ( newElement != NULL ) );

   if ( after == NULL )
   {
      /*
      ** Just enqueue the element if no 'after' element was specified
      */

     newElement->next = list->head;
     list->head = newElement;

     if ( list->tail == NULL )
     {
        list->tail = newElement;
     }
   }
   else
   {
      newElement->next = after->next;

      if ( newElement->next == NULL )
      {
         XCHG_ASSERT( list->tail == after );

         list->tail = newElement;
      }

      after->next = newElement;
   }

   list->numElements++;
}
