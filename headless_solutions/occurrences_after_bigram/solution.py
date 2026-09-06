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
    def findOcurrences(self, text: str, first: str, second: str) -> List[str]:
        ans, words = [], text.split()
        for a, b, c in zip(words, words[1:], words[2:]):
            if (a, b) == (first, second):
                ans.append(c)
        return ans
