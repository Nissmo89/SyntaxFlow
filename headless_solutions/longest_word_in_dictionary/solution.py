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
    def longestWord(self, words: List[str]) -> str:
        words.sort()
        built = set()
        res = ""
        for w in words:
            if len(w) == 1 or w[:-1] in built:
                if len(w) > len(res):
                    res = w
                built.add(w)
        return res
