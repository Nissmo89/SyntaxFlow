import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def addBinary(self, a: str, b: str) -> str:
        result = []  # Use a list to build the result characters
        i, j = len(a) - 1, len(b) - 1
        carry = 0

        # Loop while there are digits in either string or a carry exists
        while i >= 0 or j >= 0 or carry:
            current_sum = carry  # Start sum with the carry from the previous step

            # Add digit from string 'a' if available
            if i >= 0:
                current_sum += int(a[i]) # Convert char '0'/'1' to int 0/1
                i -= 1 # Move to the next digit in 'a'
            
            # Add digit from string 'b' if available
            if j >= 0:
                current_sum += int(b[j]) # Convert char '0'/'1' to int 0/1
                j -= 1 # Move to the next digit in 'b'
            
            # Append the current digit (current_sum % 2) to the result
            result.append(str(current_sum % 2)) # Convert int 0/1 back to char '0'/'1'

            # Update carry for the next iteration
            carry = current_sum // 2
        
        # The result is built in reverse order, so reverse it and join
        return "".join(result[::-1])
