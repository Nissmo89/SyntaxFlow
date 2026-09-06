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

from collections import deque

class Solution:
    def ladderLength(self, beginWord: str, endWord: str, wordList: list[str]) -> int:
        word_set = set(wordList)
        if endWord not in word_set: return 0
        
        queue = deque([(beginWord, 1)])
        while queue:
            word, length = queue.popleft()
            if word == endWord: return length
            
            for i in range(len(word)):
                for char_code in range(ord('a'), ord('z') + 1):
                    next_word = word[:i] + chr(char_code) + word[i+1:]
                    if next_word in word_set:
                        word_set.remove(next_word)
                        queue.append((next_word, length + 1))
        return 0
