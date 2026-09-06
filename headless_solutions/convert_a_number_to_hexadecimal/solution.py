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
    def toHex(self, num: int) -> str:
        if num == 0:
            return "0"
        mp = '0123456789abcdef'
        ans = ''
        if num < 0:
            num += 2**32
        while num > 0:
            n = num & 15
            c = mp[n]
            ans = c + ans
            num = num >> 4
        return ans
