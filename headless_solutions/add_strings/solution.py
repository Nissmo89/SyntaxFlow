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
    def addStrings(self, num1: str, num2: str) -> str:
        result = [] # Use a list of characters to build the result
        i = len(num1) - 1
        j = len(num2) - 1
        carry = 0

        # Loop until both numbers are processed and there's no carry left
        while i >= 0 or j >= 0 or carry:
            digit1 = int(num1[i]) if i >= 0 else 0
            digit2 = int(num2[j]) if j >= 0 else 0

            current_sum = digit1 + digit2 + carry
            result.append(str(current_sum % 10)) # Append the digit as a string
            carry = current_sum // 10 # Integer division for carry

            i -= 1 # Move to the next digit in num1
            j -= 1 # Move to the next digit in num2
        
        # The result is built in reverse order, so reverse the list and join
        return "".join(result[::-1])
