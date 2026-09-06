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
    def fractionToDecimal(self, numerator: int, denominator: int) -> str:
        if numerator == 0: return "0"
        res = []
        if (numerator < 0) ^ (denominator < 0): res.append("-")
        
        n, d = abs(numerator), abs(denominator)
        res.append(str(n // d))
        rem = n % d
        if rem == 0: return "".join(res)
        
        res.append(".")
        lookup = {}
        while rem != 0:
            if rem in lookup:
                res.insert(lookup[rem], "(")
                res.append(")")
                break
            lookup[rem] = len(res)
            rem *= 10
            res.append(str(rem // d))
            rem %= d
        return "".join(res)
