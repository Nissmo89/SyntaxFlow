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
    def monotoneIncreasingDigits(self, n: int) -> int:
        s = list(str(n))
        marker = len(s)
        for i in range(len(s) - 1, 0, -1):
            if s[i - 1] > s[i]:
                s[i - 1] = chr(ord(s[i - 1]) - 1)
                marker = i
        
        for i in range(marker, len(s)):
            s[i] = '9'
            
        return int("".join(s))
