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
    def minimumLengthEncoding(self, words: List[str]) -> int:
        # Use a set for O(1) lookups and removals
        word_set = set(words)
        for w in words:
            # Remove all proper suffixes of w from the set
            for i in range(1, len(w)):
                word_set.discard(w[i:])
        
        # Each remaining word contributes its length + 1 (for '#')
        return sum(len(w) + 1 for w in word_set)
