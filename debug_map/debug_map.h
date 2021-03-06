#ifndef DEBUG_MAP_H
#define DEBUG_MAP_H


#include <memory>
#include <iterator>
#include <utility>
#include <functional>
#include <assert.h>
#include <vector>
#include <set>

namespace my
{
    template<typename KEY_T, typename VALUE_T>
    struct debug_map_iterator;

    template<typename KEY_T, typename VALUE_T>
    struct debug_map_node {

        typedef debug_map_node<KEY_T, VALUE_T> self_t;
        typedef debug_map_iterator<KEY_T, VALUE_T> iterator;

        KEY_T key;
        VALUE_T value;
        self_t* left;
        self_t* right;
        self_t* parent;
        std::set<iterator*> iterators;

        void init_pointers() {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

        debug_map_node() {
            init_pointers();
        }

        ~debug_map_node() {
            for (auto i = iterators.begin(); i != iterators.end(); i++) {
                (*i)->invalidate();
            }
            if (left) {
                delete left;
            }
            if (right) {
                delete right;
            }
        }

        debug_map_node(KEY_T key, VALUE_T value) : key(key), value(value) {
            init_pointers();
        }

        debug_map_node(self_t * parent) : parent(parent) {
            left = nullptr;
            right = nullptr;
        }

        debug_map_node(const self_t& other) {
            copy(other);
        }

        self_t& operator=(const self_t& other) {
            copy(other);
            return *this;
        }

        void copy(const self_t& other) {
            assert(other->is_valid());
            if (other.left) {
                left = new debug_map_node(this);
                left->copy(other.left);
            }
            key = other.key;
            value = other.value;
            if (other.right) {
                right = new debug_map_node(this);
                right->copy(other.right);
            }
        }

        bool is_valid() {
            if(left && left->key > key) return false;
            if(right && right->key < key) return false;
            if((left && left->parent != this) ||
                    (right && right->parent != this)) {
                return false;
            }
            return true;
        }

        bool is_valid_tree() {
            bool result = true;
            if (left) {
                result = result && left->is_valid_tree();
            }
            result = result && is_valid();
            if (right) {
                result = result && right->is_valid_tree();
            }
            return true;
        }

        self_t* find(const KEY_T& key) {
            assert(is_valid() && "No valid node");
            if(key < this->key) {
                if(left) {
                    return left->find(key);
                } else {
                    return left;
                }
            }
            if(key > this->key) {
                if(right) {
                    return right->find(key);
                } else {
                    return right;
                }
            }
            return this;
        }

        self_t* erase(const KEY_T& key) {
            assert(is_valid() && "No valid node");
            if(key < this->key) {
                if (left) {
                    return left->erase(key);
                } else {
                    return left;
                }
            }
            if(key > this->key) {
                if (right) {
                    return right->erase(key);
                } else {
                    return right;
                }
            }
            if(left && right) {
                self_t* min_ptr = right->min();
                if(min_ptr == right) {
                    get_parent_ptr() = min_ptr;
                    min_ptr->parent = parent;
                    min_ptr->left = left;
                    if(left) {
                        left->parent = min_ptr;
                    }
                } else {
                    min_ptr->get_parent_ptr() = min_ptr->right;
                    if(min_ptr->right) {
                        min_ptr->right->parent = min_ptr->parent;
                    }

                    min_ptr->parent = parent;
                    get_parent_ptr() = min_ptr;
                    min_ptr->left = left;
                    if(left) {
                        left->parent = min_ptr;
                    }
                    min_ptr->right = right;
                    if(right) {
                        right->parent = min_ptr;
                    }
                }
                return this;
            }
            if(left) {
                get_parent_ptr() = left;
                left->parent = parent;
            } else {
                get_parent_ptr() = right;
                if(right) {
                    right->parent = parent;
                }
            }
            return this;
        }

        void insert(self_t* ptr) {
            assert(is_valid() && "No valid node");
            if(ptr->key < key) {
                if(left) {
                    return left->insert(ptr);
                } else {
                    left = ptr;
                    ptr->parent = this;
                }
            }
            if(ptr->key > key) {
                if(right) {
                    return right->insert(ptr);
                } else {
                    right = ptr;
                    ptr->parent = this;
                }
            }
        }

        void insert_left(self_t* ptr) {
            assert(is_valid() && "No valid node");
            if(left) {
                left->insert(ptr);
            } else {
                left = ptr;
                ptr->parent = this;
            }
        }

        self_t* min() {
            assert(is_valid());
            if(left) {
                return left->min();
            } else {
                return this;
            }
        }

        self_t* max() {
            assert(is_valid() && "No valid node");
            if(right) {
                return right->max();
            } else {
                return this;
            }
        }

        bool is_left_child() {
            assert(parent != nullptr);
            if(parent->left == this) {
                return true;
            }
            return false;
        }

        self_t*& get_parent_ptr() {
            assert(is_valid() && "No valid node");
            if(is_left_child()) {
                return parent->left;
            }
            return parent->right;
        }

    };

