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

bool safe_move(json data, int index) {
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

  // Check for collisions against snakes on board
  for (int snake = 0; snake < data["board"]["snakes"].size(); snake++) {
    for (int segment = 0; segment < data["board"]["snakes"][snake]["body"].size(); segment++) {
      if (next_x == data["board"]["snakes"][snake]["body"][segment]["x"] && next_y == data["board"]["snakes"][snake]["body"][segment]["y"]) {
        return false;
      } else {
        continue;
      }
    }
  }

  return true;
}

std::vector<string> check_moves(json data, string moves[4]) {
  cout << "check_moves invoked" << "\n\n";
  std::vector<string> safe_moves = {};

  for (int i = 0; i < 4; i++) {
      cout << "Is the move " << moves[i] << "-" << i<< " safe? " << "\n\n";
    if (safe_move(data, i) == true) {
      cout << "Move is safe." << "\n\n";
      safe_moves.push_back(moves[i]);
    } 
  }
  return safe_moves;
}

string choose_move(json data, std::vector<string> safe_moves) {

  // get_closest_food_coordinates
  chosen_moves = 
  /* logic tree

  if food_x < head_x
    then
  else 
  */

  int index = rand() % safe_moves.size();
  string move = safe_moves[index];
  cout << "Move selected: " << move << "\n\n";
  return move;
}



int main(void) {
  cout << "Ready for battle!" << "\n\n";
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
    const json data = json::parse(req.body);
    //You can get the "you" property like this:
    //data["you"];
    //Almost alike python dictionary parsing, but with a semicolon at the end of each line.
    //You might need to make some structs to store some data in a variable
    //Example:
    //you_struct you = data["you"];

    cout << "START OF MOVE #: " << data["turn"] << "\n\n";    
    cout << "DATA: " << data << "\n\n";

    string moves[4] = {"up", "down", "left", "right"};
    
    // CHECK SAFE MOVES
    std::vector<string> safe_moves = check_moves(data, moves);

    // CHOOSE A MOVE
    string chosenMove = choose_move(data, safe_moves);

    res.set_content("{\"move\": \"" + chosenMove + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
