#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <exception>        // std::out_of_range
#include <iostream>         // std::cout, std::endl
#include <memory>           // std::unique_ptr
#include <iterator>         // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <algorithm>        // std::copy, std::equal, std::fill
#include <initializer_list> // std::initializer_list
#include <cassert>          // assert()
#include <limits>           // std::numeric_limits<T>
#include <cstddef>          // std::size_t
#include <array>            // std::array

/// Sequence container namespace.
namespace sc {
    /// Implements tha infrastructure to support a bidirectional iterator.
    template < class T >
    class MyForwardIterator : public std::iterator<std::bidirectional_iterator_tag, T>{
        public:
            typedef MyForwardIterator self_type;                        //!< Alias to iterator.
            // Below we have the iterator_traits common interface
            typedef std::ptrdiff_t difference_type;                     //!< Difference type used to calculated distance between iterators.
            typedef T value_type;                                       //!< Value type the iterator points to.
            typedef T* pointer;                                         //!< Pointer to the value type.
            typedef T& reference;                                       //!< Reference to the value type.
            typedef const T& const_reference;                           //!< Reference to the value type.
            typedef std::bidirectional_iterator_tag iterator_category;  //!< Iterator category.

            /*! 
             * @brief Create an iterator around a raw pointer.
             * \param pt_ raw pointer to the container.
            */
            MyForwardIterator( pointer pt=nullptr ) : m_ptr( pt ) { /* empty */ }
            /*!
             * @brief Copy constructor.
             * @param rhs The iterator to copy.
            */    
            MyForwardIterator( const self_type & ) = default;
            /*! Overloaded `=` operator.
             * @brief Assigns a new value to the iterator.
             * @param & The reference of the new value to assign.
             * @returns A reference to the iterator.
            */
            self_type& operator=( const self_type & ) = default;
            /*! Overloaded assignment ('*') operator.
             * @brief Access the content the iterator points to.
             * @returns A reference to the content the iterator points to.
            */
            reference operator*( void ) const {  
                assert( this->m_ptr != nullptr ); 
                return *this->m_ptr; 
            }
            /*! Overloaded `->` operator.
             * @brief Access the content the iterator points to.
             * @returns A pointer to the content the iterator points to.
            */
            pointer operator->( void ) const {   
                assert( this->m_ptr != nullptr ); 
                return this->m_ptr; 
            }
            /*! Pre-increment operator.
             * @brief Increments the iterator to the next element. Overloaded `++obj` operator.
             * @returns A reference to the iterator at the next element.
            */
            self_type& operator++( void ){
                this->m_ptr++;
                return *this;
            }
            /*! Post increment.
             * @brief Increments the iterator to the next element. Overloaded `obj++` operator.
             * @returns A copy of the iterator at this element.
            */
            self_type operator++( int ){
                self_type previous_ptr{*this};
                this->m_ptr++;
                return previous_ptr;
            }
            /*! Pre-decrement operator.
             * @brief Decrements the iterator to the previous element. Overloaded `--obj` operator.
             * @returns A reference to the iterator at the previous element.
            */
            self_type operator--( void ){
                this->m_ptr--;
                return *this;
            }
            /*! Post-decrement operator.
             * @brief Decrements the iterator to the previous element. Overloaded `obj--` operator.
             * @returns A copy of the iterator at this element.
            */
            self_type operator--( int ){
                self_type previous_ptr{*this};
                this->m_ptr--;
                return previous_ptr;
            }
            /*! Add and assign operator.
             * @brief Adds a distance to the actual iterator. Overloaded `obj += offset` operator.
             * @param offset The distance to add to the iterator.
             * @returns A reference to the new iterator.
            */
            self_type& operator+=(difference_type offset){
                this->m_ptr += offset;
                return *this;
            }
            /*! Subtract and assign operator.
             * @brief Subtracts a distance from the actual iterator. Overloaded `obj -= offset` operator.
             * @param offset The distance to subtract from the iterator.
             * @returns A reference to the new iterator.
            */
            self_type& operator-=(difference_type offset){
                this->m_ptr -= offset;
                return *this;
            }
            /*! Relational less than operator.
             * @brief Checks if the actual iterator is less than the other one. Overloaded `obj < other` operator.
             * @param ita The iterator to compare with itb.
             * @param itb The iterator to compare ita with.
             * @returns True if the actual iterator is less than the other one, false otherwise.
            */
            friend bool operator<(const self_type& ita, const self_type& itb){ return ita-itb < 0; }
            /*! Relational greater than operator.
             * @brief Checks if the actual iterator is greater than the other one. Overloaded `obj > other` operator.
             * @param ita The iterator to compare with itb.
             * @param itb The iterator to compare ita with.
             * @returns True if the actual iterator is greater than the other one, false otherwise.
            */
            friend bool operator>(const self_type& ita, const self_type& itb) { return ita-itb > 0; }
            /*! Relational less than or equal operator.
             * @brief Checks if the actual iterator is less than or equal to the other one. Overloaded `obj <= other` operator.
             * @param ita The iterator to compare with itb.
             * @param itb The iterator to compare ita with.
             * @returns True if the actual iterator is less than or equal to the other one, false otherwise.
            */
            friend bool operator<=(const self_type& ita, const self_type& itb) { return ita-itb <= 0; }
            /*! Relational greater than or equal operator.
             * @brief Checks if the actual iterator is greater than or equal to the other one. Overloaded `obj >= other` operator.
             * @param ita The iterator to compare with itb.
             * @param itb The iterator to compare ita with.
             * @returns True if the actual iterator is greater than or equal to the other one, false otherwise.
            */
            friend bool operator>=(const self_type& ita, const self_type& itb){ return ita-itb >= 0; }
            /*! Add operator.
             * @brief Adds a distance to the actual iterator. Overloaded `offset + obj` operator.
             * @param offset The distance to add to the iterator.
             * @param it The iterator to add the distance to.
             * @returns The value of the iterator plus the distance.s
            */
            friend self_type operator+( difference_type offset, self_type it ){ return self_type{it.m_ptr + offset}; }
            /*! Add operator.
             * @brief Adds a distance to the actual iterator. Overloaded `obj + offset` operator.
             * @param it The iterator to add the distance to.
             * @param offset The distance to add to the iterator.
             * @returns The value of the iterator plus the distance.
            */
            friend self_type operator+( self_type it, difference_type offset ) { return self_type{it.m_ptr + offset }; }
            /*! Subtract operator.
             * @brief Subtracts a distance from the actual iterator. Overloaded `obj - offset` operator.
             * @param offset The distance to subtract from the iterator.
             * @param it The iterator to subtract the distance from.
             * @returns The value of the iterator minus the distance.
            */
            friend self_type operator-( self_type it, difference_type offset ){ return self_type{it.m_ptr - offset}; }
            /*! Equality operator.
             * @brief Checks if the actual iterator is equal to the other one. Overloaded `obj == other` operator.
             * @param rhs_ A reference to the iterator to compare with.
             * @returns True if the actual iterator is equal to the other one, false otherwise.
            */
            bool operator==( const self_type & rhs_ ) const { return this->m_ptr == rhs_.m_ptr; }
            /*! Inequality operator.
             * @brief Checks if the actual iterator is not equal to the other one. Overloaded `obj != other` operator.
             * @param rhs_ A reference to the iterator to compare with.
             * @returns True if the actual iterator is not equal to the other one, false otherwise.
            */
            bool operator!=( const self_type & rhs_ ) const { return this->m_ptr != rhs_.m_ptr; }
            /*! Subtract operator.
             * @brief Mensure the distance between the actual iterator and the other one. Overloaded `obj - other` operator.
             * @param rhs_ A reference to the iterator for which to calculate the distance.
             * @returns The distance between the actual iterator and the other one.
            */
            difference_type operator-( const self_type & rhs_ ) const { return std::distance(rhs_.m_ptr, this->m_ptr); }
            /*! Binary left shift operator.
             * @brief Extract a printable stream of informations from the iterator. Overloaded `obj << stream` operator.
             * @param os_ The stream object to write the informations to.
             * @param p_ The iterator to extract the informations from.
             * @returns The stream object.
            */
            friend std::ostream& operator<<( std::ostream& os_, const MyForwardIterator &p_ ) {
                os_ << "[@ " << p_.m_ptr  << ": " << *p_.m_ptr << " ]" ;
                return os_;
            }

