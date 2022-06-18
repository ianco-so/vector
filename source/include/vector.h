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

            /*! Create an iterator around a raw pointer.
             * \param pt_ raw pointer to the container.
             */
            MyForwardIterator( pointer pt=nullptr ) : m_ptr( pt ) { /* empty */ }

            /// Access the content the iterator points to.
            reference operator*( void ) const {  
                assert( m_ptr != nullptr ); 
                return *m_ptr; 
            }

            /// Overloaded `->` operator.
            pointer operator->( void ) const {   
                assert( m_ptr != nullptr ); 
                return m_ptr; 
            }

            /// Assignment operator.
            self_type& operator=( const self_type & ) = default;
            /// Copy constructor.
            MyForwardIterator( const self_type & ) = default;

            /// Pre-increment operator.
            self_type& operator++( void ){
                m_ptr++;
                return *this;
            }

            /// Post-increment operator.
            self_type operator++( int ){
                self_type previous_ptr{*this};
                m_ptr++;
                return previous_ptr;
            }

            /// Pre-decrement operator.
            self_type operator--( void ){
                m_ptr--;
                return *this;
            }

            /// Post-decrement operator.
            self_type operator--( int ){
                self_type previous_ptr{*this};
                m_ptr--;
                return previous_ptr;
            }

            self_type& operator+=(difference_type offset){
                m_ptr += offset;
                return *this;
            }
            self_type& operator-=(difference_type offset){
                m_ptr -= offset;
                return *this;
            }

            friend bool operator<(const self_type& ita, const self_type& itb){
                return ita-itb < 0;
            }
            friend bool operator>(const self_type& ita, const self_type& itb){
                return ita-itb > 0;
            }
            friend bool operator>=(const self_type& ita, const self_type& itb){
                return ita-itb >= 0;
            }
            friend bool operator<=(const self_type& ita, const self_type& itb){
                return ita-itb <= 0;
            }

            friend self_type operator+( difference_type offset, self_type it ){
                return self_type{it.m_ptr + offset};
            }
            friend self_type operator+( self_type it, difference_type offset ){
                return self_type{it.m_ptr + offset};
            }
            friend self_type operator-( self_type it, difference_type offset ){
                return self_type{it.m_ptr - offset};
            }

            /// Equality operator.
            bool operator==( const self_type & rhs_ ) const {
                return this->m_ptr == rhs_.m_ptr;
            }

            /// Not equality operator.
            bool operator!=( const self_type & rhs_ ) const {
                return this->m_ptr != rhs_.m_ptr;
            }

            /// Returns the difference between two iterators.
            difference_type operator-( const self_type & rhs_ ) const {
                return std::distance(rhs_.m_ptr, this->m_ptr);
            }

            /// Stream extractor operator.
            friend std::ostream& operator<<( std::ostream& os_, const MyForwardIterator &p_ )
            {
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
    class vector
    {   
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
            explicit vector(size_type count = 0) {
                this->m_capacity = count;
                this->m_end = count;
                this->m_storage.reset(new value_type[this->m_capacity]);
                iterator it = this->begin();
                std::fill(it, it+this->m_capacity, *new value_type);
            }
            virtual ~vector(void) = default;
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
            vector(const std::initializer_list<T>& il) {
                this->m_capacity = il.size();
                this->m_end = il.size();
                this->m_storage.reset(new value_type[this->capacity()]);
                iterator it = this->begin();
                std::copy(il.begin(), il.end(), it);
            }
            template < typename InputItr >
            vector(InputItr first, InputItr last) {
                this->m_capacity = std::distance(first, last);
                this->m_end = this->capacity();
                this->m_storage.reset(new value_type[this->capacity()]);
                iterator it = this->begin();
                std::copy(first, last, it);
            }

            //=== [II] ITERATORS
            iterator begin(void) { return iterator(&this->m_storage[0]); }
            iterator end(void) { return iterator(&this->m_storage[this->size()]); }
            const_iterator cbegin(void) const { return const_iterator(&this->m_storage[0]); }
            const_iterator cend(void) const { return const_iterator(&this->m_storage[this->size()]); }

            // [III] Capacity
            size_type size(void) const { return this->m_end; }
            size_type capacity(void) const { return this->m_capacity; }
            bool empty(void) const { return this->size() == 0; }

            // [IV] Modifiers
            void clear(void) {
                this->m_end = 0; //!< Reset the end index.
                this->m_storage.reset(new value_type[this->capacity()]);
            }

            void push_back(const_reference value) {
                if(this->full()) this->capacity() > 0? this->reserve(this->capacity()*2) : this->reserve(1);
                this->m_storage[this->size()] = value;
                this->m_end++;
            }
            void pop_back(void) { 
                if (this->empty()) throw std::out_of_range("vector::pop_back(): vector is empty");
                this->m_storage[this->size()-1] = *new value_type(0u);
                this->m_end--;
            }
            
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
            template < typename InputItr >
            iterator insert( iterator pos_ , InputItr first_, InputItr last_ ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                if (first_ >= last_) throw std::invalid_argument("vector::insert: invalid input list");
                size_type input_list_size = std::distance(first_, last_);
                size_type offset_pos = pos_-this->begin();

                if (this->capacity() < this->size()+input_list_size) reserve(this->size()+input_list_size);

                std::copy_backward( this->begin() + offset_pos,
                                    this->end(), 
                                    this->end()+ input_list_size);
                std::copy(first_, last_, this->begin()+offset_pos);
                this->m_end += input_list_size;
                return iterator(this->begin()+ offset_pos);
            }
            template < typename InputItr >
            iterator insert( const_iterator pos_ , InputItr first_, InputItr last_ ) {
                if (this->m_storage == nullptr) throw std::runtime_error("vector::insert(): vector is empty");
                if (first_ >= last_) throw std::invalid_argument("vector::insert: invalid input list");
                size_type input_list_size = std::distance(first_, last_);
                size_type offset_pos = pos_-this->begin();

                if (this->capacity() < this->size()+input_list_size) reserve(this->size()+input_list_size);

                std::copy_backward( this->begin() + offset_pos,
                                    this->end(), 
                                    this->end()+ input_list_size);
                std::copy(first_, last_, this->begin()+offset_pos);
                this->m_end += input_list_size;
                return iterator(this->begin()+ offset_pos);
            }
            iterator insert( iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                return this->insert(pos_, ilist_.begin(), ilist_.end());
            }
            iterator insert( const_iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                return this->insert(pos_, ilist_.begin(), ilist_.end());;
            }

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
            
            void assign(size_type count_, const_reference value_) {
                if(count_ > this->capacity()) reserve(count_);
                std::fill(this->begin(), this->begin()+count_, value_);
                this->m_end = count_;
            }
            void assign(const std::initializer_list<T>& ilist) {
                if(ilist.size() > this->capacity()) reserve(ilist.size());
                std::copy(ilist.begin(), ilist.end(), this->begin());
                this->m_end = ilist.size();
            }
            template < typename InputItr >
            void assign(InputItr first, InputItr last) {
                if(std::distance(first, last) > capacity()) reserve(std::distance(first, last));
                std::copy(first, last, this->begin());
                this->m_end = std::distance(first, last);
            }

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
            iterator erase( const_iterator pos ) {
                return this->erase(pos, pos+1);
            }
            iterator erase( iterator pos ) {
                return this->erase(pos, pos+1);
            }

            // [V] Element access

            const_reference front(void) const { return this->at(0); }
            const_reference back(void) const { return this->at(this->size()-1); }
            reference front(void) { return this->at(0); }
            reference back(void) { return this->m_storage[this->size()-1]; }
            //const_reference operator[](size_type pos) const { return this->at(pos); }
            //reference operator[](size_type pos) { return this->at(pos); }
            const_reference operator[](size_type pos) const {return m_storage[pos]; }
            reference operator[](size_type pos) { return m_storage[pos]; }
            const_reference at(size_type pos) const {
                if(pos >= this->m_end) throw std::out_of_range("Index out of bounds.");
                return m_storage[pos];
            }
            reference at(size_type pos) {
                if(pos >= this->m_end) throw std::out_of_range("Index out of bounds.");
                return m_storage[pos];
            }
            pointer data(void) {                // FIXME Descobrir qual é pra retornar
                return this->m_storage.get(); 
            }
            const_reference data(void) const {  // FIXME Descobrir qual é pra retornar
                return this->m_storage; 
            }

            // [VII] Friend functions.
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
    template <typename T>
    bool operator==( const vector<T> &va, const vector<T>&vb ){ return equal(va, vb); }
    template <typename T>
    bool operator!=( const vector<T> &va, const vector<T>&vb ){ return not equal(va, vb); }
    template <typename T>
    bool equal( const vector<T> &va, const vector<T>&vb ){
        if(va.size() != vb.size()) return false;
        for(auto i = 0; i < va.size(); ++i) if(va.at(i) != vb.at(i)) return false;
        return true;
    }
} // namespace sc.

#endif
