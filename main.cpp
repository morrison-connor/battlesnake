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


std::vector<string> search_food(json data, std::vector<string> safe_moves) {
  // TODO: get_closest_food_coordinates

  std::vector<string> chosen_moves = {};
  int food_x = data["board"]["food"][0]["x"];
  int food_y = data["board"]["food"][0]["y"];
  int head_x = data["you"]["head"]["x"];
  int head_y = data["you"]["head"]["y"];
  cout << "food_x: " << food_x << "food_y: " << food_y << "\n\n";
  cout << "head_x: " << head_x << "head_y: " << head_y << "\n\n";

  // Choose moves to search food
  for (int i = 0; i < safe_moves.size(); i++) {  // Could simplify this to be an OR statement as the result is the same
    if ((safe_moves[i] == "up" && food_y > head_y) || (safe_moves[i] == "down" && food_y < head_y)) {
      chosen_moves.push_back(safe_moves[i]);
    } else if (safe_moves[i] == "down" && food_y < head_y) {
      chosen_moves.push_back(safe_moves[i]);
    }

    if ((safe_moves[i] == "left" && food_x < head_y) || (safe_moves[i] == "right" && food_y > head_y)) {
      chosen_moves.push_back(safe_moves[i]);
    }
  }
  cout << "chosen_moves_size: " << chosen_moves.size() << "\n\n";
  return chosen_moves;
}


string choose_move(json data, std::vector<string> safe_moves) {

  cout << "Check moves invoked" << "\n\n";

  int index;
  string move;
  std::vector<string> chosen_moves = {};

  if (data["board"]["food"].size() > 0) {
    cout << "Searching for food..." << "\n\n";
    chosen_moves = search_food(data, safe_moves);
  }

  if (chosen_moves.size() > 0) {
    index = rand() % chosen_moves.size();
    move = chosen_moves[index];
  } else { // fail-safe if no move is selected
    index = rand() % safe_moves.size();
    move = safe_moves[index];
  }
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
