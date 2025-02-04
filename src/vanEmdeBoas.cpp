
#include <iostream>
#include <math.h>
#include <cstdint>
#include <bitset>
/*
Implementation of vanEmdeBoas Tree supporting insert, query, and successor in
O(loglog(U)) operations
*/

class VEB {
    public: 
    virtual uint32_t get_max() {return 0;}
    virtual uint32_t get_min() {return 0;}
    virtual bool query(uint32_t x) {return 0;}
    virtual bool insert(uint32_t x) {return 0;}
    virtual long successor(uint32_t x) {return 0;}
};

class VebBase : public VEB {
    private:
    uint32_t min;
    uint32_t max;
    std::bitset<16> base_bits;

    public:

    VebBase() {
        //some value outside base case handling
        min = 101;
        max = 101;
    };

    //potential issues if min and max get called
    //when there has been no inserts, but a base
    //VEB should never be empty in real use

    uint32_t get_max() override {
        return max;
    }

    uint32_t get_min() override {
        return min;
    }

    bool insert(uint32_t x) override {
      //std::cout << "Insert Base Case";
        if (min == 101) {
            min = x;
            max = x;
        }
        if (x < min) {
            min = x;
        }
        if (x > max) {
            max = x;
        }
        base_bits[x] = 1;
        return true;
    }

    bool query(uint32_t x) override {
        return base_bits[x];
    }
    
    long successor(uint32_t x) override {
        for (int i = x + 1; i < 16; i++) {
            if (base_bits[i]) {
                return i;
            }
        }
        return -1;
    }
};

class VanEmdeBoas : VEB {
  private:
  uint32_t universe_bits;
  uint32_t cluster_bits;
  bool has_max;
  bool has_min;
  uint32_t min;
  uint32_t max;

  VEB* veb_metadata;
  VEB** clusters;

  /*Returns the "high" half of x, corresponding to it's cluster number*/
  uint32_t high(uint32_t x) {
    return x >> cluster_bits;
  }
  /*Returns the "low" half of x, corresponding to the index within the cluster*/
  uint32_t low(uint32_t x) {
    return x % (1 << cluster_bits);
  }

  public:

  VanEmdeBoas(uint32_t num_bits){
    //assumes size is a clean power of 2. (2^64, 2^32, 2^16, etc), things will break otherwise

    universe_bits = num_bits;
    cluster_bits = num_bits >> 1;
    has_max = false;
    has_min = false;
    clusters = (VEB**) new int*[(1 << cluster_bits)]();

    if(cluster_bits == 4) {
      veb_metadata = new VebBase();
    }
    else {
      veb_metadata = new VanEmdeBoas(cluster_bits);
    }
    
    
  }

  uint32_t get_max() override{
    return max;
  }

  uint32_t get_min() override{
    return min;
  }

  /*
  Inserts a value into the VEB. Return true if successful, false if an error occurs
  */
  bool insert(uint32_t x) override{
    //std::cerr << "insert call for " + std::to_string(x) + " Insert level: " + std::to_string(universe_bits) + "\n";

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

    //if cluster does not exist
    if (clusters[cluster_num] == NULL) {
      //std::cout << "Creating new cluster " + std::to_string(cluster_num) + "\n";
      if (cluster_bits == 4) {
        //std::cout << "Creating base case\n";
        clusters[cluster_num] = new VebBase();
      }
      else {
        //std::cout << "Creating non base case\n";
        clusters[cluster_num] = new VanEmdeBoas(cluster_bits);
      }
      //std::cout << "Inserting new cluster into metadata\n";
      veb_metadata->insert(cluster_num);
    }
    
    VanEmdeBoas *cluster = (VanEmdeBoas*) clusters[cluster_num];
    //std::cout << "Inserting cluster val " + std::to_string(cluster_val) + "\n";
    //std::cout << "Recursive insert into cluster: " + std::to_string(cluster_num) + " with value: " + std::to_string(cluster_val) + "\n";
    //std::cout << std::to_string((long)cluster) + "\n";
    cluster->insert(cluster_val);
    return true;
  }

  /*
  Queries the VEB. Returns true if value is found, returns false if not (or error)
  */
  bool query(uint32_t x) override {

    //std::cout << "Query call\n";
    //In base case, min and max simply represent whether 0 or 1 exist in the structure
    
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
    if (clusters[cluster_num] == NULL) {
      return false;
    }

    uint32_t cluster_val = low(x);
    VEB *cluster = clusters[cluster_num];
    //single recursive call into size sqrt(u)
    return cluster->query(cluster_val);
    
  }

/*
Returns the successor of x, or -1 if it does not exist
*/
  long successor(uint32_t x) {

  //std::cout << "Successor call\n";
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

    if (clusters[cluster_num] == NULL) {
      //skip to searching for successor in metadata
      //std::cout << "Searching for successor in metadata, current size = " + std::to_string(universe_bits) + "\n";
      long next_cluster_num = veb_metadata->successor(cluster_num);
      if (next_cluster_num == -1) {
        return -1;
      }
      else {
        //std::cout << "Returning next cluster min\n";
        VEB *next_cluster =  clusters[next_cluster_num];
        return (next_cluster_num << cluster_bits) + next_cluster->get_min();
      }
    }

    uint32_t cluster_val = low(x);
    VEB *cluster =  clusters[cluster_num];

    //recursive call into size sqrt(u)
    //std::cout << "Searching for successor in recursive call\n";
    long succ = cluster->successor(cluster_val);

    //if could not find the successor in recursive call, then successor must be the minimum of the next node
    if (succ == -1) {
      //std::cout << "searching for successor in metadata after recursive call\n";
      long next_cluster_num = veb_metadata->successor(cluster_num);
      if (next_cluster_num == -1) {
        return -1;
      }
      VEB *next_cluster = clusters[next_cluster_num];

      return (next_cluster_num << cluster_bits) + next_cluster->get_min();
    }
    else {
      return (cluster_num << cluster_bits) + succ;
    }
  
  return -1;
  }
};
