 	
#include <iostream>
#include "vanEmdeBoas.cpp"
#include <climits>

int main ()
{
  
 // uint32_t two_to_the_32 = 4294967295;
  VanEmdeBoas veb = VanEmdeBoas(32);
  
  /*
  veb.insert(100);
  veb.insert(200);
  veb.insert(10000);
  veb.insert(294967296);
  */

  veb.insert(1);
  veb.insert(3);
  veb.insert(100);
  veb.insert(1000);

  std::cout << veb.query(1);
  std::cout << veb.query(3);
  std::cout << veb.query(5);
  std::cout << veb.query(100);
  std::cout << veb.query(1000);
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