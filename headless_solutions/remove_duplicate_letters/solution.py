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
    def removeDuplicateLetters(self, s: str) -> str:
        count = {c: s.count(c) for c in set(s)}
        stack = []
        visited = set()
        
        for char in s:
            count[char] -= 1
            if char not in visited:
                while stack and char < stack[-1] and count[stack[-1]] > 0:
                    visited.remove(stack.pop())
                stack.append(char)
                visited.add(char)
        return "".join(stack)
