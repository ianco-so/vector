#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <exception>    // std::out_of_range
#include <iostream>     // std::cout, std::endl
#include <memory>       // std::unique_ptr
#include <iterator>     // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <algorithm>    // std::copy, std::equal, std::fill
#include <initializer_list> // std::initializer_list
#include <cassert>      // assert()
#include <limits>       // std::numeric_limits<T>
#include <cstddef>      // std::size_t

/// Sequence container namespace.
namespace sc {
    /// Implements tha infrastructure to support a bidirectional iterator.
    template < class T >
    class MyForwardIterator : public std::iterator<std::bidirectional_iterator_tag, T>{
        public:
            typedef MyForwardIterator self_type;   //!< Alias to iterator.
            // Below we have the iterator_traits common interface
            typedef std::ptrdiff_t difference_type; //!< Difference type used to calculated distance between iterators.
            typedef T value_type;           //!< Value type the iterator points to.
            typedef T* pointer;             //!< Pointer to the value type.
            typedef T& reference;           //!< Reference to the value type.
            typedef const T& const_reference;           //!< Reference to the value type.
            typedef std::bidirectional_iterator_tag iterator_category; //!< Iterator category.

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
                return ita < itb;
            }
            friend bool operator>(const self_type& ita, const self_type& itb){
                return ita > itb;
            }
            friend bool operator>=(const self_type& ita, const self_type& itb){
                return ita >= itb;
            }
            friend bool operator<=(const self_type& ita, const self_type& itb){
                return ita <= itb;
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
                return std::distance(this->m_ptr, rhs_.m_ptr);
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
            using size_type = unsigned long; //!< The size type.
            using value_type = T;            //!< The value type.
            using pointer = value_type*;     //!< Pointer to a value stored in the container.
            using reference = value_type&;   //!< Reference to a value stored in the container.
            using const_reference = const value_type&; //!< Const reference to a value stored in the container.

            using iterator = MyForwardIterator< value_type >; //!< The iterator, instantiated from a template class.
            using const_iterator = MyForwardIterator< const value_type >; //!< The const_iterator, instantiated from a template class.

        public:
            //=== [I] SPECIAL MEMBERS (6 OF THEM)
            explicit vector(size_type count = 0) {
                m_capacity = count;
                m_end = count;
                m_storage.reset(new value_type[m_capacity]);
                iterator it = m_storage.get();
                std::fill(it, it+m_capacity, *new value_type);
            };
            virtual ~vector(void) = default;
            vector(const vector& other) {
                m_capacity = other.m_capacity;
                m_end = other.m_end;
                m_storage.reset(new value_type[m_capacity]);
                const_iterator other_it = other.m_storage.get();
                iterator it_this = m_storage.get();
                std::copy(other_it, other_it+m_end, it_this);
            }
            vector(const std::initializer_list<T>& il) {
                m_capacity = il.size();
                m_end = il.size();
                m_storage.reset(new value_type[m_capacity]);
                iterator it = m_storage.get();
                std::copy(il.begin(), il.end(), it);
            }
            vector(vector&& other) {
                m_capacity = other.m_capacity;
                m_end = other.m_end;
                m_storage.reset(new value_type[m_capacity]);
                const_iterator other_it = other.m_storage.get();
                iterator it_this = m_storage.get();
                std::copy(other_it, other_it+m_end, it_this);
            }
            template < typename InputItr >
            vector(InputItr first, InputItr last) {
                m_capacity = std::distance(first, last);
                m_end = std::distance(first, last);
                m_storage.reset(new value_type[m_capacity]);
                iterator it = m_storage.get();
                std::copy(first, last, it);
            }

            vector& operator=(const vector& rhs) {
                if(this != &rhs) {
                    m_capacity = rhs.m_capacity;
                    m_end = rhs.m_end;
                    m_storage.reset(new value_type[m_capacity]);
                    const_iterator rhs_it = rhs.m_storage.get();
                    iterator it_this = m_storage.get();
                    std::copy(rhs_it, rhs_it+m_end, it_this);
                }
                return *this;
            }
            vector& operator=(vector&& rhs) {
                if(this != &rhs) {
                    m_capacity = rhs.m_capacity;
                    m_end = rhs.m_end;
                    m_storage.reset(new value_type[m_capacity]);
                    const_iterator rhs_it = rhs.m_storage.get();
                    iterator it_this = m_storage.get();
                    std::copy(rhs_it, rhs_it+m_end, it_this);
                }
                return *this;
            }

            //=== [II] ITERATORS
            iterator begin(void) {
                return iterator(&m_storage[0]);
            }
            iterator end(void) {
                return iterator(&m_storage[m_end]);
            }
            const_iterator cbegin(void) const {
                return const_iterator(&m_storage[0]);
            }
            const_iterator cend(void) const {
                return const_iterator(&m_storage[m_end]);
            }

            // [III] Capacity
            size_type size(void) const {
                return m_end;
            }
            size_type capacity(void) const {
                return m_capacity;
            }
            bool empty(void) const {
                return m_end == 0;
            }

            // [IV] Modifiers
            void clear(void) {
                m_end = 0;
            }
            void push_front( const_reference ) {/* empty */}
            void push_back(const_reference value) {
                if(!full()) {
                    m_storage[m_end++] = value;
                }
                else {
                    size_type new_capacity = m_capacity == 0 ? 1 : 2*m_capacity;
                    reserve(new_capacity);
                    m_storage[m_end++] = value;
                }
            }
            void pop_back(void) {
                if(not empty()) {
                    m_end--;
                }
            }
            void pop_front( void ) {/* empty */}

            iterator insert( iterator pos_ , const_reference value_ ) {
                 // TODO
                return iterator(new value_type);
            }
            iterator insert( const_iterator pos_ , const_reference value_ ) {
                 // TODO
                return iterator(new value_type);
            }

            template < typename InputItr >
            iterator insert( iterator pos_ , InputItr first_, InputItr last_ ) {
                 // TODO
                return iterator(new value_type);
            }
            template < typename InputItr >
            iterator insert( const_iterator pos_ , InputItr first_, InputItr last_ ) {
                 // TODO
                return iterator(new value_type);
            }

            iterator insert( iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                 // TODO
                return iterator(new value_type);
            }
            iterator insert( const_iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                 // TODO
                return iterator(new value_type);
            }

            void reserve(size_type new_capacity) {
                // FIXME Lógica com erro
                // if(new_capacity > capacity()) {
                //     vector vec(new_capacity);
                //     std::copy(this->begin(), this->end(), vec);
                //     delete[] m_storage.get();
                //     *this = vec;
                // }
            }
            void shrink_to_fit(void) {
                m_capacity = m_end;
            }

            void assign(size_type count_, const_reference value_) {
                if(count_ > capacity()) {
                    size_type new_capacity = m_capacity == 0 ? 1 : 2*m_capacity;
                    reserve(new_capacity);
                }
                std::fill(begin(), begin()+count_, value_);
            }
            void assign(const std::initializer_list<T>& ilist) {
                if(ilist.size() > capacity()) {
                    size_type new_capacity = m_capacity == 0 ? 1 : 2*m_capacity;
                    reserve(new_capacity);
                }
                std::copy(ilist.begin(), ilist.end(), m_storage);
            }
            template < typename InputItr >
            void assign(InputItr first, InputItr last) {
                if(std::distance(first, last) > capacity()) {
                    size_type new_capacity = m_capacity == 0 ? 1 : 2*m_capacity;
                    reserve(new_capacity);
                }
                std::copy(first, last, m_storage);
            }

            iterator erase( iterator first, iterator last ) {
                // TODO
                return iterator(new value_type);
            }
            iterator erase( const_iterator first, const_iterator last ) {
                // TODO
                return iterator(new value_type);
            }

            iterator erase( const_iterator pos ) {
                // TODO
                return iterator(new value_type);
            }
            iterator erase( iterator pos ) {
                // TODO
                return iterator(new value_type);
            }

            // [V] Element access
            const_reference back(void) const {
                return m_storage[m_end];
            }
            const_reference front(void) const {
                return m_storage[0];
            }
            reference back(void) {
                return m_storage[m_end];
            }
            reference front(void) {
                return m_storage[0];
            }
            const_reference operator[](size_type pos) const {
                return m_storage[pos];
            }
            reference operator[](size_type pos) {
                return m_storage[pos];
            }
            const_reference at(size_type pos) const {
                if(pos > m_end) {
                    throw std::out_of_range("Index out of bounds.");
                }
                return m_storage[pos];
            }
            reference at(size_type pos) {
                if(pos > m_end) {
                    throw std::out_of_range("Index out of bounds.");
                }
                return m_storage[pos];
            }
            pointer data(void) {
                return m_storage.get(); // FIXME Descobrir qual é pra retornar
            }
            const_reference data(void) const {
                return m_storage; // FIXME Descobrir qual é pra retornar
            }

            // [VII] Friend functions.
            friend std::ostream & operator<<(std::ostream & os_, const vector<T> & v_) {
                // O que eu quero imprimir???
                os_ << "{ ";
                for( auto i{0u} ; i < v_.m_capacity ; ++i )
                {
                    if ( i == v_.m_end ) os_ << "| ";
                    os_ << v_.m_storage[ i ] << " ";
                }
                os_ << "}, m_end=" << v_.m_end << ", m_capacity=" << v_.m_capacity;

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

        private:
            bool full(void) const {
                return m_end == m_capacity;
            }

            size_type m_end;                //!< The list's current size (or index past-last valid element).
            size_type m_capacity;           //!< The list's storage capacity.
            std::unique_ptr<T[]> m_storage; //!< The list's data storage area.
    };

    // [VI] Operators
    template <typename T>
    bool operator==(const vector<T>& va, const vector<T>& vb) {
        if(va.size() != vb.size()) {
            return false;
        }
        for(auto i = 0; i < (int) va.size(); ++i) {
            if(va.at(i) != vb.at(i)) {
                return false;
            }
        }
        return true;
    }
    template <typename T>
    bool operator!=(const vector<T>& va, const vector<T>& vb) {
        if(va.size() != vb.size()) {
            return true;
        }
        for(auto i = 0; i < (int) va.size(); ++i) {
            if(va.at(i) != vb.at(i)) {
                return true;
            }
        }
        return false;
    }

} // namespace sc.

#endif
