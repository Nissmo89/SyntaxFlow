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
    def flipAndInvertImage(self, image: List[List[int]]) -> List[List[int]]:
        for m in range(len(image)):
            image[m] = image[m][::-1]
            for n in range(len(image[m])):
                if image[m][n] == 0:
                    image[m][n] = 1
                else:
                    image[m][n] = 0
        return image