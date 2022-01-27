#include <iostream>
#include "System.hpp"

int main() {
  System *curr = new System();
  curr->go();
  delete curr;
} 