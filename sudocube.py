#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pdb

from operator import mul
from itertools import tee, izip, imap, repeat
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

    self.boxes = {}
    for idx in xrange(len(self.table)):
      corner = self.getBoxBase(idx)
      if not corner in self.boxes:
        self.boxes[corner] = self.generate(corner)
    
    unitSeparator = ' | '
    unitSeparatorSpec = ' || '

    lineLength = self.edgeSize * self.symbolSize + (self.edgeSize / self.dimensions[0] - 1) * len(unitSeparatorSpec) + (self.dimensions[0] - 1) * self.edgeSize / self.dimensions[0] * len(unitSeparator) #the padding
    
    lineSeparator = '\n' + '-' * lineLength + '\n'
    lineSeparatorSpec = '\n' + '=' * lineLength + '\n'
    
    layerSeparator = '\n\n' + 'X' * lineLength + '\n\n'
    layerSeparatorSpec = '\n\n' + '()' * ((lineLength + 1) / 2) + '\n\n'
    
    self.separators =[(unitSeparator, unitSeparatorSpec),
                      (lineSeparator, lineSeparatorSpec),
                      (layerSeparator, layerSeparatorSpec)]

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
  
  def __repr__(self):
    return str((self.dimensions.__repr__(), self.table.__repr__()))
    
  def __str__(self):
    return self.strRecursive(len(self.dimensions), 0)

  def strRecursive(self, level, unitIndex):
    if 0 == level:
      val = self.table[unitIndex]
      if val > 0:
        return str(val).rjust(self.symbolSize)
      return ' ' * self.symbolSize
    else:
      index = unitIndex * self.unitSizes[level]
      subIndex = unitIndex * self.edgeSize
      return self.separators[level - 1][1].join([self.separators[level - 1][0].join([self.strRecursive(level - 1, idx + i) for i in xrange(self.dimensions[level - 1])]) for idx in xrange(subIndex, subIndex + self.edgeSize, self.dimensions[level - 1])])

  def checkLine(self, line):
    checker = [0] * (self.edgeSize)
    for value in line:
      if value > 0:
        checker[value - 1] += 1
        if checker[value - 1] > 1:
          return False
    return True

  def validatePosition(self, index):
    "startpointokból indulunk self.dimension[i]-nyi stepekkel haladva"
    coords = self.innerToCoordinate(index)
    startPoints = [self.coordinateToInner(*(coords[:i] + [0] + coords[i + 1:])) for i in xrange(self.numOfDimensions)]
    for start, step in izip(startPoints, self.unitSizes):
      if not self.checkLine(self.table[start : start + self.edgeSize * step : step]):
        return False

    return self.checkLine([self.table[i] for i in self.boxes[self.getBoxBase(index)]])

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

  def getBoxBase(self, idx):
    for i in xrange(len(self.dimensions)):
      delta = idx % self.unitSizes[i]
      idx -= delta
      idx -= idx % (self.dimensions[i] * self.unitSizes[i])
      idx += delta
    return idx

  def generate(self, idx):
    gen = []
    coords = [0] * self.numOfDimensions
    for i in repeat(None, self.edgeSize):
      gen.append(idx)
      for j in xrange(len(coords)):
        coords[j] += 1
        idx += self.unitSizes[j]
        coord, mod = coords[j], self.dimensions[j]
        if coord == mod:
          coords[j] = 0
          idx -= mod * self.unitSizes[j]
        else:
          break
    return gen


if __name__ == "__main__":
  cube = SudoCube((4, 1, 1))
  print cube.symbolSize
  print cube
  cube.solve()
  print cube
  
  print cube.__repr__()
