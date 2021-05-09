/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){
    // front
    n1 = 0;
    // back 
    n2 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T left = data[n1];
    // data.erase(n1);
    n1++;


    if (n2-n1 <= n1) {
        vector<T> newVector;
        for (int i = n1; i < n2; i++) {
            newVector.push_back(data[i]);
        }
        // delete(data);
        data = newVector;
        n2 = data.size();
        n1 = 0;
    }
    return left;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    
    T back = data[n2-1];
    data.pop_back();
    n2--;

    if (n2-n1 <= n1) {
        vector<T> newVector;
        for (int i = n1; i < n2; i++) {
            newVector.push_back(data[i]);
        }
        // free(data);
        data = newVector;
        n2 = data.size();
        n1 = 0;
    }

    return back;
}


/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    // T front = data[n1];
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    // T back = data[n2-1];
    return data[n2-1];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n1 == n2;
}
