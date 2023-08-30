#include "Maze.cpp"
int main(){
  Maze maze{'i'-'a'+1 ,'w'-'a'+1};
  std::string maze_input{R"(
                            a    11111111111000000111111
                            b    11111111111011110000011
                            c    110S1111011000111111E11
                            d    11011001011110001111111
                            e    10011001010001100000111
                            f    10001001011101001111011
                            g    11101111000101011100011
                            h    11100000010000000000011
                            i    11111111111111111111111

                                 abcdefghijklmnopqrstuvw
)"};
  maze.assign(maze_input).show().find_route().show();
  return 0;
}
