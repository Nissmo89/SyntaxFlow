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
    def reconstructQueue(self, people: List[List[int]]) -> List[List[int]]:
        # Sort people:
        # 1. By height in descending order (-p[0])
        # 2. By k-value in ascending order (p[1])
        # The lambda function defines the custom sorting key.
        # -p[0] ensures descending order for height.
        # p[1] ensures ascending order for k for people with the same height.
        people.sort(key=lambda p: (-p[0], p[1]))

        queue = []

        # Insert each person into the queue at their k-th position
        for person in people:
            # person[1] is the k-value, which is the index for insertion.
            # list.insert(index, element) inserts the element at the specified index,
            # shifting subsequent elements to the right.
            queue.insert(person[1], person)

        return queue
