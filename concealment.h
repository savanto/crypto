#ifndef _CONCEALMENT_H
#define _CONCEALMENT_H

#include <string>
#include <vector>
/* Called by the main program in response to concealment switch
 * Runs the concealment analysis subroutines
 */
void concealment(std::string ciphertext);

void letters(std::vector<std::string> cipherwords);

int shortest(std::vector<std::string> cipherwords);

int longest(std::vector<std::string> cipherwords);
#endif // _CONCEALMENT_H
