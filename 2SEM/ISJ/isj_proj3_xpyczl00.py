#!/usr/bin/env python3

import re

# ukol za 2 body
def first_odd_or_even(numbers):
    """Returns 0 if there is the same number of even numbers and odd numbers
       in the input list of ints, or there are only odd or only even numbers.
       Returns the first odd number in the input list if the list has more even
       numbers.
       Returns the first even number in the input list if the list has more odd 
       numbers.

    >>> first_odd_or_even([2,4,2,3,6])
    3
    >>> first_odd_or_even([3,5,4])
    4
    >>> first_odd_or_even([2,4,3,5])
    0
    >>> first_odd_or_even([2,4])
    0
    >>> first_odd_or_even([3])
    0
    """
    odd_array=[]
    even_array=[]
    odd=0
    even=0
    for value in numbers:
        if (value%2==0):
            even+=1
            even_array.append(value)
        else:
            odd+=1
            odd_array.append(value)
    if odd==even:
        return 0
    elif odd!=0 and even==0:
        return 0
    
    elif even!=0 and odd==0:
        return 0
    

    elif even>odd:
        return odd_array[0] 
    
    elif odd>even:
        return even_array[0]

# ukol za 3 body
def to_pilot_alpha(word):
    """Returns a list of pilot alpha codes corresponding to the input word

    >>> to_pilot_alpha('Smrz')
    ['Sierra', 'Mike', 'Romeo', 'Zulu']
    """

    pilot_alpha = ['Alfa', 'Bravo', 'Charlie', 'Delta', 'Echo', 'Foxtrot',
        'Golf', 'Hotel', 'India', 'Juliett', 'Kilo', 'Lima', 'Mike',
        'November', 'Oscar', 'Papa', 'Quebec', 'Romeo', 'Sierra', 'Tango',
        'Uniform', 'Victor', 'Whiskey', 'Xray', 'Yankee', 'Zulu']

    pilot_alpha_list = []
    
    for letter in word:
        for code in pilot_alpha:
            if code[0].lower()==letter or code[0]==letter:
                pilot_alpha_list.append(code)


    return pilot_alpha_list


if __name__ == "__main__":
    import doctest
    doctest.testmod()