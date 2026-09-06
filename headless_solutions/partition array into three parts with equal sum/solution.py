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
    def canThreePartsEqualSum(self, arr: list[int]) -> bool:
        total_sum = sum(arr)
        if total_sum % 3 != 0:
            return False
        
        target = total_sum // 3
        current_sum = 0
        count = 0
        
        # Iterate up to the second to last element
        for i in range(len(arr) - 1):
            current_sum += arr[i]
            if current_sum == target:
                count += 1
                current_sum = 0
                if count == 2:
                    return True
        return False
