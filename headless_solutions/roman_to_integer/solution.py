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
    def romanToInt(self, s: str) -> int:
        m = {
            "I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000
        }
        ans = 0
        for i in range(len(s)):
            if i > 0 and m[s[i]] > m[s[i - 1]]:
                ans += m[s[i]] - 2 * m[s[i - 1]]
            else:
                ans += m[s[i]]
        return ans
