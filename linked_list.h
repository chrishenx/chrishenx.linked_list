#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "list_node.h"

#include <stdexcept>
#include <ostream>

namespace chrishenx {

  template <typename T>
  class List {
  public:
    class Iterator;

    // Protects of multiples constructions of Iterator objects pointing to nullptr
    static const Iterator NO_ELEMENT;

    // Constructors

    List() {}
    List(std::initializer_list<T> il);
    List(const T& value, int times);
    List(const List& other) { append(other); }
    List(List&& other) {
      first = other.first;
      last = other.last;
      length = other.length;
      other.first = nullptr;
      other.last = nullptr;
      other.length = 0;
    }

    // Destructor
    ~List() { clear(); }

    // Operators

    List& operator=(const List& other);
    List& operator=(List&& other);

    bool operator==(const List& other) const;
    bool operator!=(const List& other) const;

    void operator<<(const T& value) { append(value) };
    bool operator>>(T& output) {
      if (last) 
        output = last->value;
      return pop();
    }

    // Methods to create new nodes 

    void append(const T& value) {
      ListNode<T>* new_node = new ListNode<T>(value);
      append(new_node);
    }
    void append(T&& value) {
      ListNode<T>* new_node = new ListNode<T>(std::move(value));
      append(new_node);
    }
    void append(const T& value, int times);
    void append(const List& other);
    void append(List&& other);

    void push_first(const T& value) {
      ListNode<T>* new_node = new ListNode<T>(value);
      push_first(new_node);
    }
    void push_first(T&& value) {
      ListNode<T>* new_node = new ListNode<T>(std::move(value));
      push_first(new_node);
    }
    void push_first(const T& value, int times);
    void push_first(const List& other);
    void push_first(List&& other);

    bool insert(int pos, const T& value);
    bool insert(int pos, T&& value);

    // Methods to delete existing nodes

    bool pop();
    bool pop_first();

    bool remove_from_begin(const T& value);
    bool remove_from_end(const T& value);
    bool remove_all(const T& value);
    bool erase(int pos);

    // Utility methods

    void reverse();
    List reversed() const;

    Iterator erase(Iterator it);
    Iterator rerase(Iterator it);

    void clear();

    // Information about the state of the list

    int size() const { return length; }
    bool empty() const { return length == 0; }

    // Access methods

    const T& get(int pos);
    bool set(int pos, const T& value);
    bool set(int pos, T&& value);

    // Iterator acces methods

    Iterator begin() const { return first; }
    const Iterator& end() const { return NO_ELEMENT; }
    Iterator rbegin() const { return last; }
    const Iterator& rend() const { return NO_ELEMENT; }

    Iterator getIterator(const T& value) const;

  private:
    int length = 0;
    ListNode<T>* first = nullptr;
    ListNode<T>* last = nullptr;

    // Appends a previously allocated object at the end
    void append(ListNode<T>* new_node);
    // Appends a previously allocated object at the begining 
    void push_first(ListNode<T>* new_node);
    // Inserts a previously allocated object at the specified position
    bool insert(int pos, ListNode<T>* new_node);

    // Returns the node specified by the pos argument
    ListNode<T>* getNode(int pos);
  };

  // List's constructors implementation

  template <typename T>
  List<T>::List(const T& value, int times) {
    for (int i = 0; i < length; i++) {
      append(value);
    }
  }

  template <typename T>
  List<T>::List(std::initializer_list<T> il) {
    for (const T& value : il) {
      append(value);
    }
  }

  // List's operators implementation

