#include <iostream>
#include <iterator>

#include "sudocube.hpp"


int main()
{
  std::vector<int> sizes(std::istream_iterator<int>(std::cin), std::istream_iterator<int>());
  SudoCube cube{sizes};
  
  cube.print(std::cout);
  std::cout << '\n';
  std::cout << '\n';
  
  cube.solve();
  
  cube.print(std::cout);
  
  std::cout << '\n';
}
