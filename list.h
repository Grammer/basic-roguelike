#ifndef LIST_H
#define LIST_H

#include <cstdlib>
#include <cstring>

template< class T > class List
{
    T *array;
    int t_iFillSize, t_iAllocSize;

public:
    // Creates an empty list
    List()
    {
        array = NULL;
        t_iFillSize = t_iAllocSize = 0;
    }

    // Delete list by freeing any allocated resources
    // Note that deleting the list doesn't delete it's elements!
    // Use DeleteAll() *BEFORE* deleting the list if you want to destoy the elements
    virtual ~List()
    {
        if ( array )
            delete []array;
    }

    // These are used to iterate through elements of the list using an iterator
    // Note that you *CANNOT* insert elements in the list while iterating through it!
    // Inserting elements can result in reallocation of the list and iterator won't be valid anymore
    // Begin() returns the adress of the first element of the list
    T* Begin( void ) const
    {
        if ( t_iFillSize == 0 )
            return (T*)NULL;
        return &array[0];
    }

    // End() returns the adress of the last element of the list
    T* End( void ) const
    {
        if ( t_iFillSize == 0 )
            return (T*)NULL;
        return &array[t_iFillSize];
    }

    // Remove element from the list
    T* Remove( T* receivedElement )
    {
        for ( T* curElement = receivedElement; curElement < End() - 1; curElement++ )
            *curElement = *( curElement + 1 );
        t_iFillSize--;
        if ( t_iFillSize == 0 )
            return ((T *)NULL) - 1;
        else
            return receivedElement - 1;
    }

    // Pushing an element on the stack
    void Push( const T pushElement )
    {
        if ( t_iFillSize + 1 >= t_iAllocSize )
            Allocate();
        array[t_iFillSize++] = pushElement;
    }

    // Poping an element from the stack
    T* Pop( void )
    {
        if ( t_iFillSize == 0 )
            return (T*)0;
        return &array[--t_iFillSize];
    }

    // You can assign a new value on index
    // If needed, the array will allocate new elements up to index
    void Set( const T setElement, int index )
    {
        if ( index < 0 )
            return;
        while ( t_iAllocSize < index + 1 )
            Allocate();
        array[index] = setElement;
        if ( index + 1 > t_iFillSize )
            t_iFillSize = index + 1;
    }

    // Get element value
    T Get( int index ) const
    {
        return array[index];
    }

    // Emptying a list and destroying its elements
    void DeleteAll( void )
    {
        for ( T *curElement = Begin(); curElement != End(); curElement++ )
            delete (*curElement);
        t_iFillSize = 0;
    }

    List< T > & operator = ( List< T > const & list )
    {
        while ( t_iAllocSize < list.t_iAllocSize )
            Allocate();

        int i = 0;
        for ( T *t = list.Begin(); t != list.End(); t++ )
            array[i++] = *t;

        return *this;
    }

protected:
    void Allocate( void )
    {
        int m_iNewSize = t_iAllocSize * 2;
        if ( m_iNewSize == 0 )
            m_iNewSize = 10;
        T* t_pNewArray = new T[m_iNewSize];
        if ( array )
        {
            if ( t_iFillSize > 0 )
                memcpy( t_pNewArray, array, sizeof( T ) * t_iFillSize );
            delete []array;
        }
        array = t_pNewArray;
        t_iAllocSize = m_iNewSize;
    }
};

#endif // ENEMY_LIST_H
