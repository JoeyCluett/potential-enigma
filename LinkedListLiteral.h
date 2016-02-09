/*
    LinkedListLiteral, linked list library
    Copyright (C) 2016  Joseph Cluett

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author(s):
        Joseph Cluett (main author)

    File Type: header, LinkedListLiteral

    Date Created: 2/1/2016

    Date Last Modified: 2/9/2016

    Purpose:
        The second gen linked list library. More secure with the use of the JJC namespace
        and the ability to hold data other than pointers.

    Misc. Notes:
        This library will hold any data type that has a defined constructor, this includes all default
        primitive data types.
*/

#ifndef LINKEDLISTLITERAL_H
#define LINKEDLISTLITERAL_H

#ifndef LL
#define LL LinkedList
#endif // LL

#ifndef ll
#define ll LinkedList
#endif // ll

#include <vector>
#include <iostream>

/* Made namespace JJC to avoid contention with LinkedList because most of the functions/methods
    defined here have the same names but slightly different implementation */
namespace JJC {

template<class T>
class LinkedList {
    public:
        LinkedList(void);          //default constructor
        LinkedList(int init_size); //constructor to initialize LLL to specific size

        int addToEnd  (T data); //adds data to end of linked list
        int addToBegin(T data); //adds data to beginning of linked list

        void removeEnd(void);   //de-allocates memory associated with last element
        void removeBegin(void); //--                                  first element

        int getSize(void);

        /* Two callback functions, one accepts a T pointer and one accepts
            T data. Neither are allowed to return any value */
        void (*callback_ptr)(T*);
        void (*callback_lit)(T);
        void makeCallbackPtr(void); //calls callback_ptr on every element in linked list
        void makeCallbackLit(void); //calls callback_lit --

        T at(int index);                   //for reading data from linked list, instead of overloading index operator
        std::vector<T> * getDataVec(void);    //returns a pointer to a vector of T data
        std::vector<T*>* getPointerVec(void);//--                               T data pointers
        /* For getPointerVec() vector, can't use 'using namespace std'. Not sure why yet. */

        /* Methods that affect only user_node, they do NOT give the user the ability to modify the linked list
            through the pointer, but do allow the user to retrieve values through it */
        int moveBack(void);           //returns 1 if move is not allowed due to size constraints of linked list
        int moveBack(int mv_dist);    //--
        int moveForward(void);        //--
        int moveForward(int mv_dist); //--
        T getData(void); //for retrieving data in user_node
        T* getPtr(void); //for retrieving pointer to data in node pointed to by user_node

    private:
        /* Vars get set when callback_ptr and callback_lit get defined by user */
        bool callback_ptr_set;
        bool callback_lit_set;

        int ll_size = 0;

        struct node {
            node* prev;
            node* next;
            T datum;

            //assuming that T has a copy constructor; all primitive data types have one
            node(T _datum) {
                datum = _datum;
            }

        }; //end of node

        node* first_node; //location of first node in linked list
        node* last_node;  //--          last node in linked list
        node* user_node;  //a node pointer that allows the user access to the linked list
};

}

// end of LLL declaration
//==============================================================================================
// start of LLL definitions:

template<class T>
JJC::LL<T>::LL(void) {
    callback_lit_set = false;
    callback_ptr_set = false;
    first_node = NULL;
    last_node  = NULL;
    ll_size = 0;
}

template<class T>
JJC::LL<T>::LL(int init_size) {
    if(init_size == 0) {
        callback_lit_set = false;
        callback_ptr_set = false;
        first_node = NULL;
        last_node = NULL;
        ll_size = 0;
        return;
    } else if(init_size == 1) {
        addToEnd((T)0);
    } else if(init_size > 1) {
        //std::cout << "debug::in ll(init_size)" << std::cout;
        for(int i = 0; i < init_size; i++) {
            addToEnd((T)0); //attempts to initialize data to zero
        }
    }
    return;
}

template<class T>
int JJC::LL<T>::addToEnd(T data) {
    if(ll_size == 0) {
        first_node = new node(data);
        last_node = first_node;
        first_node->prev = NULL;
        first_node->next = NULL;
        ll_size = 1;
        return 0;
    } else if(ll_size > 0) {
        last_node->next = new node(data); //add node after last_node
        node* temp_ptr = last_node;
        last_node = last_node->next; //last_node now points to new last node
        last_node->prev = temp_ptr;
        last_node->next = NULL;
        ll_size++;
        return 0;
    } else {
        std::cout << "non-fatal error in addToEnd: out of bounds" << std::cout;
        return 1;
    }
}

template<class T>
int JJC::LL<T>::addToBegin(T data) {
    if(ll_size == 0) {
        addToEnd(data);
    } else if(ll_size > 0) {
        first_node->prev = new node(data); //add node before first_node
        node* temp_ptr = first_node;
        first_node = first_node->prev; //first_node now points to new first node
        first_node->next = temp_ptr;
        first_node->prev = NULL;
        ll_size++;
        return 0;
    } else {
        std::cout << "non-fatal error in addToEnd: out of bounds" << std::cout;
    }
    return 1;
}

template<class T>
void JJC::LL<T>::removeEnd(void) {
    if(ll_size == 0) {

    }
}

template<class T>
void JJC::LL<T>::removeBegin(void) {
    ;
}

template<class T>
int JJC::LL<T>::getSize(void) {
    return ll_size;
}

#endif // LINKEDLISTLITERAL_H
