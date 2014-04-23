#!/usr/bin/env python
# -*- coding: utf-8 -*-

from operator import mul
from itertools import tee, izip, imap
from math import log10

def pairwise(iterable):
  "s -> (s0,s1), (s1,s2), (s2, s3), ..."
  a, b = tee(iterable)
  next(b, None)
  return izip(a, b)

class SudoCube:
  def __init__(self, dimensions):
    """Sudoku generalized for arbitrary dimensions
    dimensions: the orthotope that is the basic building stone of the sudo(hyper)cube, everything comes from this"""

    self.dimensions = map(int, dimensions)
    self.numOfDimensions = len(self.dimensions)
    self.edgeSize = reduce(mul, self.dimensions)
    self.unitSizes = map(lambda x: self.edgeSize ** x, range(len(self.dimensions) + 1))
    
    self.symbolSize = int(log10(self.edgeSize) + 1)

    self.table = [0] * self.unitSizes[-1]

  def coordinateToInner(self, *coords):
    return sum(imap(mul, coords, self.unitSizes))

  def innerToCoordinate(self, inner):
    return map(lambda (div, mod): inner % mod / div, pairwise(self.unitSizes))

  def __getitem__(self, key):
    "expect key to be an iterable of coordinates"
    return self.table[self.coordinateToInner(*key)]

  def __setitem__(self, key, value):
    "expect key to be an iterable of coordinates"
    self.table[self.coordinateToInner(*key)] = value

  def printLine(self, line):
    index = line * self.unitSizes[1]
    for i in range(self.edgeSize / self.dimensions[0]):
      for j in range(self.dimensions[0]): 
        print str(self.table[index + i * self.dimensions[0] + j]).rjust(self.symbolSize), ' ',
      print ' ',
    print ''

  def printLayer(self, layer):
    lineIdx = layer * self.edgeSize
    for i in range(self.edgeSize):
      if 0 == i % self.dimensions[1]: print ''
      self.printLine(lineIdx + i)

  def printCube(self):
    if 2 == len(self.dimensions):
      self.printLayer(0)

  def validatePosition(self, index):
    "startpointokból indulunk self.dimension[i]-nyi stepekkel haladva"
    coords = self.innerToCoordinate(index)
    startPoints = [self.coordinateToInner(*(coords[:i] + [0] + coords[i + 1:])) for i in xrange(self.numOfDimensions)]
    for start, step in izip(startPoints, self.unitSizes):
      checker = [0] * (self.edgeSize)
      for value in self.table[start : start + self.edgeSize * step : step]:
        if value > 0:
          checker[value - 1] += 1
          if checker[value - 1] > 1:
            return False
    return True

  def increaseOnPosition(self, index):
    self.table[index] += 1
    while self.table[index] <= self.edgeSize and not self.validatePosition(index):
      self.table[index] += 1
    return self.table[index] <= self.edgeSize

  def solve(self):
    emptyPlaces = [x[1] for x in izip(self.table, xrange(len(self.table))) if 0 == x[0]]
    if 0 == len(emptyPlaces):
      return
    curr = 0
    while curr >= 0 and curr < len(emptyPlaces):
      index = emptyPlaces[curr]
      if self.increaseOnPosition(index):
        curr += 1
      else:
        self.table[index] = 0
        curr -= 1


if __name__ == "__main__":
  cube = SudoCube((2, 3))
  print cube.symbolSize
  cube.printCube()
  cube.solve()
  cube.printCube()
