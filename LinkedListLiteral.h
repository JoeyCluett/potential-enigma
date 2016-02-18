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

    Date Last Modified: 2/12/2016

    Purpose:
        The second gen linked list library. More secure with the use of the JJC namespace
        and the ability to hold data other than pointers.

    Misc. Notes:
        This library will hold any data type that has a defined constructor, this includes all default
        primitive data types. Using this library allows users to easily create doubly linked lists, thus
        allowing for easy two-way traversal of the linked list.
*/

#ifndef LINKEDLISTLITERAL_H
#define LINKEDLISTLITERAL_H

#ifndef LL
#define LL LinkedList
#endif // LL

#ifndef ll
#define ll LinkedList
#endif // ll

/* determine if code containing user_node should be compiled (resulting in different size executable) */
#ifndef COMPILE_UN_METHODS
#define COMPILE_UN_METHODS 1 //change this to 0 if you do not want user_node methods in your program, it will not affect the rest of the library
#endif // COMPILE_UN_METHODS

#define OUT_OF_BOUNDS "non-fatal error: out of bounds"

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

        /* Methods that pertain to setting/calling callback functions */
        void makeCallbackPtr(void); //calls callback_ptr on every element in linked list
        void makeCallbackLit(void); //calls callback_lit --
        void setCallbackPtr(void(*callback_fnct)(T*));
        void setCallbackLit(void(*callback_fnct)(T));

        //ll.at() treats linked list like a zero-indexed array
        T at(int index);                   //for reading data from linked list, instead of overloading index operator
        std::vector<T >* getDataVec(void);   //returns a pointer to a vector of T data
        std::vector<T*>* getPointerVec(void);//--                               T data pointers
        /* For getPointerVec() vector, can't use 'using namespace std'. Not sure why yet. */

        #if COMPILE_UN_METHODS == 1 //conditional compilation of user_node methods
            /* Methods that affect only user_node, they do NOT give the user the ability to modify the linked list
                through the pointer, but do allow the user to retrieve values through it */
            int  UN_moveBack(void);           //returns 1 if move is not allowed due to size constraints of linked list, returns 0 if move was successful
            int  UN_moveBack(int mv_dist);    //--
            int  UN_moveForward(void);        //--
            int  UN_moveForward(int mv_dist); //--
            int  UN_setToEnd(void);           //--
            int  UN_setToBeg(void);           //--
            T    UN_getData(void); //for retrieving data in user_node
            T*   UN_getPtr(void);  //for retrieving pointer to data in node pointed to by user_node
            bool UN_isSet(void);   //tells calling function if user_node is pointing at part of linked list
        #endif // COMPILE_UN_METHODS

    private:
        /* Two callback functions, one accepts a T pointer and one accepts
            T data. Neither are allowed to return any value */
        void (*callback_ptr)(T*);
        void (*callback_lit)(T);

        /* Vars get set when callback_ptr and callback_lit get defined by user */
        bool callback_ptr_set = false;
        bool callback_lit_set = false;

        int ll_size = 0;
        bool user_node_set = false;

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
        std::cout << OUT_OF_BOUNDS << std::cout;
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
        std::cout << OUT_OF_BOUNDS << std::cout;
    }
    return 1;
}

template<class T>
void JJC::LL<T>::removeEnd(void) {
    if(ll_size == 0) {
        std::cout << OUT_OF_BOUNDS << std::endl;
    } else if(ll_size == 1) { //must ensure that first_node and last_node are made NULL
        delete first_node;
        last_node = NULL;
        ll_size = 0;
        user_node = 0; //reassign user_node as well
    } else if(ll_size > 1) {
        node* temp_ptr = last_node;
        last_node = last_node->prev_node;
        delete temp_ptr;
        last_node->next = NULL;
        ll_size--;
    }
}

template<class T>
void JJC::LL<T>::removeBegin(void) {
    if(ll_size < 2) {
        removeEnd();
    } else if(ll_size >= 2) {
        node* temp_ptr = first_node;
        first_node = first_node->next;
        delete temp_ptr; //delete old first node
        first_node->prev = NULL;
        ll_size--;
    }
}

template<class T>
int JJC::LL<T>::getSize(void) {
    return ll_size;
}

template<class T>
void JJC::LL<T>::makeCallbackPtr(void) {
    if(callback_ptr_set) {
        if(ll_size > 0) {
            node* temp_node = first_node;
            while(temp_node != NULL) { //iterate through linked list
                callback_ptr(&temp_node->datum);
                temp_node = temp_node->next;
            }
        } else {
            std::cout << OUT_OF_BOUNDS << std::endl;
        }
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl;
    }
    return;
}

