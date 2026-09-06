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
    def commonChars(self, words: List[str]) -> List[str]:
        # Initialize freq with the first word
        freq = [0] * 26
        for c in words[0]:
            freq[ord(c) - ord('a')] += 1
        
        # Update freq for each subsequent word
        for word in words[1:]:
            temp_freq = [0] * 26
            for c in word:
                temp_freq[ord(c) - ord('a')] += 1
            for i in range(26):
                freq[i] = min(freq[i], temp_freq[i])
        
        # Construct the result
        result = []
        for i in range(26):
            result.extend([chr(i + ord('a'))] * freq[i])
        return result
