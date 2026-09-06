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
    def compareVersion(self, version1: str, version2: str) -> int:
        p1 = 0  # Pointer for version1
        p2 = 0  # Pointer for version2
        n1 = len(version1)
        n2 = len(version2)

        # Iterate as long as there are characters to process in either string
        while p1 < n1 or p2 < n2:
            num1 = 0
            num2 = 0

            # Parse the current revision from version1
            while p1 < n1 and version1[p1] != '.':
                num1 = num1 * 10 + int(version1[p1])
                p1 += 1

            # Parse the current revision from version2
            while p2 < n2 and version2[p2] != '.':
                num2 = num2 * 10 + int(version2[p2])
                p2 += 1

            # Compare the parsed revisions
            if num1 > num2:
                return 1
            if num1 < num2:
                return -1

            # Move past the dot if it exists
            if p1 < n1:
                p1 += 1  # Skip the '.'
            if p2 < n2:
                p2 += 1  # Skip the '.'

        # If all revisions were equal
        return 0
