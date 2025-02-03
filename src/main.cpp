 	
#include <iostream>
#include "vanEmdeBoas.cpp"
#include <climits>

int main ()
{
  
 // uint32_t two_to_the_32 = 4294967295;
  VanEmdeBoas veb = VanEmdeBoas(4);
  
  /*
  veb.insert(100);
  veb.insert(200);
  veb.insert(10000);
  veb.insert(294967296);
  */

  veb.insert(1);
  veb.insert(3);
  veb.insert(4);
  veb.insert(10);

  std::cout << veb.successor(0);
  std::cout << veb.successor(3);
  std::cout << veb.successor(5);
  std::cout << veb.successor(11);
  std::cout << veb.successor(15);

  //veb.insert(8);
  //veb.insert(100);
  //std::cout << std::to_string(veb.successor(10)) + "\n";
  /*
  std::cout << "\n";
  */
}

/*

Office Hours Questions:
unordered_map ?
type: ull? ll? How to represent min and max on initialization (null, -1, etc.)
strategy to benchmark

*/