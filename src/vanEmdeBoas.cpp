 	
#ifndef VAN_EMDE_BOAS_TREE
#define VAN_EMDE_BOAS_TREE

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <cstdint>

/*
Implementation of vanEmdeBoas Tree supporting insert, query, and successor in
O(loglog(U)) operations
*/

class VanEmdeBoas {
  public:
  uint32_t universe_bits;
  uint32_t cluster_bits;
  uint32_t mod_size;
  bool has_max;
  bool has_min;
  uint32_t min;
  uint32_t max;
  VanEmdeBoas* veb_metadata;

  VanEmdeBoas **clusters;

  /*Returns the "high" half of x, corresponding to it's cluster number*/
  uint32_t high(uint32_t x) {
    return x >> cluster_bits;
  }
  /*Returns the "low" half of x, corresponding to the index within the cluster*/
  uint32_t low(uint32_t x) {
    return x % mod_size;
  }

  VanEmdeBoas(uint32_t num_bits){
    //assumes size is a clean power of 2. (2^64, 2^32, 2^16, etc), things will break otherwise
    universe_bits = num_bits;
    has_max = false;
    has_min = false;

    //base case!
    if (num_bits == 1) {
      veb_metadata = NULL;
    }
    else {
      cluster_bits = num_bits / 2;
      mod_size = 1 << cluster_bits;
      veb_metadata = new VanEmdeBoas(cluster_bits);
      clusters = (VanEmdeBoas**) new int* [1 << cluster_bits];
    }
  }

  /*
  Inserts a value into the VEB. Return true if successful, false if an error occurs
  */
  bool insert(uint32_t x) {
    //In base case, let min and max represent whether or not 0 and 1 exist in the structure
    if (universe_bits == 1) {
      if (x == 0) {
        min = 0;
        has_min = true;
        if (!has_max) {
          max = 0;
          has_max = true;
        }
      }
      else {
        max = 1;
        has_max = true;
        if (!has_min) {
          min = 1;
          has_min = true;
        }
        return true;
      }
      return false;
    }
    else {
      if (!has_min) {
        min = x;
        max = x;
        has_min = true;
        has_max = true;
        return true;
      }
      if (x < min) {
        uint32_t temp = min;
        min = x;
        x = temp;
      }
      else if (x > max) {
        max = x;
      }

      //continue insert after potential min/max swaps
      uint32_t cluster_num = high(x);
      uint32_t cluster_val = low(x);

      //create cluster if it does not exist
      
      //if (clusters.find(cluster_num) == clusters.end()) {
      // VanEmdeBoas *newVEB = new VanEmdeBoas(cluster_bits);
      //  clusters.insert({cluster_num, newVEB});
      //  veb_metadata->insert(cluster_num);
      //}

      //if cluster does not exist
      if (!veb_metadata->query(cluster_num)) {
        clusters[cluster_num] = new VanEmdeBoas(cluster_bits);
        veb_metadata->insert(cluster_num);
      }
      
      VanEmdeBoas *cluster = clusters[cluster_num];
      //std::cout << "Inserting cluster val " + std::to_string(cluster_val) + "\n";
      cluster->insert(cluster_val);
      return true;
    }
  }

  /*
  Queries the VEB. Returns true if value is found, returns false if not (or error)
  */
  bool query(uint32_t x) {
    //In base case, min and max simply represent whether 0 or 1 exist in the structure
    if (universe_bits == 1) {
      if (x == 0) {
        return has_min && min == 0;
      }
      else {
        return has_max && max == 1;
      }
    }
    else {
      //if tree has no inserted elements yet return false
      if (!has_min) {
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
      uint32_t cluster_num = high(x);

      //if cluster does not exist, return false
      if (!veb_metadata->query(cluster_num)) {
        return false;
      }

      uint32_t cluster_val = low(x);
      VanEmdeBoas *cluster = clusters[cluster_num];
      //single recursive call into size sqrt(u)
      return cluster->query(cluster_val);
    }
  }

/*
Returns the successor of x, or -1 if it does not exist
*/
  long successor(uint32_t x) {
    //base case
    if (universe_bits == 1) {

      //successor does not exist for max
      if (x == 1) {
        return -1;
      }
      else {
        //return max if it exists, returns null if it doesn't
        if (has_max) {
          return 1;
        }
        return -1;
      }
    }
    else {
      //if tree has no elements, cannot find successor
      if(!has_max && !has_min) {
        return -1;
      }

      //if element is greater than or equal to max, cannot find successor
      if (x >= max) {
        return -1;
      }

      //if element is less than minimum, return minimum
      if (x < min) {
        return min;
      }

      
      uint32_t cluster_num = high(x);

      if (!veb_metadata->query(cluster_num)) {
        //skip to searching for successor in metadata
        long next_cluster_num = veb_metadata->successor(cluster_num);
        if (next_cluster_num == -1) {
          return -1;
        }
        else {
          VanEmdeBoas *next_cluster = clusters[next_cluster_num];
          return (next_cluster_num << cluster_bits) + next_cluster->min;
        }
      }

      uint32_t cluster_val = low(x);
      VanEmdeBoas *cluster = clusters[cluster_num];

      //recursive call into size sqrt(u)
      long succ = cluster->successor(cluster_val);


      //if could not find the successor in recursive call, then successor must be the minimum of the next node
      if (succ == -1) {

        long next_cluster_num = veb_metadata->successor(cluster_num);
        if (next_cluster_num == -1) {
          return -1;
        }
        VanEmdeBoas *next_cluster = clusters[next_cluster_num];

        return (next_cluster_num << cluster_bits) + next_cluster->min;
      }
      else {
        return (cluster_num << cluster_bits) + succ;
      }
    }
    return -1;
  }
};

#endif