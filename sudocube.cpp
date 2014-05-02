#include "sudocube.hpp"

#include <ostream>

void SudoCube::print(std::ostream& out)
{
  printRecursively(out, mDimensions.size(), 0);
}

bool SudoCube::solve()
{
  IntList emptyPlaces;
  for (int i(0); i < mTable.size(); ++i)
  {
    if (0 == mTable[i])
    {
      emptyPlaces.push_back(i);
    }
  }

  int curr(0);
  while (0 <= curr && curr < emptyPlaces.size())
  {
    int index(emptyPlaces[curr]);
    if (increaseOnPosition(index))
    {
      ++curr;
    }
    else
    {
      mTable[index] = 0;
      --curr;
    }
  }
  return curr != -1;
}

bool SudoCube::increaseOnPosition(int index)
{
  do
  {
    ++mTable[index];
  } while (mTable[index] <= mEdgeSize && !positionValid(index));
  return mTable[index] <= mEdgeSize;
}

bool SudoCube::positionValid(int index)
{
  //check the lines
  for (int i(0); i < mDimensions.size(); ++i)
  {
    if (!checkLine(&mTable[index + index % mUnitSizes[i] - index % mUnitSizes[i + 1]], mUnitSizes[i]))
    {
      return false;
    }
  }
  
  //check the box
  
  return true;
}

bool SudoCube::checkLine(UnitType* array, int step)
{
  static IntList checker;
  checker.assign(mEdgeSize, 0);
  
  for (int i(0); i < mEdgeSize; ++i)
  {
    if (*array > 0)
    {
      if (1 != ++checker[(*array) - 1])
      {
        return false;
      }
    }
    array += step;
  }
  return true;
}

void SudoCube::printRecursively(std::ostream& out, int level, int unitIndex)
{
  if (0 == level)
  {
    int value(mTable[unitIndex]);
    if (value)
    {
      out << value;
    }
    else
    {
      out << ' ';
    }
    int written(std::log10(value) + 1);
    for (int i(0); i < mSymbolWidth - written; ++i)
    {
      out << ' ';
    }
  }
  else
  {
    int index(mUnitSizes[level] * unitIndex);
    for (int i(0); i < mEdgeSize; ++i)
    {
      if (0 != i)
      {
        out << mSeparators[level - 1][0 == i % mDimensions[level - 1]];
      }
      printRecursively(out, level - 1, index / mUnitSizes[level - 1] + i);
    }
  }
}

