#!/usr/bin/env python
# -*- coding: utf-8 -*-

from operator import mul
from itertools import tee, izip, imap


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
    
    self.table = [0] * self.unitSizes[-1]

  def coordinateToInner(self, *coords):
    return sum(imap(mul, coords, self.unitSizes))
  
  def innerToCoordinate(self, inner):
    return map(lambda (div, mod): inner % mod / div, pairwise(self.unitSizes))


if __name__ == "__main__":
  cube = SudoCube((3, 2))
  print cube.unitSizes
  print cube.table
  print cube.innerToCoordinate(cube.coordinateToInner(1, 1))
  print cube.innerToCoordinate(cube.coordinateToInner(0, 1))
  print cube.innerToCoordinate(cube.coordinateToInner(1, 0))
  print cube.innerToCoordinate(cube.coordinateToInner(5, 5))
