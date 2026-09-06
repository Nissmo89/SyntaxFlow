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
    def primePalindrome(self, n: int) -> int:
        def is_prime(num):
            if num < 2: return False
            for i in range(2, int(num**0.5) + 1):
                if num % i == 0: return False
            return True

        if 8 <= n <= 11: return 11
        for length in range(1, 10):
            if length % 2 == 0 and length != 2: continue
            
            start = 10**((length - 1) // 2)
            end = 10**((length + 1) // 2)
            for i in range(start, end):
                s = str(i)
                res = int(s + s[:-1][::-1] if length % 2 else s + s[::-1])
                if res >= n and is_prime(res):
                    return res
        return -1
