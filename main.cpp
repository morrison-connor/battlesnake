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

bool SafeMove(json data, Head h, int index) {
  int next_x = h.x;
  int next_y = h.y;

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
  } else if (next_x == data["you"]["body"][1]["x"] && next_y == data["you"]["body"][1]["y"]) {
    return false;
  }
  cout << "Neck x: " << data["you"]["body"][1]["x"] << " Neck y: " << data["you"]["body"][1]["y"] << "\n\n";

  return true;
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
    const json data = json::parse(req.body);

    cout << "START OF MOVE #: " << data["turn"] << "\n\n";    
    cout << "DATA: " << data << "\n\n";

    Head h;
    h.x = data["you"]["head"]["x"];
    h.y = data["you"]["head"]["y"];

    //You can get the "you" property like this:
    //data["you"];
    //Almost alike python dictionary parsing, but with a semicolon at the end of each line.
    //You might need to make some structs to store some data in a variable
    //Example:
    //you_struct you = data["you"];
  
    std::vector<string> moves {"up", "down", "left", "right"};

    int index = rand() % 4;
    cout << "Is the move " << moves[index] << " safe? " << SafeMove(data, h, index) << "\n\n";
    if (SafeMove(data, h, index) == false) {
      index = rand() % 4;
    }

    cout << "Move selected: " << moves[index] << "\n\n";

    res.set_content("{\"move\": \"" + moves[index] + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
