#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
class Maze{
public:
  using mtype  = std::vector<std::vector<char>>;
  using mmtype = std::vector<char>;
  using msize  = mtype::size_type;
  using mpt    = std::pair<msize, msize>;
  using mpts   = std::set<mpt>;

  
  msize mrow, mcol;
  msize npos{std::numeric_limits<msize>::max()};
  mtype maze;
  mpt   start, exit;
  mpts  exit_route{},visited{};
  bool  is_assigned{false}, is_solved{false}, is_unsolvable{false};
  Maze(msize row, msize col):mrow{row},mcol{col},maze{mtype(row, mmtype(col, '0'))}{
    std::cout << "Maze is constructed." << std::endl;
  }

  //assigns Maze by string only 'S', 'E', '0' or '1' will be recognised.
  //'S': Start point, 'E': Exit, '0': Accessible, '1': Inaccessible
  Maze& assign(std::string_view const& maze_input){
    std::string input(maze_input.length(), '_');
    msize const valid_size{mrow*mcol};
    auto siter{std::copy_if(maze_input.cbegin(),maze_input.cend(), input.begin(),
			    [](char const& ch)->bool{return ch=='0'||ch=='1'||ch=='S'||ch=='E';})};
    if (auto len{std::distance(input.begin(), siter)}; len!= valid_size){
      throw std::runtime_error(len<valid_size?"Too short maze_input.":"Too long maze_input.");
    }//input size error ends

    
    bool start_already_set{false},exit_already_set{false};
    for (msize row{}; row < mrow; ++row){
      for (msize col{}; col < mcol; ++col){
	switch(msize const idx{mcol*row+col}; input.at(idx)){// 0 <= row*mcol+col+1 <= mrow*mcol - 1
	case 'S': if (start_already_set){
	    throw std::runtime_error("Start point more than 1.");
	  }else{
	    maze[row][col] = 'S';
	    start = std::make_pair(row,col);
	    break;
	  }
	case 'E': if (start_already_set){
	    throw std::runtime_error("Start point more than 1.");
	  }else{
	    maze[row][col] = 'E';
	    exit = std::make_pair(row,col);
	    break;
	  }
	default: maze[row][col] = input[idx];
	}
      }
    }
    is_assigned = true;
    return *this;
  }
      
  //displays assigned Maze. if it is solved marks its route by the char == ' '(space!).
  Maze& show(){
    if (is_assigned){
      std::printf("\n\n\n");
      for (msize r{}; r != mrow; ++r){
	for (msize c{}; c != mcol; ++c){
	  mpt const pt{r,c};
	  if (is_solved&&exit_route.find(pt) != exit_route.end()
	      && pt != start && pt != exit){
	    std::cout << ' ';
	  }else{
	    std::cout << maze[r][c];
	  }
	}
	std::printf("\n");
      }
      std::cout << "Start Point at [" << start.first << " , " << start.second << "]"<< std::endl;
      std::cout << "Exit  Point at  [" << exit.first << " , " << exit.second << "]"<< std::endl;
      if (is_unsolvable){std::cout << "Assigned Maze is not solvable!!" << std::endl;}
      std::printf("\n\n\n");
      
    }else{
      std::cout << "\n\n\nCurrently Maze object has not meaningful states...\n\n\n" << std::endl;
    }
    return *this;
  }


  //finds out the route if exists
  Maze& find_route(){
    visited.clear();
    exit_route.clear();
    find_route_internal(start);
    if (exit_route.size() != 0){
      is_solved = true;
      is_unsolvable = false;
    }else{
      is_solved = false;
      is_unsolvable = true;
    }
    return *this;
  }

  
  //this one called inside find_route.
  //a vital part implementing backtracking algorithm 
  void find_route_internal(mpt s, mpts route = mpts{}){
    while (route.insert(s).second){
      visited.insert(s);
      for (msize dir{}; dir != 4; ++dir){
	auto const n{get_neighbour(s, dir)};
	if (is_accessible(n) and visited.find(n) == visited.end()){
	  find_route_internal(n, route);
	}
      }
    }
    if (s == exit){
      exit_route = route;
    }
  }
    



  //internal methods for finding routes of the Maze begins

  mpt get_neighbour(mpt const& pt, msize direction){
    auto [r,c]{pt};
    mpt neighbour;
    switch(direction){
    case 0://left one
      neighbour = c!=0?std::make_pair(r, c-1):std::make_pair(npos,npos);
      break;
    case 1://upper one
      neighbour = r!=0? std::make_pair(r-1, c): std::make_pair(npos,npos);
      break;
    case 2://right one
      neighbour = c!=mcol-1 ? std::make_pair(r, c+1):std::make_pair(npos,npos);
      break;
    case 3://lower one
      neighbour = r!=mrow-1 ? std::make_pair(r+1, c):std::make_pair(npos,npos);
      break;
    default: std::runtime_error("Undefined direction.");
    }
    return neighbour;
  }
  bool is_accessible(mpt const& pt){
    auto [r,c]{pt};
    if (r == npos){
      return false;
    }else{
      return maze[r][c] == '0' || maze[r][c] == 'E';
    }
  }
  
  //internal methods for finding routes of the Maze ends
};//End of Maze class
