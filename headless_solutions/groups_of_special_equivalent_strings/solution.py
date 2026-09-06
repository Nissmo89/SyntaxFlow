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

from typing import List

class Solution:
    def numSpecialEquivGroups(self, words: List[str]) -> int:
        signatures = set()
        for w in words:
            even = sorted(w[0::2])   # characters at even indices
            odd  = sorted(w[1::2])   # characters at odd indices
            # join lists into a single string; use a delimiter to avoid ambiguity
            sig = ''.join(even) + '#' + ''.join(odd)
            signatures.add(sig)
        return len(signatures)