        private:
            pointer m_ptr; //!< The raw pointer.
    };

    /// This class implements the ADT list with dynamic array.
    /*!
     * sc::vector is a sequence container that encapsulates dynamic size arrays.
     *
     * The elements are stored contiguously, which means that elements can
     * be accessed not only through iterators, but also using offsets to
     * regular pointers to elements.
     * This means that a pointer to an element of a vector may be passed to
     * any function that expects a pointer to an element of an array.
     *
     * \tparam T The type of the elements.
    */
    template < typename T >
    class vector {   
        //=== Aliases
        public:
            using size_type = unsigned long;                                //!< The size type.
            using value_type = T;                                           //!< The value type.
            using pointer = value_type*;                                    //!< Pointer to a value stored in the container.
            using reference = value_type&;                                  //!< Reference to a value stored in the container.
            using const_reference = const value_type&;                      //!< Const reference to a value stored in the container.
            using iterator = MyForwardIterator< value_type >;               //!< The iterator, instantiated from a template class.
            using const_iterator = MyForwardIterator< const value_type >;   //!< The const_iterator, instantiated from a template class.
        private:
            size_type m_end;                //!< The list's current size (or index past-last valid element).
            size_type m_capacity;           //!< The list's storage capacity.
            std::unique_ptr<T[]> m_storage; //!< The list's data storage area.

