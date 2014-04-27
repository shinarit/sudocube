#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <numeric>

// temporary!!!
#include <ostream>
#include <iterator>
///temporary!!!

class SudoCube
{
public:
  typedef int UnitType;
  
  template <class Cont>
  SudoCube(const Cont& dimensions)
  : mDimensions(begin(dimensions), end(dimensions))
  , mEdgeSize(std::accumulate(begin(dimensions), end(dimensions), 1, std::multiplies<int>()))
  , mTable(std::pow(mEdgeSize, mDimensions.size()))
  {
    int unit(1);
    for(int i(0); i <= mDimensions.size(); ++i)
    {
      mUnitSizes.push_back(unit);
      unit *= mEdgeSize;
    }
  }
  
private:
  struct Indexer
  {
    typedef std::vector<int> IndexList;
    Indexer(SudoCube& cube, const IndexList& indexes, int nextIndex): mCube(cube), mIndexes(indexes)
    {
      mIndexes.push_back(nextIndex);
    }
    Indexer operator[](int index)
    {
      return Indexer(mCube, mIndexes, index);
    }

    operator UnitType()
    {
      return mCube.mTable[std::inner_product(begin(mIndexes), end(mIndexes), begin(mCube.mUnitSizes), 0)];
    }
    typedef UnitType& UnitTypeRef;
    UnitTypeRef operator=(UnitType value)
    {
      int index(std::inner_product(begin(mIndexes), end(mIndexes), begin(mCube.mUnitSizes), 0));
      mCube.mTable[index] = value;
      return mCube.mTable[index];
    }

    IndexList   mIndexes;
    SudoCube&   mCube;
  };
  
public:
  Indexer operator[](int index)
  {
    return Indexer(*this, {}, index);
  }

  void print(std::ostream& out)
  {
    std::copy(begin(mTable), end(mTable), std::ostream_iterator<int>(out));
  }

private:
  typedef std::vector<UnitType> ValueList;

  ValueList     mDimensions;
  int           mEdgeSize;
  ValueList     mTable;
  
  ValueList     mUnitSizes;
};
