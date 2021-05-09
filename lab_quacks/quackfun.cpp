/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

namespace QuackFun {

    /**
     * Sums items in a stack.
     * @param s A stack holding values to sum.
     * @return The sum of all the elements in the stack, leaving the original
     *  stack in the same state (unchanged).
     *
     * @note You may modify the stack as long as you restore it to its original
     *  values.
     * @note You may use only two local variables of type T in your function.
     *  Note that this function is templatized on the stack's type, so stacks of
     *  objects overloading the + operator can be summed.
     * @note We are using the Standard Template Library (STL) stack in this
     *  problem. Its pop function works a bit differently from the stack we
     *  built. Try searching for "stl stack" to learn how to use it.
     * @hint Think recursively!
     */
    template <typename T>
    T sum(stack<T>& s) {
        if (s.empty()) {
            return T();
        }

        T value = s.top();
        s.pop();
        T sumValue = value + sum(s);
        s.push(value);

        return sumValue; // stub return value (0 for primitive types). Change this!
                    // Note: T() is the default value for objects, and 0 for
                    // primitive types
    }

    /**
     * Reverses even sized blocks of items in the queue. Blocks start at size
     * one and increase for each subsequent block.
     * @param q A queue of items to be scrambled
     *
     * @note Any "leftover" numbers should be handled as if their block was
     *  complete.
     * @note We are using the Standard Template Library (STL) queue in this
     *  problem. Its pop function works a bit differently from the stack we
     *  built. Try searching for "stl stack" to learn how to use it.
     * @hint You'll want to make a local stack variable.
     */
    template <typename T>
    void scramble(queue<T>& q)
    {
        stack<T> s;
        // optional: queue<T> q2;
        int n = 1;
        //int sumValue = 1;
        int sizeq = q.size();
        while (sum(n) < sizeq) {
            if (n % 2 == 0) {
                for (int i = 0; i < n; i++) {
                    T curr = q.front();
                    s.push(curr);
                    q.pop();

                }
                for (int i = 0; i < n; i++) {
                    T temp = s.top();
                    q.push(temp);
                    s.pop();
                }
            } else {
                for (int i = 0; i < n; i++) {
                    T curr = q.front();
                    q.pop();
                    q.push(curr);
                }
            }
            n++;
            //sumValue = sum(n);
        }

        int leftover = sizeq - sum(n-1);
        if (n % 2 == 0) {
            for (int i = 0; i < leftover; i++) {
                T curr = q.front();
                s.push(curr);
                q.pop();
            }
            for (int i = 0; i < leftover; i++) {
                T temp = s.top();
                q.push(temp);
                s.pop();
            }
        } else {
            for (int i = 0; i < leftover; i++) {
                T curr = q.front();
                q.pop();
                q.push(curr);
            }
        }
    }

    //helper function
    int sum(int n) {
        if (n == 1) {
            return 1;
        } else if (n == 0) {
            return 0;
        }
        return n+sum(n-1);
    }
    /**
     * @return true if the parameter stack and queue contain only elements of
     *  exactly the same values in exactly the same order; false, otherwise.
     *
     * @note You may assume the stack and queue contain the same number of items!
     * @note There are restrictions for writing this function.
     * - Your function may not use any loops
     * - In your function you may only declare ONE local boolean variable to use in
     *   your return statement, and you may only declare TWO local variables of
     *   parametrized type T to use however you wish.
     * - No other local variables can be used.
     * - After execution of verifySame, the stack and queue must be unchanged. Be
     *   sure to comment your code VERY well.
     */
    template <typename T>
    bool verifySame(stack<T>& s, queue<T>& q)
    {
        bool retval = true; // optional

        if (s.size() == 1) {
            T tempStack = s.top(); 
            T tempQueue = q.front();
            if (tempStack != tempQueue) {
                retval = false;
            }
            q.pop();
            q.push(tempQueue);
            return retval;
        }

        T tempStack = s.top(); 
        s.pop();
        
        retval = retval && verifySame(s,q);

        T tempQueue = q.front(); 

        if (tempStack != tempQueue) {
            retval = false;
        }

        q.pop();
        q.push(tempQueue);
        s.push(tempStack);

        return retval;
    }
}
