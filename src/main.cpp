 	
#include <iostream>
#include "vanEmdeBoas.cpp"
#include <climits>

int main ()
{
  
  long long two_to_the_32 = 4294967296;
  VanEmdeBoas veb = VanEmdeBoas(two_to_the_32);

  veb.insert(0);
  veb.insert(2);
  veb.insert(3);
  veb.insert(8);
  veb.insert(100);
  veb.insert(200);
  veb.insert(10000);
  veb.insert(294967296);

  std::cout << veb.successor(50000);
  std::cout << "\n";
}

