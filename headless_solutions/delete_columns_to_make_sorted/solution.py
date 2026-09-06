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
    def minDeletionSize(self, strs: List[str]) -> int:
        # Handle edge case where input might be empty, though constraints say n >= 1
        if not strs:
            return 0

        deleted_columns_count = 0
        num_rows = len(strs)
        num_cols = len(strs[0]) # All strings have the same length

        # Iterate through each column
        for j in range(num_cols):
            # For each column, iterate through rows to check if it's sorted
            # We compare strs[i][j] with strs[i+1][j]
            for i in range(num_rows - 1):
                # If the current character is greater than the character below it,
                # the column is not sorted lexicographically.
                if strs[i][j] > strs[i+1][j]:
                    deleted_columns_count += 1 # Increment count for this unsorted column
                    break # No need to check further in this column, move to the next
        
        return deleted_columns_count
