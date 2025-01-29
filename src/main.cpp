 	
#include <iostream>
#include "vanEmdeBoas.cpp"

int main ()
{

  VanEmdeBoas veb = VanEmdeBoas(65536);

  veb.insert(0);
  veb.insert(2);
  veb.insert(3);
  veb.insert(8);
  veb.insert(100);
  veb.insert(200);
  veb.insert(10000);

/*
std::cout << veb.query(129);
  std::cout << veb.query(130);
  std::cout << veb.query(131);
  std::cout << veb.query(132);
  std::cout << veb.query(133);
*/

  std::cout << veb.successor(1000);
  std::cout << "\n";
  
  //std::cout << "Succ(3) = ";
  //std::cout << veb.successor(3);
  //std::cout << "\n";

  /*
  veb.insert(120);
  veb.insert(8);

  std::cout << (veb.query(120) ? "Has 120\n" : "Does not have 120\n");
  std::cout << (veb.query(8) ? "Has 8\n" : "Does not have 8\n");

  std::cout << "Succ(0) = ";
  std::cout << veb.successor(0);
  std::cout << "\n";

  std::cout << "Succ(100) = ";
  std::cout << veb.successor(100);
  std::cout << "\n";

  */


  //base case testing
  /*
  VanEmdeBoas veb = VanEmdeBoas(2);
  std::cout << (veb.query(0) ? "Has 0\n" : "Does not have 0\n");
  std::cout << "Inserting 0\n";
  veb.insert(0);
  std::cout << (veb.query(0) ? "Has 0\n" : "Does not have 0\n");
  std::cout << veb.successor(0);

  std::cout << (veb.query(1) ? "Has 0\n" : "Does not have 1\n");
  std::cout << "Inserting 1\n";
  veb.insert(1);
  std::cout << (veb.query(1) ? "Has 0\n" : "Does not have 1\n");

  std::cout << veb.successor(0);
  */
  
}

