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

import heapq
from collections import Counter

class Solution:
    def topKFrequent(self, words: List[str], k: int) -> List[str]:
        counts = Counter(words)
        # Custom class to handle heap comparison logic
        class Element:
            def __init__(self, freq, word):
                self.freq = freq
                self.word = word
            def __lt__(self, other):
                if self.freq == other.freq:
                    return self.word > other.word
                return self.freq < other.freq
        
        heap = []
        for word, freq in counts.items():
            heapq.heappush(heap, Element(freq, word))
            if len(heap) > k:
                heapq.heappop(heap)
        
        res = []
        while heap:
            res.append(heapq.heappop(heap).word)
        return res[::-1]
