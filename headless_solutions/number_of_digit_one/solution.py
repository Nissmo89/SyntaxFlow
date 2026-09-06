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
    def countDigitOne(self, n: int) -> int:
        count = 0
        m = 1
        while m <= n:
            a, x, b = n // (m * 10), (n // m) % 10, n % m
            count += a * m
            if x > 1:
                count += m
            elif x == 1:
                count += (b + 1)
            m *= 10
        return count
