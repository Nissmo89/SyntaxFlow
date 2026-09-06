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
    def findNthDigit(self, n: int) -> int:
        length, count, start = 1, 9, 1
        while n > length * count:
            n -= length * count
            length += 1
            count *= 10
            start *= 10
        
        num = start + (n - 1) // length
        return int(str(num)[(n - 1) % length])
