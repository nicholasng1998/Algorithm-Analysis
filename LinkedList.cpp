// LinkedList.cpp -- class for a linked list as a data structure

#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP
#include <vector>
#include <iostream>
using namespace std;

template <typename T>
struct Node {
  T info;
  Node<T> *next;
};

template <typename T>
class LinkedList {
  Node<T> *start;
 public:
  LinkedList() { start = NULL; }
  ~LinkedList() { makeEmpty(); }
  // inserts at the beginning of the linked list
  void insertFront (T & element ) {
    Node<T> *newNode = new Node<T>;
    newNode->info = element;
    newNode->next = start;
    start = newNode;
  }
  // returns true if element is found; returns false if element is not found
  string find (string & target) {
    //bool found = false;
    Node<T> *ptr = start;
    while (ptr != NULL) {
      if (ptr->info.at(1) == target) {
        //found = true;
        return ptr->info.at(0);
      }  
      else
        ptr = ptr->next;
    }
    return "notfound";
  }
  bool isEmpty() { return start == NULL; }
  void makeEmpty() {
    while (start != NULL) {
      Node<T> *ptr  = start;
      start = start->next;
      delete ptr;
	}
  }
  friend ostream& operator<< (ostream& os, LinkedList<T>& list) {
    Node<T> *ptr = list.start;
    while (ptr != NULL) {
      os << ptr->info << " ";
      ptr = ptr->next;
    }
    return os;
  }
};
#endif
