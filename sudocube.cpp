#include "sudocube.hpp"

#include <ostream>

void SudoCube::print(std::ostream& out) const
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

bool SudoCube::positionValid(int index) const
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

bool SudoCube::checkLine(const UnitType* array, int step) const
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

void SudoCube::printRecursively(std::ostream& out, int level, int unitIndex) const
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

SudoCube::IntList SudoCube::generateBox() const
{
  IntList result;
  IntList coords(mDimensions.size(), 0);
  int index(0);
  for (int i(0); i < mEdgeSize; ++i)
  {
    result.push_back(index);
    for (int j(0); j < coords.size(); ++j)
    {
      int& coord(coords[j]);
      int mod(mDimensions[j]);

      ++coord;
      index += mUnitSizes[j];
      
      if (coord == mod)
      {
        coord = 0;
        index -= mod * mUnitSizes[j];
      }
      else
      {
        break;
      }
    }
  }

  return result;
}

int SudoCube::getBoxCorner(int index) const
{
  for (int i(0); i < mDimensions.size(); ++i)
  {
    int delta(index % mUnitSizes[i]);

    index -= delta;
    index -= index % (mDimensions[i] * mUnitSizes[i]);
    index += delta;
  }
  return index;
}

