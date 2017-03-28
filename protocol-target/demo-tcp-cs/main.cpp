#include <iostream>
using namespace std;

int main() {
  int x;
  int y ;
  int *p = NULL;

  cin >> x;
  if(x!=8) {
    *p = x;
    return 0;
  }
   

  cin >> y;
  if (y == 9)
    *p = x;
 
  return 0;
}