            bool full(void) const { return this->size() == this->capacity(); }
        public:
            //=== [I] SPECIAL MEMBERS (6 OF THEM)
            /*! Default constructor.
             * @brief Creates an vector with the given capacity (if given) of elements.
             * @param count The capacity of the vector.
            */
            explicit vector(size_type count = 0) {
                this->m_capacity = count;
                this->m_end = count;
                this->m_storage.reset(new value_type[this->m_capacity]);
                iterator it = this->begin();
                std::fill(it, it+this->m_capacity, *new value_type);
            }
            /*! Default Destructor.
             * @brief Destroys the vector.
            */
            virtual ~vector(void) = default;
            /*! Copy constructor.
             * @brief Creates a copy of the vector.
             * @param other The vector to copy.
            */
            vector(const vector& other) {
                this->m_capacity = other.capacity();
                this->m_end = other.size();
                this->m_storage.reset(new value_type[this->capacity()]);
                const_iterator other_it = other.cbegin();
                iterator it_this = this->begin();
                std::copy(other_it, other_it+this->size(), it_this);
            }
            /* There are no tests for this move constructor.
                vector(vector&& other) {
                    this->m_capacity = other.capacity();
                    this->m_end = other.size();
                    this->m_storage.reset(new value_type[this->capacity()]);
                    const_iterator other_it = other.cbegin();
                    iterator it_this = this->begin();
                    std::copy(other_it, other_it+this->size(), it_this);
                }   
            */
            /*! Copy assignment operator.
             * @brief Assigns a copy of the vector to the current one.
             * @param rhs The vector to copy.
             * @returns A reference to the current vector.
            */
            vector& operator=(const vector& rhs) {
                if(this != &rhs) {
                    this->m_capacity = rhs.capacity();
                    this->m_end = rhs.size();
                    this->m_storage.reset(new value_type[this->capacity()]);
                    const_iterator rhs_it = rhs.cbegin();
                    iterator it_this = this->begin();
                    std::copy(rhs_it, rhs_it+this->size(), it_this);
                }
                return *this;
            }
            /* There are no tests for this move assignment operator (constructor)
                vector& operator=(vector&& rhs) {
                    if(&this != &rhs) {
                        this->m_capacity = rhs.capacity();
                        this->m_end = rhs.size();
                        this->m_storage.reset(new value_type[this->capacity()]);
                        const_iterator rhs_it = rhs.cbegin();
                        iterator it_this = this->begin();
                        std::copy(rhs_it, rhs_it+this->size(), it_this);
                    }
                    return *this;
                }
            */
            /*! Ilicializer list constructor.
             * @brief Creates a vector from a list of elements.
             * @param il The list of elements to initialize the vector with.
            */
            vector(const std::initializer_list<T>& il) {
                this->m_capacity = il.size();
                this->m_end = il.size();
                this->m_storage.reset(new value_type[this->capacity()]);
                iterator it = this->begin();
                std::copy(il.begin(), il.end(), it);
            }
            /*! Range constructor.
             * @brief Creates a vector from a range of elements.
             * @param first The first element of the range.
             * @param last The past-the-last element of the range.
            */
            template < typename InputItr >
            vector(InputItr first, InputItr last) {
                this->m_capacity = std::distance(first, last);
                this->m_end = this->capacity();
                this->m_storage.reset(new value_type[this->capacity()]);
                iterator it = this->begin();
                std::copy(first, last, it);
            }

