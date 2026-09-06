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
    def isNumber(self, s: str) -> bool:
        seen_digit, seen_dot, seen_e, number_after_e = False, False, False, True
        for i, char in enumerate(s):
            if char.isdigit():
                seen_digit = True
                number_after_e = True
            elif char == '.':
                if seen_dot or seen_e: return False
                seen_dot = True
            elif char in ('e', 'E'):
                if seen_e or not seen_digit: return False
                seen_e = True
                number_after_e = False
            elif char in ('+', '-'):
                if i > 0 and s[i-1] not in ('e', 'E'): return False
            else:
                return False
        return seen_digit and number_after_e
