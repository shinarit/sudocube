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


if __name__ == "__main__":
  cube = SudoCube((4, 3))
  print cube.symbolSize
  cube.printCube()