            //=== [II] ITERATORS
            /// Returns an iterator to the first element of the vector.
            iterator begin(void) { return iterator(&this->m_storage[0]); }
            /// Returns a const_iterator to the first element of the vector.
            const_iterator cbegin(void) const { return const_iterator(&this->m_storage[0]); }
            /// Returns an iterator to the past-the-last element of the vector.
            iterator end(void) { return iterator(&this->m_storage[this->size()]); }
            /// Returns a const_iterator to the past-the-last element of the vector.
            const_iterator cend(void) const { return const_iterator(&this->m_storage[this->size()]); }

            // [III] Capacity
            /// Returns the number of elements in the vector.
            size_type size(void) const { return this->m_end; }
            /// Returns the capacity of the vector.
            size_type capacity(void) const { return this->m_capacity; }
            /// Returns true if the vector is empty.
            bool empty(void) const { return this->size() == 0; }

            // [IV] Modifiers
            /// Clears the vector removing all elements from it, but leaving the capacity unchanged.
            void clear(void) {
                this->m_end = 0; //!< Reset the end index.
                this->m_storage.reset(new value_type[this->capacity()]);
            }
            /// Adds a new element to the end of the vector.
            void push_back(const_reference value) {
                if(this->full()) this->capacity() > 0? this->reserve(this->capacity()*2) : this->reserve(1);
                this->m_storage[this->size()] = value;
                this->m_end++;
            }
            /// Removes the last element from the vector.
            void pop_back(void) { 
                if (this->empty()) throw std::out_of_range("vector::pop_back(): vector is empty");
                this->m_storage[this->size()-1] = *new value_type(0u);
                this->m_end--;
            }
            /*! 
             * @brief Inserts a new element at a given position.
             * @param pos_ The position to insert the element at.
             * @param value_ The value to insert.
             * @returns An iterator to the newly inserted element.
             * \exception std::out_of_range If the position is greater than the size of the vector.
             * \exception std::runtime_error If the vector pointer is null.
            */
            iterator insert( iterator pos_ , const_reference value_ ) { 
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                size_type offset_pos = pos_-this->begin();
                if (offset_pos < 0 or offset_pos > this->size()) throw std::out_of_range("vector::insert(): invalid range");
                if (this->full()) this->capacity() > 0? this->reserve(this->capacity()*2) : this->reserve(1);

                std::copy_backward(this->begin()+offset_pos, this->end(), this->end()+1);
                this->m_storage[offset_pos] = value_;
                this->m_end++;
                return iterator(this->begin()+ offset_pos);
            }
            /// Const version of insert given a value and the position to insert it at.
            iterator insert( const_iterator pos_ , const_reference value_ ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                size_type offset_pos = pos_-this->begin();
                if (offset_pos < 0 or offset_pos > this->size()) throw std::out_of_range("vector::insert(): invalid range");
                if (this->full()) this->capacity() > 0? this->reserve(this->capacity()*2) : this->reserve(1);

                std::copy_backward(this->begin()+offset_pos, this->end(), this->end()+1);
                this->m_storage[offset_pos] = value_;
                this->m_end++;
                return iterator(this->begin()+ offset_pos);
            }
            /*!
             * @brief Inserts a range of elements at a given position.
             * @param pos_ The position to insert the elements at.
             * @param first_ The first element of the range.
             * @param last_ The past-the-last element of the range.
             * @returns An iterator to the first element of the inserted range.
             * \exception std::out_of_range If the position is greater than the size of the vector.
             * \exception std::runtime_error If the vector pointer is null.
             * \exception std::out_of_range If the position is greater than the size of the vector.
            */
            template < typename InputItr >
            iterator insert( iterator pos_ , InputItr first_, InputItr last_ ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                if (first_ >= last_) throw std::invalid_argument("vector::insert: invalid input list");
                size_type input_list_size = std::distance(first_, last_);
                size_type offset_pos = pos_-this->begin();
                if (offset_pos < 0 or offset_pos > this->size()) throw std::out_of_range("vector::insert(): invalid range");
                if (this->capacity() < this->size()+input_list_size) reserve(this->size()+input_list_size);

                std::copy_backward( this->begin() + offset_pos,
                                    this->end(), 
                                    this->end()+ input_list_size);
                std::copy(first_, last_, this->begin()+offset_pos);
                this->m_end += input_list_size;
                return iterator(this->begin()+ offset_pos);
            }
            /// Const version of insert given a range of elements and the position to insert them at.
            template < typename InputItr >
            iterator insert( const_iterator pos_ , InputItr first_, InputItr last_ ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                if (first_ >= last_) throw std::invalid_argument("vector::insert: invalid input list");
                size_type input_list_size = std::distance(first_, last_);
                size_type offset_pos = pos_-this->begin();
                if (offset_pos < 0 or offset_pos > this->size()) throw std::out_of_range("vector::insert(): invalid range");
                if (this->capacity() < this->size()+input_list_size) reserve(this->size()+input_list_size);

                std::copy_backward( this->begin() + offset_pos,
                                    this->end(), 
                                    this->end()+ input_list_size);
                std::copy(first_, last_, this->begin()+offset_pos);
                this->m_end += input_list_size;
                return iterator(this->begin()+ offset_pos);
            }
            /*!
             * @brief Inserts a inicializer list into the vector at a given position.
                * @param pos_ The position to insert the elements at.
                * @param ilist_ The inicializer list to insert.
                * @returns An iterator to the first element of the inserted range.
                * \exception std::out_of_range If the position is greater than the size of the vector.
                * \exception std::runtime_error If the vector pointer is null.
                * \exception std::out_of_range If the position is greater than the size of the vector.
            */
            iterator insert( iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                return this->insert(pos_, ilist_.begin(), ilist_.end());
            }
            /// Const version of insert given a inicializer list and the position to insert it at.
            iterator insert( const_iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                return this->insert(pos_, ilist_.begin(), ilist_.end());;
            }
            /*!
             * @brief Reserves a given extra amount of space in the vector.
             * @param extra_capacity_ The amount of extra space to reserve.
            */
            void reserve(size_type new_capacity) {
                if(new_capacity <= this->capacity()) return;
                std::unique_ptr<value_type[]> new_storage;
                //Resize the vector to the new capacity.
                new_storage.reset(new value_type[new_capacity]);
                std::copy(this->begin(), this->end(), new_storage.get());
                //Assign the new array to the old one.
                this->m_storage = std::move(new_storage);
                //Update the capacity.
                this->m_capacity = new_capacity;
            }
            /*!
             * @brief Shrink the storage capacity to the size of the vector.
            */
            void shrink_to_fit(void) {
                //Check if the capacity is greater than the size.
                if (this->capacity() <= this->size()) return;
                std::unique_ptr<value_type[]> new_storage;
                new_storage.reset(new value_type[this->size()]);
                std::copy(this->begin(), this->end(), new_storage.get());
                //Assign the new array to the old one.
                this->m_storage = std::move(new_storage);
                //Update the capacity.
                this->m_capacity = this->size();   
            }
            /*!
             * @brief Change the firsts elements of the vector to a given value.
             * @param count_ The number of elements to change.
             * @param value_ The value to change the elements to.
            */
            void assign(size_type count_, const_reference value_) {
                if(count_ > this->capacity()) reserve(count_);
                std::fill(this->begin(), this->begin()+count_, value_);
                this->m_end = count_;
            }
            /*!
             * @brief Change the firsts elements of the vector to the elements of a given initializer list.
             * @param ilist The initializer list to change the elements to.
            */
            void assign(const std::initializer_list<T>& ilist) {
                if(ilist.size() > this->capacity()) reserve(ilist.size());
                std::copy(ilist.begin(), ilist.end(), this->begin());
                this->m_end = ilist.size();
            }
            /*!
             * @brief Change the firsts elements of the vector to the elements of a given range.
             * @param first The first element of the range to change the elements to.
             * @param last The last element of the range to change the elements to.
            */
            template < typename InputItr >
            void assign(InputItr first, InputItr last) {
                if(std::distance(first, last) > capacity()) reserve(std::distance(first, last));
                std::copy(first, last, this->begin());
                this->m_end = std::distance(first, last);
            }
            /*!
             * @brief Removes the range of elements from the vector.
             * @param first The first element of the range to remove.
             * @param last The last element of the range to remove.
             * @returns An iterator to the first element of the removed range.
             * \exception std::out_of_range If the position is greater than the size of the vector.
             * \exception std::runtime_error If the vector pointer is null.
            */
            iterator erase( iterator first, iterator last ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::erase(): vector is empty");
                if (this->empty()) throw std::out_of_range("vector::erase(): empty vector");
                if (first >= last) throw std::invalid_argument("vector::erase(): invalid range");
                size_type input_list_size = last-first;
                size_type offset_first = first-this->begin();
                size_type offset_last = last-this->begin();
                if (offset_first < 0 or offset_last > this->size()) throw std::out_of_range("vector::erase(): invalid range");

                std::copy(  this->begin() + offset_first+ input_list_size,
                            this->end(), 
                            this->begin()+ offset_first);
                this->m_end = this->cend()-this->cbegin() - input_list_size;
                return iterator(this->begin()+ offset_first);
            }
            /// Const version of erase given a range.
            iterator erase( const_iterator first, const_iterator last ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::erase(): vector is empty");
                if (this->empty()) throw std::out_of_range("vector::erase(): empty vector");
                if (first >= last) throw std::invalid_argument("vector::erase(): invalid range");
                size_type input_list_size = last-first;
                size_type offset_first = first-this->begin();
                size_type offset_last = last-this->begin();
                if (offset_first < 0 or offset_last > this->size()) throw std::out_of_range("vector::erase(): invalid range");

                std::copy(  this->begin() + offset_first+ input_list_size,
                            this->end(), 
                            this->begin()+ offset_first);
                this->m_end = this->cend()-this->cbegin() - input_list_size;
                return iterator(this->begin()+ offset_first);
            }
            /*!
             * @brief Removes the element at a given position.
             * @param pos The position of the element to remove.
             * @returns An iterator to the element that was removed.
             * \exception std::out_of_range If the position is greater than the size of the vector.
             * \exception std::runtime_error If the vector pointer is null.
            */
            iterator erase( iterator pos ) { return this->erase(pos, pos+1); }
            /// Const version of erase given a position.
            iterator erase( const_iterator pos ) { return this->erase(pos, pos+1); }

