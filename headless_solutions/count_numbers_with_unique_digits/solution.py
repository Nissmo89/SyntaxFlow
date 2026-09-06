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
    def countNumbersWithUniqueDigits(self, n: int) -> int:
        # Base case for 10^0 = 1, range is 0 <= x < 1 (only x = 0)
        if n == 0:
            return 1
        
        total = 10            # Base answer for n = 1 (digits 0 through 9)
        unique_digits = 9     # Unique choices for the first digit (1..9)
        available_options = 9 # Available digits for subsequent positions
        
        for _ in range(2, n + 1):
            if available_options == 0:
                break
            unique_digits *= available_options
            total += unique_digits
            available_options -= 1
            
        return total
