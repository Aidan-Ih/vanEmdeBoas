 	
#include <iostream>
#include <math.h>
#include <unordered_map>

class VanEmdeBoas {
  public:
  int universe_size;
  int cluster_size;
  int min;
  int max;
  VanEmdeBoas* veb_metadata;

  std::unordered_map<int, VanEmdeBoas*> clusters;

  /*Returns the "high" half of x, corresponding to it's cluster number*/
  int high(int x) {
    return x >> int(log2(cluster_size));
  }
  /*Returns the "low" half of x, corresponding to the index within the cluster*/
  int low(int x) {
    return x % cluster_size;
  }

  VanEmdeBoas(int size){
    //assumes size is a clean power of 2. (2^64, 2^32, 2^16, etc), things will break otherwise
    universe_size = size;
    min = -1;
    max = -1;

    //base case!
    if (size <= 2) {
      cluster_size = -1;
      veb_metadata = NULL;
    }
    else {
      cluster_size = int(sqrt(size));
      veb_metadata = new VanEmdeBoas(cluster_size);
    }
  }

  /*
  Inserts a value into the VEB. Return true if successful, false if an error occurs
  */
  bool insert(int x) {
    //In base case, let min and max represent whether or not 0 and 1 exist in the structure
    if (universe_size == 2) {
      //std::cout << "Hello from the insert base case\n";
      if (x == 0) {
        min = 0;
        return true;
      }
      else {
        max = 1;
        return true;
      }
    }
    else {
      //first insertion, max/min initialized to -1

      //std::cout << "Current size: ";
      //std::cout << universe_size;
      //std::cout << "\n";
      if (min == -1 && max == -1) {
        min = x;
        max = x;
        return true;
      }
      if (x < min) {
        int temp = min;
        min = x;
        x = temp;
      }
      else if (x > max) {
        int temp = max;
        max = x;
        x = temp;
      }

      //continue insert after potential min/max swaps
      int cluster_num = high(x);
      int cluster_val = low(x);

      //create cluster if it does not exist
      if (clusters.find(cluster_num) == clusters.end()) {
        VanEmdeBoas *newVEB = new VanEmdeBoas(cluster_size);
        clusters.insert({cluster_num, newVEB});
        veb_metadata->insert(cluster_num);
      }
      
      VanEmdeBoas *cluster = clusters[cluster_num];
      //single recursive call into size sqrt(u)
      clusters[cluster_num]->insert(cluster_val);
      return true;
    }
  }

  /*
  Queries the VEB. Returns true if value is found, returns false if not (or error)
  */
  bool query(int x) {
    //In base case, min and max simply represent whether 0 or 1 exist in the structure
    if (universe_size == 2) {
      if (x == 0) {
        return (min == -1) ? false : true;
      }
      else {
        return (max == -1) ? false : true;
      }
    }
    else {
      //if tree has no inserted elements yet return false
      if (min == -1) {
        return false;
      }

      //if x < min or x > max return false
      if (x < min || x > max) {
        return false;
      }

      //if x == min or max return true
      if (x == min || x == max) {
        return true;
      }

      //if none of above, make recursive call into cluster
      int cluster_num = high(x);

      //if cluster does not exist, return false
      if (clusters.find(cluster_num) == clusters.end()) {
        return false;
      }

      int cluster_val = low(x);
      VanEmdeBoas *cluster = clusters[cluster_num];
      //single recursive call into size sqrt(u)
      return cluster->query(cluster_val);
    }
  }

/*
Returns the successor of x, or -1 if it does not exist
*/
  int successor(int x) {
    //base case
    std::cout << "Successor call for: ";
    std::cout << x;
    std::cout << "\n";

    if (universe_size == 2) {
      //successor does not exist for max
      if (x == 1) {
        return -1;
      }
      else {
        //return max if it exists, returns null if it doesn't
        return max;
      }
    }
    else {
      //if tree has no elements, cannot find successor
      if(max == -1 || min == -1) {
        std::cout << "Successor returning -1 due to empty tree";
        return -1;
      }

      //if element is greater than or equal to max, cannot find successor
      if (x >= max) {
        std::cout << "successor returning -1 due to greater than max\n";
        return -1;
      }

      //if element is less than minimum, return minimum
      if (x < min) {
        std::cout << "Successor returning min\n";
        return min;
      }

      int cluster_num = high(x);
      if (clusters.find(cluster_num) == clusters.end()) {
        std::cout << "Successor could not find cluster\n";
        return -1;
      }

      int cluster_val = low(x);
      VanEmdeBoas *cluster = clusters[cluster_num];

      std::cout << "Recursive call into cluster: ";
      std::cout << cluster_num;
      std::cout << " with val: ";
      std::cout << cluster_val;
      std::cout << "\n";

      //recursive call into size sqrt(u)
      int succ = cluster->successor(cluster_val);

      //if could not find the successor in recursive call, then successor must be the minimum of the next node
      if (succ == -1) {
        int next_cluster_num = veb_metadata->successor(cluster_num);
        if (next_cluster_num == -1) {
          return -1;
        }
        VanEmdeBoas *next_cluster = clusters[next_cluster_num];
        return next_cluster->min;
      }

      //otherwise, return found successor
      return (cluster_num << cluster_size) + succ;
    }
  }
};