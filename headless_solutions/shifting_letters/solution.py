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
    def shiftingLetters(self, s: str, shifts: List[int]) -> str:
        res = []
        current_shift = 0
        # Process from right to left to accumulate suffix sums
        for i in range(len(s) - 1, -1, -1):
            current_shift = (current_shift + shifts[i]) % 26
            # Calculate new character
            char_code = (ord(s[i]) - ord('a') + current_shift) % 26
            res.append(chr(char_code + ord('a')))
        
        return "".join(res[::-1])
