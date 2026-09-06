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
    def smallestSubsequence(self, s: str) -> str:
        last = {c: i for i, c in enumerate(s)}
        stack = []
        seen = set()
        for i, c in enumerate(s):
            if c in seen: continue
            while stack and stack[-1] > c and i < last[stack[-1]]:
                seen.remove(stack.pop())
            stack.append(c)
            seen.add(c)
        return "".join(stack)
