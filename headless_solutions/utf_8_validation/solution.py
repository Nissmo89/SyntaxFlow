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
    def validUtf8(self, data: List[int]) -> bool:
        remaining = 0
        for val in data:
            if remaining == 0:
                if (val >> 7) == 0: continue
                elif (val >> 5) == 0b110: remaining = 1
                elif (val >> 4) == 0b1110: remaining = 2
                elif (val >> 3) == 0b11110: remaining = 3
                else: return False
            else:
                if (val >> 6) != 0b10: return False
                remaining -= 1
        return remaining == 0