            // [V] Element access
            /// Returns a const value reference to the first element of the vactor.
            const_reference front(void) const { return this->at(0); }
            /// Returns a const value reference to the last element of the vector.
            const_reference back(void) const { return this->at(this->size()-1); }
            /// Returns a reference to the first element of the vector.
            reference front(void) { return this->at(0); }
            /// Returns a reference to the last element of the vector.
            reference back(void) { return this->m_storage[this->size()-1]; }
            /// Returns a const reference to the element at a given position. This positon could or could not be in the vector.
            const_reference operator[](size_type pos) const {return m_storage[pos]; }
            /// Returns a reference to the element at a given position. This positon could or could not be in the vector.
            reference operator[](size_type pos) { return m_storage[pos]; }
            /// Returns a const reference to the element at a given position. This position must be in the vector.
            const_reference at(size_type pos) const {
                if(pos >= this->m_end) throw std::out_of_range("Index out of bounds.");
                return m_storage[pos];
            }
            /// Returns a reference to the element at a given position. This position must be in the vector.
            reference at(size_type pos) {
                if(pos >= this->m_end) throw std::out_of_range("Index out of bounds.");
                return m_storage[pos];
            }

            /*! \todo Implement the following functions:
                pointer data(void) { return this->m_storage.get(); }
            */
            /*! \todo Implement the following functions:
                const_reference data(void) const { return this->m_storage;  }
            */

