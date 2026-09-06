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
    def myAtoi(self, s: str) -> int:
        s = s.lstrip()
        if not s: return 0
        
        sign = 1
        i = 0
        if s[0] in ('-', '+'):
            sign = -1 if s[0] == '-' else 1
            i = 1
            
        res = 0
        while i < len(s) and s[i].isdigit():
            res = res * 10 + int(s[i])
            i += 1
            
        res *= sign
        return max(min(res, 2**31 - 1), -2**31)
