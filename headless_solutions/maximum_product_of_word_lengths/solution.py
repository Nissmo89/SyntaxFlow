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
    def maxProduct(self, words: List[str]) -> int:
        n = len(words)
        masks = [0] * n
        for i in range(n):
            for char in words[i]:
                masks[i] |= (1 << (ord(char) - ord('a')))
        
        max_prod = 0
        for i in range(n):
            for j in range(i + 1, n):
                if (masks[i] & masks[j]) == 0:
                    max_prod = max(max_prod, len(words[i]) * len(words[j]))
        return max_prod
