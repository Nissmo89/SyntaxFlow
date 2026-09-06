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

from collections import defaultdict

class Solution:
    def numMatchingSubseq(self, s: str, words: List[str]) -> int:
        waiting = defaultdict(list)
        for w in words:
            it = iter(w)
            waiting[next(it)].append(it)
        
        count = 0
        for char in s:
            # Get all iterators waiting for this char
            to_process = waiting.pop(char, [])
            for it in to_process:
                nxt = next(it, None)
                if nxt is None:
                    count += 1
                else:
                    waiting[nxt].append(it)
        return count
