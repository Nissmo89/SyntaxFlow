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
    def floodFill(self, image: List[List[int]], sr: int, sc: int, color: int) -> List[List[int]]:
        originalColor = image[sr][sc]
        self.dfs(image, sr, sc, originalColor, color)
        return image

    def dfs(self, image, i, j, originalColor, newColor):
        if i < 0 or i >= len(image) or j < 0 or j >= len(image[0]) or image[i][j] == newColor or image[i][j] != originalColor:
            return
        image[i][j] = newColor
        self.dfs(image, i - 1, j, originalColor, newColor)
        self.dfs(image, i + 1, j, originalColor, newColor)
        self.dfs(image, i, j - 1, originalColor, newColor)
        self.dfs(image, i, j + 1, originalColor, newColor)
