/**
 * C++ Standard-Template-Library (STL) Data Structure and Variable Reference
 * Please refer to the following websites for more detailed documentations regarding:
 * 	- `std::vector`       : http://www.cplusplus.com/reference/vector/vector/
 * 	- `std::unordered_map`: http://www.cplusplus.com/reference/unordered_map/unordered_map/
 * 	- More C++ Containers : http://www.cplusplus.com/reference/stl/
 */ 	
#include <vector>         // List
#include <unordered_map>  // Hash Table
#include <string>         // String
#include <iostream>       // Print Statements


int main()
{
        // =====================================================================
        // std::vector
        // =====================================================================
        /**
         * `std` is the standard namespace. It is used to avoid name clashes of
         * classes, variables, and functions. Classes/Variables/Functions that
         * are supported by the C++ standard library belong to this namespace.
         * 
         * `vector` is the list-like data structure in STL. It is a
         * *template class*. A template class can be *instantiated* for various
         * types. In the example below, `< unsigned (<- typename) >` is an
         * instantiation of the `vector` template class (for unsigned integer).
         * Similarly, we can create another array of floating point values using
         * the statement: `std::vector < float > b = { 1.0, 2.0, 3.0 };`.
         */
        std::vector < unsigned > a = { 1, 2, 3, 4, 5 };

        /**
         * Traverse through `a` using *iterators*. 
         * `std::vector < unsigned > ::iterator` is the typename for the
         * iterators. If you find it tedious, you can let the compiler
         * automatically infer the typename for you:
         * 
         *     for (auto iter = a.begin(); ...)
         */
        for (std::vector < unsigned > ::iterator iter = a.begin();
             iter != a.end(); ++iter)
        {
                // dereference the iterator the obtain the array value.
                std::cout << *iter << ", ";
        }
        std::cout << std::endl;  // newline character
        // =====================================================================
        // std::unordered_map
        // =====================================================================
        /**
         * `unordered_map` is the dictionary-like data structure in STL. We can
         * see that it is taking two template arguments (i.e., `string` and
         * `unsigned`). The first argument is the key type and the second one is
         * the value type (hence `b` is a dictionary that uses string as the key
         * and unsigned integer as the value).
         */
        std::unordered_map < std::string, unsigned > b = 
                {
                        {"Red"  , 0}, 
                        {"Green", 1},
                        {"Blue" , 2}
                };
        
        for (std::unordered_map < std::string, unsigned > ::iterator iter = b.begin();
             iter != b.end(); ++iter)
        {
                // When we dereference the iterator, what we get is a
                // *reference* to a *pair* object (we will illustrate an example
                // on the reference concept soon). The first element of the pair
                // reference is the *constant* key, and the second element is
                // the value corresponding to the key.
                std::pair < const std::string, unsigned > & key_value_pair = *iter;

                std::cout << "(" << key_value_pair.first << ", "
                          << key_value_pair.second << "), ";
                // Alternatively, we can also write:
                //
                //     std::cout << ... iter->first ... iter->second ...
        }
        std::cout << std::endl; // newline character

        // =====================================================================
        // Variable Reference
        // =====================================================================

        int x = 100; 
        int * ptr_x = &x;  // `ptr_x` is a pointer to `x`.
        int & ref_x =  x;  // `ref_x` is a *reference* to `x`
        int copy_x = x;    // `copy_x` is a copy of `x`.

        x = 50;  // When we modify the value of `x`, we are modifying the value
                 // of `ref_x` and the value that `ptr_x` is pointing to and at
                 // the same time, but not the value of `copy_x`.
        std::cout << "x: " << x << ", "
                  << "ptr_x: " << *ptr_x << ", "
                  << "ref_x: " <<  ref_x << ", "
                  << "copy_x: " << copy_x << std::endl;
        // Note that we do NOT need to "take the address of" and/or
        // "dereference" when dealing when references, which make them a cleaner
        // solution when compared with pointers.
        return 0;
}
