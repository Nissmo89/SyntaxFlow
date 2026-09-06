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
    def shortestCompletingWord(self, licensePlate: str, words: List[str]) -> str:
        # Filter out non-letter characters from the license plate and convert it to lowercase
        plate = ''.join(filter(str.isalpha, licensePlate)).lower()

        # Count the frequency of each letter in the license plate
        count = {}
        for c in plate:
            count[c] = count.get(c, 0) + 1

        # Initialize the shortest word
        shortest = "aaaaaaaaaaaaaaaaaaaa"

        # Iterate through the list of words
        for word in words:
            # Count the frequency of each letter in the word
            wordCount = {}
            for c in word:
                wordCount[c] = wordCount.get(c, 0) + 1

            # Check if the word contains all the letters in the license plate
            containsAllLetters = True
            for c, freq in count.items():
                if wordCount.get(c, 0) < freq:
                    containsAllLetters = False
                    break

            # If the word contains all the letters and is shorter than the current shortest word, update the shortest word
            if containsAllLetters and len(word) < len(shortest):
                shortest = word

        return shortest
