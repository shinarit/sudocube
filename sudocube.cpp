#include "sudocube.hpp"

void SudoCube::print(std::ostream& out)
{
  printRecursively(out, mDimensions.size(), 0);
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

