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
    def baseNeg2(self, n: int) -> str:
        if n == 0:
            return "0"
        
        digits = []
        while n != 0:
            remainder = n & 1
            digits.append(str(remainder))
            n = (n - remainder) // -2
            
        return "".join(reversed(digits))
