#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <numeric>
#include <string>
#include <array>

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
  , mSymbolWidth(std::log10(mEdgeSize) + 1)
  {
    int unit(1);
    for(int i(0); i <= mDimensions.size(); ++i)
    {
      mUnitSizes.push_back(unit);
      unit *= mEdgeSize;
    }
    
    mSeparators.push_back({"|", "||"});

    int lineLength = mEdgeSize * mSymbolWidth +
                     (mEdgeSize / mDimensions[0] - 1) * mSeparators[0][1].size() +
                     (mDimensions[0] - 1) * mEdgeSize / mDimensions[0] * mSeparators[0][0].size();
    mSeparators.push_back({"\n" + std::string(lineLength, '-') + "\n", "\n" + std::string(lineLength, '=') + "\n"});
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

  void print(std::ostream& out);
  
private:
  std::vector<std::array<std::string, 2>> mSeparators;
  typedef std::vector<UnitType> ValueList;

  void printRecursively(std::ostream& out, int level, int unitIndex);

  ValueList     mDimensions;
  int           mEdgeSize;
  ValueList     mTable;
  
  ValueList     mUnitSizes;
  int           mSymbolWidth;
};
