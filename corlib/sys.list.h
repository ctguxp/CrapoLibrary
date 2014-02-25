// Perch Lake Computer System

#pragma once

namespace sys
  {
  template<class T>
  class list;

  template<class T>
  class link
    {
    public:
      // Destructor
      ~link()
        {
        if(_next != nullptr)
          {
          delete _next;
          _next = nullptr;
          }
        }
      // Get data property
      T get_datum() const { return _datum; }
      // Get next link property
      link* get_next() const { return _next; }
    private:
      // Constructor take data and next link
      link(T datum, link* next)
        :_datum(datum)
        ,_next(next)
        {
        }
    private:
      T     _datum;  // Data member
      link* _next;   // Pointer to next link
      
      friend list<T>;
    };

  template<class T>
  class list
    {
    public:
      // Default constructor
      list()
        :_head(nullptr)
        ,_tail(nullptr)
        {
        }
      // Destructor
      ~list()
        {
        free();
        }
      // Is empty list
      bool is_empty() const { return _head == nullptr; }
      // Appends to list
      void append(const T& i)
        {
        link<T>* p = new link<T>(i, nullptr);
        if(_head == nullptr)
          _head = p;
        else
          _tail->_next = p;
        _tail = p;
        }
      // Prepends to end of list
      void prepend(const T& i)
        {
        link<T>* p = new link<T>(i, _head);
        if(_head == nullptr)
          _tail = p;
        _head = p;
        }
      // Get head link property
      link<T> const* get_head() const { return _head; }
    private:
      // Delete pointers
      void free()
        {
        if(_head != nullptr)
          {
          delete _head;
          _head = nullptr;
          _tail = nullptr;
          }
        int x = 0;
        x++;
        }
    private:
      link<T>* _head; // Head of the list
      link<T>* _tail; // Tail of the list
    };
  }
