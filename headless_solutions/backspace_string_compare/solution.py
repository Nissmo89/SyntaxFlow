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
    def backspaceCompare(self, s: str, t: str) -> bool:
        def get_next_char(string, pointer):
            """Helper function to find the next effective character and its new pointer position."""
            skip_count = 0
            while pointer >= 0:
                if string[pointer] == '#':
                    skip_count += 1
                    pointer -= 1
                elif skip_count > 0:
                    skip_count -= 1
                    pointer -= 1
                else:
                    # Found a valid character
                    break
            return pointer

        ptr_s = len(s) - 1
        ptr_t = len(t) - 1

        while ptr_s >= 0 or ptr_t >= 0:
            # Find the next effective character for s
            ptr_s = get_next_char(s, ptr_s)
            
            # Find the next effective character for t
            ptr_t = get_next_char(t, ptr_t)

            # Compare the effective characters
            if ptr_s >= 0 and ptr_t >= 0 and s[ptr_s] == t[ptr_t]:
                # Characters match, move to the previous ones
                ptr_s -= 1
                ptr_t -= 1
            else:
                # Mismatch, or one string exhausted while the other still has valid characters
                # If both ptr_s and ptr_t are -1, it means both strings became empty and matched.
                # Otherwise, they don't match.
                return ptr_s == -1 and ptr_t == -1
        
        # If the loop finishes, it means both ptr_s and ptr_t have become -1,
        # implying all effective characters matched.
        return True
