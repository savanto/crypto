#ifndef _TRANSPOSITION_H
#define _TRANSPOSITION_H

#include <string>
#include <vector>
/* Called by the main program in response to transposition switch
 * Runs the transposition analysis subroutines
 */
void transposition(std::string ciphertext);

std::vector<int> factor(int n);

void paraqsort(std::vector<int> & master, std::string & slave, int low, int high);

std::string reverse(std::string s);

std::vector<std::string> makeGrid(int rows, int cols, std::string ciphertext);
#endif // _TRANSPOSITION_H
