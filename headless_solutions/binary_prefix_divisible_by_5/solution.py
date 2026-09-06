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

from typing import List

class Solution:
    def prefixesDivBy5(self, nums: List[int]) -> List[bool]:
        answer = []  # Initialize the result list
        current_remainder = 0  # Stores the remainder of the current prefix number when divided by 5

        # Iterate through each digit in the binary array
        for num in nums:
            # Update the current_remainder using the modular arithmetic recurrence:
            # (previous_remainder * 2 + current_bit) % 5
            # Multiplying by 2 shifts the binary number left by one position.
            # Adding 'num' appends the new bit.
            # Taking modulo 5 keeps the remainder small and prevents overflow.
            current_remainder = (current_remainder * 2 + num) % 5
            
            # If the remainder is 0, the number is divisible by 5.
            answer.append(current_remainder == 0)
            
        return answer  # Return the list of boolean results
