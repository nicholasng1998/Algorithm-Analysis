// HashTable.cpp
#include <vector>
#include "LinkedList.cpp" // available at MMLS.


template <typename T>
class HashTable {
  vector< LinkedList<T> > table;
  int hashfunction (int hashitem) { // hash function
    return hashitem % table.size();
  }
 public:
  HashTable (int size) {
    table.resize (size); // resize vector to support size elements.
  }
  ~HashTable() {
    for (int i = 0; i < table.size(); i++)
      table[i].makeEmpty();
  }
  /*Implementation for prime number hash table*/
  int setSize(int size){
    table.resize(size);
  }

  int size() {
    return table.size();
  }
  void insert (T data, int newItemLength) { 
    int indexP = hashfunction(newItemLength);
    table[indexP].insertFront(data);
  }

  string retrieve (string & target) {
    int location = hashfunction(target.length());
    return table[location].find(target);
  }

  friend ostream& operator<< (ostream& os, HashTable<T>& ht) {
    for (int i = 0; i < ht.size(); i++)
      os << i << " = " << ht.table[i] << endl;
    return os;
  }
};