            // [VII] Friend functions.
            /*!
             * @brief Writes the contents of the vector to an output stream object.
             * @details This function is a friend of the vector class 
             *          and can be used to write the contents of the vector to an output stream.
             * @param os_ The output stream object to write to.
             * @param v_ The vector to write.
             * @returns The output stream object.
            */
            friend std::ostream & operator<<(std::ostream & os_, const vector<T> & v_) {
                os_ << "{ ";
                for( auto i{0u} ; i < v_.capacity(); ++i )
                {
                    if ( i == v_.size() ) os_ << "| ";
                    os_ << v_.m_storage[ i ] << " ";
                }
                os_ << "}, m_end=" << v_.size() << ", m_capacity=" << v_.capacity();
                return os_;
            }
            /*!
             * @brief Swaps the contents of two given vectors.
             * @details This function is a friend of the vector class
             *          and can be used to swap the contents of two vectors.
             * @param first_ The first vector to swap.
             * @param second_ The second vector to swap.
            */
            friend void swap(vector<T> & first_, vector<T> & second_) {
                // enable ADL
                using std::swap;

                // Swap each member of the class.
                swap( first_.m_end,      second_.m_end      );
                swap( first_.m_capacity, second_.m_capacity );
                swap( first_.m_storage,  second_.m_storage  );
            }
    };

    // [VI] Operators
    /*!
     * @brief Compares two vectors for equality.
     * @details This function call the equal function to compare two vectors.
     * @param va The first vector to compare.
     * @param vb The second vector to compare.
     * @returns True if the vectors are equal, false otherwise.
    */
    template <typename T>
    bool operator==( const vector<T> &va, const vector<T>&vb ){ return equal(va, vb); }
    /*!
     * @brief Compares two vectors for inequality.
     * @details This function call the equal function and inverts the result.
     * @param va The first vector to compare.
     * @param vb The second vector to compare.
     * @returns True if the vectors are not equal, false otherwise.
    */
    template <typename T>
    bool operator!=( const vector<T> &va, const vector<T>&vb ){ return not equal(va, vb); }
    /*!
     * @brief Compares two vectors for equality.
     * @param va The first vector to compare.
     * @param vb The second vector to compare.
     * @returns True if the vectors are equal, false otherwise.
    */
    template <typename T>
    bool equal( const vector<T> &va, const vector<T>&vb ){
        if(va.size() != vb.size()) return false;
        for(auto i = 0; i < va.size(); ++i) if(va.at(i) != vb.at(i)) return false;
        return true;
    }
} // namespace sc.

#endif // _VECTOR_H_