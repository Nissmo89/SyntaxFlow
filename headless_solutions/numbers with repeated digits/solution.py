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
    def numDupDigitsAtMostN(self, n: int) -> int:
        s = str(n)
        l = len(s)
        
        def P(n, k):
            import math
            return math.perm(n, k)
        
        count = 0
        # Numbers with fewer digits
        for i in range(1, l):
            count += 9 * P(9, i - 1)
            
        # Numbers with same number of digits
        used = [False] * 10
        for i in range(l):
            digit = int(s[i])
            for j in range(1 if i == 0 else 0, digit):
                if not used[j]:
                    count += P(9 - i, l - 1 - i)
            if used[digit]:
                return n - count
            used[digit] = True
            
        return n - (count + 1)