    template<typename KEY_T, typename VALUE_T>
    struct debug_map_iterator {
        typedef debug_map_node<KEY_T, VALUE_T> node_type;
        typedef debug_map_iterator<KEY_T, VALUE_T> self_t;

        node_type* node;
        node_type* root;
        bool end;

        debug_map_iterator() {
            end = false;
        }

        debug_map_iterator(const self_t& other) {
            copy(other);
        }

        debug_map_iterator(node_type * node, node_type * root, bool end) : node(node), root(root), end(end) {
            assert(node != nullptr);
            node->iterators.insert(this);
        }

        ~debug_map_iterator() {
            if (node) {
                node->iterators.erase(this);
            }
        }

        self_t& operator=(const self_t& other) {
            copy(other);
            return *this;
        }

        void copy(const self_t& other) {
            node = other.node;
            root = other.root;
            end = other.end;
        }

        void invalidate() {
            root = nullptr;
            node = nullptr;
            end = false;
        }

        self_t& operator++() {
            assert(node != nullptr);
            assert(node != root);
            node_type* current = root->left;
            node_type* result = nullptr;
            while(current) {
                if(current->key > node->key) {
                    result = current;
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            if(result) {
                if(result == root) {
                    end = true;
                }
                node = result;
            } else {
                end = true;
            }
            return *this;
        }

        self_t operator++(int) {
            self_t result = *this;
            ++(*this);
            return result;
        }

        self_t& operator--() {
            assert(node != nullptr);
            node_type* current = root;
            node_type* result = nullptr;
            while(current) {
                if(current->key < node->key) {
                    result = current;
                    current = current->right;
                } else {
                    current = current->left;
                }
            }
            assert(result != nullptr);
            node = result;
            return *this;
        }

        self_t operator--(int) {
            self_t result = *this;
            --(*this);
            return result;
        }

        KEY_T const& operator*() const {
            assert(node != nullptr);
            return node->key;
        }

        KEY_T const* operator->() const {
            assert(node != nullptr);
            return &node->key;
        }

        bool operator==(const self_t& other) {
            assert(root == other.root);
            return (end == other.end);
        }

        bool operator!=(const self_t& other) {
            return !this->operator==(other);
        }

        bool is_end() {
            return end;
        }
    };

    template<typename KEY_T, typename VALUE_T>
    struct debug_map {
        typedef debug_map<KEY_T, VALUE_T> self_t;
        typedef debug_map_iterator<KEY_T, VALUE_T> iterator;
        typedef debug_map_node<KEY_T, VALUE_T> node_t;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        int size;

        debug_map() {
            size = 0;
        }

        debug_map(const self_t& other) {
            copy(other);
        }

        self_t& operator=(const self_t& other) {
            copy(other);
            return *this;
        }

        void copy(const self_t& other) {
            size = other.size();
            fake_root = other.fake_root;
        }

        ~debug_map() {
        }

        iterator insert(KEY_T key, VALUE_T value) {
            iterator res(new node_t(key, value), &fake_root, false);
            fake_root.insert_left(res.node);
            size++;
            return res;
        }

        iterator end() {
            iterator result(&fake_root, &fake_root, true);
            return result;
        }

        iterator begin() {
            iterator result(fake_root.min(), &fake_root, false);
            return result;
        }

        iterator find(KEY_T key) {
            if(!fake_root.left) {
                return end();
            }
            node_t * result = fake_root.left->find(key);
            if(result) {
                iterator res(result, &fake_root, false);
                return res;
            } else {
                return end();
            }
        }

        iterator erase(KEY_T key) {
            iterator it = find(key);
            assert(it != end());
            iterator next = it;
            next++;
            KEY_T next_value = *next;

            node_t* delete_node = it.node;
            fake_root.erase(key);
            delete_node->left = nullptr;
            delete_node->right = nullptr;
            delete delete_node;
            return find(next_value);
        }

        bool check_tree() {
            return fake_root.is_valid_tree();
        }

    private:
        node_t fake_root;
    };
}

#endif // debug_map_H
