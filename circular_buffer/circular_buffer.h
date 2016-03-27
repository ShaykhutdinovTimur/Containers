#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h> // size_t ...
#include <stddef.h> // ptrdiff_t...
#include <iterator> // reverse iterator trick


namespace my {

    template<typename T>
          // T is circular_buffer type
    class circular_buffer_iterator {
      public:
        typedef T cbuf_type;
        typedef ptrdiff_t  difference_type;
        typedef circular_buffer_iterator<T> self_type;
        circular_buffer_iterator(
             cbuf_type *b, size_t start_pos)
             : buf_(b), pos_(start_pos) {}

        T &operator*()
                { return (*buf_)[pos_]; }
        T *operator->()
                { return &(operator*()); }

        self_type &operator++() {
          ++pos_;
          if (pos_ == buf_->capacity()) {
              pos_ = 0;
          }
          return *this;
        }
        self_type operator++(int) {
          self_type tmp(*this);
          ++(*this);
          return tmp;
        }
        self_type &operator--() {
          --pos_;
          if (pos_ == -1) {
              pos_ = buf_->capacity() - 1;
          }
          return *this;
        }
        self_type operator--(int) {
          self_type tmp(*this);
          --(*this);
          return tmp;
        }

        self_type operator+(difference_type n)
        {
          self_type tmp(*this);
          tmp.pos_ += n;
          tmp.pos_ %= buf_->capacity();
          return tmp;
        }
        self_type &operator+=(difference_type n) {
          pos_ += n;
          pos_ %= buf_->capacity();
          return *this;
        }
        self_type operator-(difference_type n)
        {
          self_type tmp(*this);
          tmp.pos_ -= n;
          tmp.pos_ += buf_->capacity();
          tmp.pos_ %= buf_->capacity();
          return tmp;
        }

        self_type &operator-=(difference_type n) {
          pos_ -= n;
          pos_ += buf_->capacity();
          pos_ %= buf_->capacity();
          return *this;
        }

      private:
        cbuf_type *buf_;
        size_t     pos_;
    };

    template <class T>
    class circular_buffer {

        typedef T          value_type;
             /* T is template param */
        typedef T         *pointer;
        typedef const T   *const_pointer;
        typedef T         &reference;
        typedef const T   &const_reference;
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;

        typedef circular_buffer<T> self_type;
        typedef circular_buffer_iterator<self_type> iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, size()); }



      public:

        explicit circular_buffer(
            size_t capacity = 100)
            : array_(new T[capacity]),
              array_size_(capacity),
              head_(0), tail_(0),
              contents_size_(0) {}

        ~circular_buffer() {
            delete [] array_;
        }


        reference front()
                       { return array_[head_]; }
        reference back()
                       { return array_[tail_]; }
        const_reference front() const
                       { return array_[head_]; }
        const_reference back() const
                       { return array_[tail_]; }


        void clear()
          { head_ = tail_ = contents_size_ = 0; }

        void increment_tail() {
          ++tail_;
          ++contents_size_;
          if (tail_ == array_size_) tail_ = 0;
        }
        void increment_head(){
          // precondition: !empty()
          ++head_;
          -contents_size_;
          if (head_ == array_size_) head_ = 0;
        }

        void pop_front() { increment_head(); }

        void push_back(const value_type &item) {
          increment_tail();
          if (contents_size_ == array_size_)
            increment_head();
          array_[tail_] = item;
        }

        size_type size() const {
            return contents_size_;
        }
        size_type capacity() const {
            return array_size_;
        }
        bool empty() const {
            return contents_size_ == 0;
        }
        size_type max_size() const {
          return size_type(-1) / sizeof(value_type);
        }
        T& operator[](int index){
            return array_[(index + head_) % array_size_];
        }
        T& operator[](int index) const {
            return array_[(index + head_) % array_size_];
        }

      private:
        T      *array_;
        size_t  array_size_;
        size_t  head_;
        size_t  tail_;
        size_t  contents_size_;
    };
}

#endif // CIRCULAR_BUFFER_H
