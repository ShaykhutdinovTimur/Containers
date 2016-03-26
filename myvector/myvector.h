#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <iterator>

namespace my {

    template<typename T>
    class my_vector_iterator {

    public:
        my_vector_iterator<T>() {
            current = NULL;
        }

        my_vector_iterator(T* c) {
            current = c;
        }

        my_vector_iterator(const my_vector_iterator& rhs) {
            current = rhs.current;
        }

        T& operator*() const {
            return *current;
        }

        my_vector_iterator<T> operator++() {
            current ++;
            return *this;
        }

        my_vector_iterator<T> operator++(int) {
            my_vector_iterator<T> temp(this);
            ++(*this);
            return temp;
        }

        my_vector_iterator<T> operator--() {
            current --;
            return *this;
        }

        my_vector_iterator<T> operator--(int) {
            my_vector_iterator<T> temp(this);
            --(*this);
            return temp;
        }

        bool operator !=(const my_vector_iterator<T>& rhs) const {
            return (current != rhs.current);
        }

        bool operator ==(const my_vector_iterator<T>& rhs) const {
            return (current == rhs.current);
        }

    private:
        T* current;
    };

    template <typename T>
    class my_vector
    {

    private:

        T* elements;
        size_t count;
        size_t reserved;

        void ensure_capacity(int new_capacity) {
            T* new_elements;
            size_t i = -1;
            try {
                new_elements = (T*)malloc(new_capacity * sizeof(T));
                //exceptions only before
                if (elements != NULL) {
                    for(i = 0; i < count; i++) {
                        new (new_elements + i) T (elements[i]);
                    }
                    for(i = 0; i < count; i++) {
                        elements[i].~T();
                    }
                    free(elements);
                }
                elements = new_elements;
                reserved = new_capacity;
            } catch (std::exception& e) {
                for (; i >= 0; i--) {
                    new_elements[i].~T();
                }
                free(new_elements);
                throw e;
            }
        }

    public:

        my_vector() {
            count = 0;
            reserved = 0;
            elements = NULL;
        }

        my_vector(const my_vector<T>& rhs) {
            count = rhs.count;
            reserved = rhs.reserved;
            try {
                ensure_capacity(reserved);
                //exceptions only before
                for(size_t i = 0; i < count; i++) {
                    new (elements + i) T (rhs.elements[i]);
                }
            } catch (std::exception& e) {
                free(elements);
                throw e;
            }            
        }

        my_vector<T>& operator=(const my_vector<T>& rhs) {
            count = rhs.count;
            reserved = rhs.reserved;
            try {
                ensure_capacity(reserved);
                //exceptions only before
                for(size_t i = 0; i < count; i++) {
                    new (elements + i) T (rhs.elements[i]);
                }
            } catch (std::exception& e) {
                free(elements);
                throw e;
            }
            return *this;
        }

        my_vector(size_t n, const T& t) {
            try {
                ensure_capacity(n);
                //exceptions only before
                count = n;
                for(size_t i = 0; i < count; i++) {
                    new (elements + i) T (t);
                }
            } catch (std::exception& e) {
                free(elements);
                throw e;
            }
        }

        void reserve(size_t n) {
            try {
                ensure_capacity(reserved);
                //exceptions only before
                reserved = n;
                count = 0;
            } catch (std::exception& e) {
                free(elements);
                throw e;
            }
        }

        ~my_vector() {
            for(size_t i = 0; i < count; i++) {
                elements[i].~T();
            }
            free(elements);
            count = 0;
        }

        void push_back(const T& rhs) {
            if(count < reserved) {
                new (elements + count)T(rhs);
                count++;
            } else {
                try {
                    ensure_capacity((reserved + 1) * 2);
                    new (elements + count)T(rhs);
                    count++;
                } catch (std::exception& e) {
                    throw e;
                }
            }
        }

        void push_front(const T& rhs) {
            if(count < reserved) {
                for(int i = count; i > 0; i--) {
                    elements[i] = elements[i - 1];
                }
                new (elements)T(rhs);
                count++;
            } else {
                try {
                    T* new_elements;
                    size_t i = -1;
                    try {
                        int new_capacity = (reserved + 1) * 2;
                        new_elements = (T*)malloc(new_capacity * sizeof(T));
                        //exceptions only before
                        if (elements != NULL) {
                            for(i = 0; i < count; i++) {
                                new (new_elements + i + 1) T (elements[i]);
                            }
                            for(i = 0; i < count; i++) {
                                elements[i].~T();
                            }
                            free(elements);
                        }
                        elements = new_elements;
                        reserved = new_capacity;
                    } catch (std::exception& e) {
                        for (; i >= 0; i--) {
                            new_elements[i].~T();
                        }
                        free(new_elements);
                        throw e;
                    }
                    new (elements)T(rhs);
                    count ++;
                } catch (std::exception& e) {
                    throw e;
                }
            }
        }

        void pop_back() {
            if (count > 0) {
                elements[count - 1].~T();
                count --;
            }
        }

        void pop_front() {
            if(count > 0) {
                elements[0].~T();
                for(size_t i = 0; i < count - 1; i++) {
                    new (elements + i)T(elements[i + 1]);
                    elements[i + 1].~T();
                }
                count --;
            }
        }

        T&   front() const {
            return elements[0];
        }

        T&   back() const {
            return elements[count - 1];
        }

        T&   operator[](size_t i) const {
            return elements[i];
        }

        size_t size() const {
            return count;
        }

        bool   empty() const {
            return (count == 0);
        }

        void   clear() {
            for(size_t i = 0; i < count; i++) {
                elements[i].~T();
            }
            count = 0;
        }

        my_vector_iterator<T> begin() const {
            return my_vector_iterator<T>(elements);
        }

        my_vector_iterator<T> end() const {
            return my_vector_iterator<T>(elements + count);
        }

        void erase(const my_vector_iterator<T>& it) {
            count --;

            my_vector_iterator<T> it_temp;
            int position = 0;
            for(it_temp = elements; it_temp != (elements + count - 1); it_temp ++) {
                position ++;
                if(it_temp == it) {
                    break;
                }
            }
            elements[position - 1].~T();
            for(size_t i = (position - 1); i < count; i++) {
                new (elements + i)T(elements[i + 1]);
                elements[i + 1].~T();
            }
        }

        void insert(const T& rhs, const my_vector_iterator<T>& it) {
            try {
                T* temp = (T*)malloc((count) * sizeof(T));
                my_vector_iterator<T> it_temp;
                int position = 0;
                for(it_temp = elements; it_temp != (elements + count - 1); it_temp ++) {
                    position ++;
                    if(it_temp == it) {
                        break;
                    }
                }
                for(int i = 0; i < (position - 1); i++) {
                    new (temp + i)T(elements[i]);
                    elements[i].~T();
                }
                new(temp + position - 1)T(rhs);
                for(size_t i = position; i < count; i++) {
                    new (temp + i)T(elements[i - 1]);
                    elements[i - 1].~T();
                }
                free(elements);
                elements = temp;
            } catch (std::exception& e) {
                throw e;
            }
        }
    };

}

#endif // MYVECTOR_H
