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
    def integerReplacement(self, n: int) -> int:
        count = 0
        while n > 1:
            if n % 2 == 0:
                n //= 2
            elif n == 3 or (n & 2) == 0:
                # n & 2 checks the second-to-last bit
                n -= 1
            else:
                n += 1
            count += 1
        return count
