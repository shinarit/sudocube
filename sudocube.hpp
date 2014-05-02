#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <numeric>
#include <string>
#include <array>
#include <map>
#include <iosfwd>

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
  , mBoxJump(mTable.size(), 0)
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
    //2d
    mSeparators.push_back({"\n" + std::string(lineLength, '-') + "\n", "\n" + std::string(lineLength, '=') + "\n"});
    //3d
    mSeparators.push_back({"\n\n" + std::string(lineLength, '=') + "\n\n", "\n\n" + std::string(lineLength, '|') + "\n\n"});
    
    //generate box checkers
    IntList boxTemplate(generateBox());
    for (int i(0); i < mTable.size(); ++i)
    {
      int corner(getBoxCorner(i));
      auto it(mBoxes.find(corner));
      if (it == mBoxes.end())
      {
        IntList box(mEdgeSize);
        std::transform(begin(boxTemplate), end(boxTemplate), std::back_insert_iterator<IntList>(box), [corner](int index){ return index + corner; });
        mBoxes.insert({corner, box});
      }
      mBoxJump[i] = &mBoxes[corner];
    }
  }

  void print(std::ostream& out) const;
  
  bool solve();
  
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

private:
  typedef std::vector<UnitType> ValueList;
  typedef std::vector<int> IntList;
  typedef std::string Separator;
  typedef std::array<Separator, 2> SeparatorPair;
  typedef std::vector<SeparatorPair> SeparatorList;
  typedef std::map<int, IntList> Boxes;
  typedef std::vector<IntList*> BoxJumplist;

  bool increaseOnPosition(int index);
  bool positionValid(int index) const;
  bool checkLine(const UnitType* array, int step) const;
  
  void printRecursively(std::ostream& out, int level, int unitIndex) const;

  IntList generateBox() const;
  int getBoxCorner(int index) const;

  IntList       mDimensions;
  int           mEdgeSize;
  ValueList     mTable;
  
  IntList       mUnitSizes;
  int           mSymbolWidth;
  
  SeparatorList mSeparators;

  Boxes         mBoxes;
  BoxJumplist   mBoxJump;
};