template<class T>
void JJC::LL<T>::makeCallbackLit(void) {
    if(callback_lit_set) {
        if(ll_size > 0) {
            node* temp_node = first_node;
            while(temp_node != NULL) { //iterate through linked list
                callback_lit(temp_node->datum);
                temp_node = temp_node->next;
            }
        } else {
            std::cout << OUT_OF_BOUNDS << std::endl;
        }
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl;
    }
    return;
}

template<class T>
void JJC::LL<T>::setCallbackPtr(void(*callback_fnct)(T*)) {
    callback_ptr = callback_fnct;
    callback_ptr_set = true;
    return;
}

template<class T>
void JJC::LL<T>::setCallbackLit(void(*callback_fnct)(T)) {
    callback_lit = callback_fnct;
    callback_lit_set = true;
    return;
}

template<class T>
T JJC::LL<T>::at(int index) {
    if(index >= 0 && index < ll_size) { //checking for legitimate range in linked list
        node* temp_node = first_node;
        for(int i = 0; i < index; i++) {
            temp_node = temp_node->next;
        }
        return temp_node->datum;
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl;
    }
}

template<class T>
std::vector<T>* JJC::LL<T>::getDataVec(void) {
    std::vector<T>* this_vec = new std::vector<T>;
    if(ll_size > 0) {
        node* temp_node = first_node;
        while(temp_node != NULL) {
            this_vec->push_back(temp_node->datum);
            temp_node = temp_node->next;
        }
        return this_vec;
    } else {
        return NULL; //if linked list is empty return 0
    }
}

template<class T>
std::vector<T*>* JJC::LL<T>::getPointerVec(void) {
    std::vector<T*>* that_vec = new std::vector<T*>;
    if(ll_size > 0) {
        node* temp_node = first_node;
        while(temp_node != NULL) {
            that_vec->push_back(&temp_node->datum);
            temp_node = temp_node->next;
        }
        return that_vec;
    } else {
        return NULL;
    }
}

    /* Functions that operate only on user_node, using conditional compilation headers to
        optionally not include any user_node methods. These methods do not allow the user
        to modify the linked list through user_node. Please note that only rudimentary
        error checking is performed. Future revsions may allow consecutive moves of
        user_node to wrap around the list. All user_node methods are prefixed with 'UN_'
        to avoid name contention with the standard linked list methods */

#if COMPILE_UN_METHODS == 1

template<class T>
int JJC::LL<T>::UN_moveBack(void) { //moves towards first_node
    if((user_node != first_node) && (user_node != NULL)) {
        user_node = user_node->prev;
        return 1; //successful move
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl;
        return 0; //unsuccessful move
    }
}

template<class T>
int JJC::LL<T>::UN_moveBack(int mv_dist) { //calls moveBack() repeatedly with errorchecking
    ;
}

template<class T>
int JJC::LL<T>::UN_moveForward(void) { //moves towards last_node
    if((user_node != last_node) && (user_node != NULL)) {
        user_node = user_node->next;
        return 1; //successful move
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl;
        return 0; //unsuccessful move
    }
}

template<class T>
int JJC::LL<T>::UN_moveForward(int mv_dist) {
    ;
}

template<class T>
int JJC::LL<T>::UN_setToEnd(void) {
    if(last_node != NULL) {
        user_node = last_node;
        return 1; //success
    } else {
        user_node = NULL;
        return 0; //failure
    }
}

template<class T>
int JJC::LL<T>::UN_setToBeg(void) {
    if(first_node != NULL) {
        user_node = first_node;
        return 1; //success
    } else {
        user_node = NULL;
        return 0; //failure
    }
}

template<class T>
T JJC::LL<T>::UN_getData(void) {
    if(user_node != NULL) {
        return user_node->datum;
    } else {
        std::cout << OUT_OF_BOUNDS << std::endl; //no error checking
    }
}

/* Recommend returning pointer and not literal data, as this allows error checking */
template<class T>
T* JJC::LL<T>::UN_getPtr(void) {
    if(user_node != NULL) {
        return &user_node->datum; //return address of data pointed to by user_node
    } else {
        return NULL; //return null pointer, this is the error check. User tests return value
    }
}

//test to see if user_node is set to valid location in linked list. return true or false depending on state of user_node
template<class T>
bool JJC::LL<T>::UN_isSet(void) {
    if(!user_node_set || ll_size == 0) {
        return false;
    } else if(user_node_set && ll_size > 0) {
        node* temp_node = last_node;
        while(temp_node != NULL) { //traverse over linked list backwards
            if(user_node == temp_node)
                return true;
            temp_node = temp_node->prev;
        }
    } else {
        return false; //for any other condition
    }
}

#endif // COMPILE_UN_METHODS
#endif // LINKEDLISTLITERAL_H

//I have not tested the above code. I have only proved it correct
//I never finish anythi