  template <typename T>
  List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) {
      clear();
      append(other);
    }
    return *this;
  }

  template <typename T>
  inline List<T>& List<T>::operator=(List<T>&& other) {
    if (this != &other) {
      clear();
      first = other.first;
      last = other.last;
      length = other.length;
      other.first = nullptr;
      other.last = nullptr;
      other.length = 0;
    }
    return *this;
  }

  template <typename T>
  bool List<T>::operator==(const List& other) const {
    if (this != &other) {
      if (length == other.length) {
        Iterator it1 = first;
        Iterator it2 = other.first;
        while (it1 != NO_ELEMENT) {
          if (*it1 != *it2) {
            return false;
          }
          it1++;
          it2++;
        }
        return true;
      }
      return false; 
    }
    return true; // The same object
  }

  template <typename T>
  bool List<T>::operator!=(const List& other) const {
    return !(*this == other);
  }

  // List's function members to create new nodes

  template <typename T>
  void List<T>::append(ListNode<T>* new_node) {
    if (length == 0) {
      first = new_node;
      last = new_node;
    }
    else {
      new_node->next = nullptr;
      new_node->previous = last;
      last->next = new_node;
      last = new_node;
    }
    length++;
  }

  template <typename T>
  void List<T>::append(const T& value, int times) {
    for (int i = 0; i < times; i++) {
      append(value);
    }
  }

  template <typename T>
  void List<T>::append(const List<T>& other) {
    if (this != &other) {
      for (ListNode<T>* node = other.first; node != nullptr; node = node->next) {
        append(node->value);
      }
    } else {
      throw std::invalid_argument("You cannot append a list object to itself");
    }
  }

  template <typename T>
  inline void List<T>::append(List<T>&& other) {
    if (this != &other) {
      if (length == 0) {
        *this = std::move(other);
      } else {
        last->next = other.first;
        other.first->previous = last;
        last = other.last;
        length += other.length;
        other.last = nullptr;
        other.first = nullptr;
        other.length = 0;
      }
    } else {
      throw std::invalid_argument("You cannot append a list object to itself");
    }
  }

  template <typename T>
  void List<T>::push_first(const List<T>& other) {
    if (this != &other) {
      for (ListNode<T>* node = other.first; node != nullptr; node = node->next) {
        append(node->value);
      }
    }
    else {
      throw std::invalid_argument("You cannot append a list object to itself");
    }
  }

  template <typename T>
  inline void List<T>::push_first(List<T>&& other) {
    if (this != &other) {
      if (length == 0) {
        *this = std::move(other);
      }
      else {
        first->previous = other.last;
        other.last->next = first;
        first = other.first;
        length += other.length;
        other.last = nullptr;
        other.first = nullptr;
        other.length = 0;
      }
    }
    else {
      throw std::invalid_argument("You cannot use push_first whith the same list object");
    }
  }

  template <typename T>
  void List<T>::push_first(ListNode<T>* new_node) {
    if (length == 0) {
      first = new_node;
      last = new_node;
    }
    else {
      new_node->previous = nullptr;
      new_node->next = first;
      first->previous = new_node;
      first = new_node;
    }
    length++;
  }

  template <typename T>
  void List<T>::push_first(const T& value, int times) {
    for (int i = 0; i < times; i++) {
      push_first(value);
    }
  }

  template <typename T>
  bool List<T>::insert(int pos, ListNode<T>* new_node) {
    if (pos == 0) {
      push_first(new_node);
    }
    else if (pos == length - 1) {
      append(new_node);
    }
    else {
      ListNode<T>* node_in_pos = getNode(pos);
      if (!node_in_pos) return false;
      new_node->previous = node_in_pos->previous;
      new_node->next = node_in_pos;
      // ListNode<T>* new_node = new ListNode<T>(value, node_in_pos->previous, node_in_pos);
      node_in_pos->previous->next = new_node;
      node_in_pos->previous = new_node;
      length++;
    }
    return true;
  }

  template <typename T>
  bool List<T>::insert(int pos, const T& value) {
    ListNode<T>* new_node = new ListNode<T>(value);
    return insert(pos, new_node);
  }

  template <typename T>
  bool List<T>::insert(int pos, T&& value) {
    ListNode<T>* new_node = new ListNode<T>(std::move(value));
    return insert(pos, new_node);
  }

  // It returns a pointer to the node at 'pos' or nullptr if the position is invalid 
  template <typename T>
  ListNode<T>* List<T>::getNode(int pos) {
    if (length == 0 || pos < 0 || pos >= length)
      return nullptr;
    int middle = int(length / 2);
    ListNode<T> * node_to_return = nullptr;
    if (pos <= middle) {
      node_to_return = first;
      for (int i = 0; i < pos; i++) {
        node_to_return = node_to_return->next;
      }
    }
    else {
      node_to_return = last;
      for (int i = length - 1; i > pos; i--) {
        node_to_return = node_to_return->previous;
      }
    }
    return node_to_return;
  }

  // List's function members to delete existing nodes

  template <typename T>
  bool List<T>::pop() {
    if (length > 0) {
      ListNode<T>* node_to_delete = last;
      if (length > 1) {
        last->previous->next = nullptr;
        last = last->previous;
      }
      else {
        last = nullptr;
        first = nullptr;
      }
      delete node_to_delete;
      length--;
      return true;
    }
    return false;
  }

  template <typename T>
  bool List<T>::pop_first() {
    if (length > 0) {
      ListNode<T>* node_to_delete = first;
      if (length > 1) {
        first->next->previous = nullptr;
        first = first->next;
      }
      else {
        last = nullptr;
        first = nullptr;
      }
      delete node_to_delete;
      length--;
      return true;
    }
    return false;
  }

  template <typename T>
  bool List<T>::remove_from_begin(const T& value) {
    for (auto it = Iterator(first); it != NO_ELEMENT; it++) {
      if (*it == value) {
        it = erase(it);
        return true;
      }
    }
    return false;
  }

  template <typename T>
  bool List<T>::remove_from_end(const T& value) {
    for (auto it = Iterator(last); it != NO_ELEMENT; it--) {
      if (*it == value) {
        it = rerase(it);
        return true;
      }
    }
    return false;
  }

  template <typename T>
  bool List<T>::remove_all(const T& value) {
    bool itemsFound = false;
    for (auto it = Iterator(first); it != NO_ELEMENT;) {
      if (*it == value) {
        itemsFound = true;
        it = erase(it);
      }
      else {
        it++;
      }
    }
    return itemsFound;
  }

  template <typename T>
  void List<T>::clear() {
    while (length > 0) {
      pop();
    }
    first = nullptr;
    last = nullptr;
  }

  // List's access methods

  template <typename T>
  const T& List<T>::get(int pos) {
    ListNode<T>* node = getNode(pos);
    if (!node) throw std::out_of_range("The index given does not point to any element");
    return node->value;
  }

  template <typename T>
  bool List<T>::set(int pos, const T& value) {
    ListNode<T>* node = getNode(pos);
    node->value = value;
    return false;
  }

  template <typename T>
  bool List<T>::set(int pos, T&& value) {
    ListNode<T>* node = getNode(pos);
    if (node) {
      node->value = std::move(value);
      return true;
    }
    return false;
  }

  // List's Iterator definition

  template <typename T>
  class List<T>::Iterator {
    friend class List<T>;
  public:
    Iterator(ListNode<typename>* content) : content(content) {}
    // Pre-Increment
    Iterator operator++(int) {
      content = content->next;
      return content;
    }
    // Pre-Decrement
    Iterator operator--(int) {
      content = content->previous;
      return content;
    }
    // Post-Increment
    Iterator operator++() {
      Iterator auxiliar = *this;
      content = content->next;
      return auxiliar;
    }
    // Post-Decrement
    Iterator operator--() {
      Iterator auxiliar = *this;
      content = content->previous;
      return auxiliar;
    }
    // Indirection operator
    T& operator*() { return content->value; }
    // Dereference operator
    T* operator->() { return &(content->value); }
    // Equality and inequality operators
    bool operator!=(const Iterator& other) { return content != other.content; }
    bool operator==(const Iterator& other) { return content == other.content; }
  private:
    ListNode<T>* content;
  };

  template <typename T>
  typename List<T>::Iterator List<T>::getIterator(const T& value)
    const {
    for (auto it = begin(); it != end(); it++) {
      if (*it == value) {
        return it;
      }
    }
    return NO_ELEMENT;
  }

  // List's function members which need to use the List's Iterator type been defined

  template <typename T>
  typename List<T>::Iterator List<T>::erase(Iterator it) {
    ListNode<T>* node = it.content;
    ListNode<T>* next = node->next;
    if (node != nullptr) {
      if (node->next == nullptr) {
        pop();
        next = nullptr;
      }
      else if (!node->previous) {
        pop_first();
      }
      else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        delete node;
        length--;
      }
      return Iterator(next);
    }
    return NO_ELEMENT;
  }

  template <typename T>
  typename List<T>::Iterator List<T>::rerase(Iterator it) {
    ListNode<T>* node = it.content;
    ListNode<T>* previous = node->previous;
    if (node) {
      if (!node->previous) {
        pop_first();
        previous = nullptr;
      }
      else if (!node->next) {
        pop();
      }
      else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        delete node;
        length--;
      }
      return Iterator(previous);
    }
    return NO_ELEMENT;
  }

  // List's utility methods

  template <typename T>
  void List<T>::reverse() {
    if (length > 1) {
      ListNode<T>* node_to_reverse = first;
      for (int i = 0; i < length; i++) {
        ListNode<T> * next_node = node_to_reverse->next;
        ListNode<T> * auxiliar_node = node_to_reverse->previous;
        node_to_reverse->previous = node_to_reverse->next;
        node_to_reverse->next = auxiliar_node;
        node_to_reverse = next_node;
      }
      ListNode<T>* auxiliar_node = first;
      first = last;
      last = auxiliar_node;
    }
  }

  template <typename T>
  List<T> List<T>::reversed() const {
    List<T> resulting_list;
    for (ListNode<T>* node = last; node != nullptr; node = node->previous) {
      resulting_list.append(node->value);
    }
    return resulting_list;
  }

  // I hate this static instantiation but #C++
  template <typename T> const typename List<T>::Iterator List<T>::NO_ELEMENT = nullptr;

  template <typename T>
  std::ostream& operator<<(std::ostream& out, const List<T>& list) {
    for (const T& value : list) {
      out << value << ' ';
    }
    return out;
  }


  // chrishenx namespace end
} 



#endif // LINKED_LIST_H