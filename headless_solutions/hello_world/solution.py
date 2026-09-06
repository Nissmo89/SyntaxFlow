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

def leftover_blocks(num_blocks, num_colors):
    total_leftover = 0
    for _ in range(num_colors):
        color = num_blocks % 15
        total_leftover += color
        num_blocks //= 15
    return total_leftover

num_blocks = 100
num_colors = 5
total_leftover = leftover_blocks(num_blocks, num_colors)
print("Total leftover blocks:", total_leftover)
