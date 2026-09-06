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
    def strWithout3a3b(self, a: int, b: int) -> str:
        res = []
        while a > 0 or b > 0:
            # Check if we are forced to pick a specific character
            if len(res) >= 2 and res[-1] == res[-2]:
                if res[-1] == 'a':
                    res.append('b')
                    b -= 1
                else:
                    res.append('a')
                    a -= 1
            # Otherwise, pick the one with more remaining
            elif a >= b:
                res.append('a')
                a -= 1
            else:
                res.append('b')
                b -= 1
        return "".join(res)
