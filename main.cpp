#include <iostream>
#include <iterator>

#include "sudocube.hpp"


int main()
{
  std::vector<int> sizes(std::istream_iterator<int>(std::cin), std::istream_iterator<int>());
  SudoCube cube{sizes};
  
  cube.print(std::cout);
  std::cout << '\n';
  int x = cube[0][1];
  std::cout << "x: " << x << '\n';
  for(int i(0); i<9; ++i)
    cube[i][0] = i;
  for(int i(0); i<9; ++i)
    cube[3][i] = 8 - i;
  cube.print(std::cout);
  std::cout << '\n';
}
