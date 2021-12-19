//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
using namespace std; // the namespace which includes 'standard' c++ functions such as cout
using namespace nlohmann;

// Battlesnake Testing Guide:
// Stop & Rerun replit, waiting for snake info to appear on top-right. Then refresh game.

struct Head {
    int x = 0;
    int y = 0;
};

bool SafeMove(json data, int index) {
  int next_x = data["you"]["head"]["x"];
  int next_y = data["you"]["head"]["y"];

  if (index == 0) {
    ++next_y;
  } else  if (index == 1) {
    --next_y;
  } else if (index == 2){
    --next_x;
  } else {
    ++next_x;
  }

  cout << "Next x: " << next_x << " Next y: " << next_y << "\n\n";

  if ( next_x < 0 || next_x == data["board"]["width"]
  || next_y < 0  || next_y == data["board"]["height"]) {
    return false;
  }

  cout << "Snake Length: " << data["you"]["body"].size() << "\n\n";

  for (int segment = 0; segment < data["you"]["body"].size(); segment++) {
    if (next_x == data["you"]["body"][segment]["x"] && next_y == data["you"]["body"][segment]["y"]) {
      return false;
    } else {
      continue;
    }
  }

  for (int segment = 0; segment < data["you"]["body"].size(); segment++) {
    if (next_x == data["you"]["body"][segment]["x"] && next_y == data["you"]["body"][segment]["y"]) {
      return false;
    } else {
      continue;
    }
  }

  return true;
}

std::vector<string> CheckMoves(json data, string moves[4]) {
  cout << "CheckMoves invoked" << "\n\n";
  std::vector<string> safeMoves = {};

  for (int i = 0; i < 4; i++) {
      cout << "Is the move " << moves[i] << "-" << i<< " safe? " << "\n\n";
    if (SafeMove(data, i) == true) {
      cout << "Move is safe." << "\n\n";
      safeMoves.push_back(moves[i]);
    } 
  }
  return safeMoves;
}

string ChooseMove(json data, std::vector<string> safeMoves) {
  int index = rand() % safeMoves.size();
  string move = safeMoves[index];
  cout << "Move selected: " << move << "\n\n";
  return move;
}

int main(void) {
  httplib::Server svr;
  svr.Get("/", [](const auto &, auto &res) {
    string head = "bendr";
    string tail = "rbc-necktie";
    string author = "Connor-Morrison";
    string color = "#a676c2";
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "application/json");
  });
  svr.Post("/end", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  svr.Post("/start", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  svr.Post("/move", [](auto &req, auto &res){
    // DECLARE VARIABLES
    const json data = json::parse(req.body);
    string moves[4] = {"up", "down", "left", "right"};

    // ACCESSING DATA
    //You can get the "you" property like this:
    //data["you"];
    //Almost alike python dictionary parsing, but with a semicolon at the end of each line.
    //You might need to make some structs to store some data in a variable
    //Example:
    //you_struct you = data["you"];
    cout << "START OF MOVE #: " << data["turn"] << "\n\n";    
    cout << "DATA: " << data << "\n\n";

    // CHECK SAFE MOVES
    std::vector<string> safeMoves = CheckMoves(data, moves);

    // CHOOSE A MOVE
    string chosenMove = ChooseMove(data, safeMoves);

    // RETURN MOVE CHOICE
    res.set_content("{\"move\": \"" + chosenMove + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
