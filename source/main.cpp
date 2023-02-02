#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>

std::map<char, char> basePairs = {{'A', 'T'}, {'G', 'C'}, {'T', 'A'}, {'C', 'G'}}; // base pairs for checking

int main(int args, char** argv){ 
  if (args <= 1) // if there are no args
      return 0;

  std::ifstream f(argv[1]);

  if (!f.is_open()){ // close file and send error if the file fails to load 
    std::cout << "Failed to load file " << argv[1] << std::endl;
    return 0;
  }

  std::string line; // string of current line
  std::string cmd; // current cmd we're building
  std::vector<std::string> cmds; // built cmds
  std::vector<std::string> dArgs; // args for cmd functions

  int i = 0; // current line
  bool arg = false; // if we're collecting arguments

  while (f && std::getline (f, line)){ // get the next line if we're not at the end of the file
    if (cmd.size() == 3){ // if we've built a full command
      cmds.push_back(cmd); // push the command into the command list

      if (cmd == "ATG")
        arg = true; // this is a function which requires args, start collecting args

      cmd.clear(); // clear the current cmd because it's already passed
    }

    if (arg){ 
      dArgs.push_back(line); // push back argument (current line)
      arg = false; // we're not collecting arguments anymore
    }

    else if (line.size()){ // if we're not collecting args and the current line is not empty 
        if (line[3] == basePairs[line[0]]) // make sure the pairs are correct
            cmd.push_back(line[0]); // push back first letter of the pair if the pair is correct

        else{ // send an error if the pairs are not correct
          std::cout << "Incorrect second base pair at line " << i << std::endl;
          return 0; // stop running
        }
    }

    i++; // line count + 1 
  }

  if (!(args > 2 && std::string(argv[2]) == "-c")) // if we're not compiling
    for (int i = 0; i < cmds.size(); i++){ // loop for how many commands we've collected 
      if (cmds[i] == "ATG"){ // printing 
        std::cout << dArgs[0] << std::endl; // print value 
        dArgs.erase(dArgs.begin()); // erase the arg
      }
      
      else if (cmds[i] == "ATC"){ // get input 
        std::string in; 
        std::cin >> in; // collect input into blank obj
      }
        
      else if (cmds[i] == "GCA"){ // if statement
        if (cmds[i + 2] == "TGC"){ // if the if statement is based on ==
          std::array<std::string, 2> vals; // the two values to check 
          
          for (int j = 0; j < 3; j++){ // get the other values
            if (j == 2) 
              j = 3; // set j to 3 for adding to the index
            
            if (cmds[i + j] == "AAA") // 0
              vals[j == 3] = "0"; 

            else if (cmds[i + j] == "GGG") // 1
              vals[j == 3] = "1";

            else if (cmds[i + j] == "ATC") // get input
              std::cin >> vals[j == 3]; 
          }

          i += 3; // add index so we skip these parts
        
          if (vals[0] != vals[1]){ 
            if (cmds[i + 1] == "ATG") // remove next argument if the next line is a command that requires arguments
              dArgs.erase(dArgs.begin()); 
            
            i++; // add index so next line is skipped
          }
        }
      }
    }
  
  else { // we're compiling 
    std::string src = "#include <iostream>\nstd::string input(){std::string s; std::cin >> s; return s;}\n\n\nint main(){\n   "; // source c++ code to be generated

    for (int i = 0; i < cmds.size(); i++){
      if (cmds[i] == "ATG"){ // output
        src += "\n    std::cout << \"" + dArgs[0] + "\" << std::endl;";

        dArgs.erase(dArgs.begin());
      }

      else if (cmds[i] == "ATC") // get input 
        src += "input()";

      else if (cmds[i] == "GCA"){
        src += "\n  if ( "; // open if statement
        
        if (cmds[i + 2] == "TGC"){ // if the if statement is based on ==
          for (int j = 0; j < 3; j++){ // get the other values
            if (j == 2) 
              j = 3; // set j to 3 for adding to the index
            
            if (cmds[i + j] == "AAA") // 0
              src += "\"0\""; 

            else if (cmds[i + j] == "GGG") // 1
              src += "\"1\"";

            else if (cmds[i + j] == "ATC") // get input
              src += "input()";

            if (j == 1) 
              src += " == "; // add == to statement
               
            else if (j == 3)
              src += ")"; // close if statement
          }

          i += 3; // add index so we skip these parts
        } 
      }
    }

    src += "\n}"; // end main function

    FILE* of = fopen("source.cpp", "w+"); // open source file

    fwrite(src.c_str(), 1, src.size(), of); // write source data to source file

    fclose(of); // close file

    system("g++ source.cpp -o output"); // compile source file
    system("rm source.cpp"); // remove source file
  }
}