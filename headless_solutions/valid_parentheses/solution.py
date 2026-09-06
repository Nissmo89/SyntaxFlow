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
    def isValid(self, s: str) -> bool:
        stack = []
        for c in s:
            if c in ['(', '{', '[']:
                stack.append(c)
            elif c in [')', '}', ']']:
                if not stack:
                    return False
                top = stack.pop()
                if (c == ')' and top != '(') or (c == '}' and top != '{') or (c == ']' and top != '['):
                    return False
        return not stack
