//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
using namespace std; // the namespace which includes 'standard' c++ functions such as cout
using namespace nlohmann;

struct Head {
    int x = 0;
    int y = 0;
};

bool SafeMove(json data, struct Head h, int index) {
  int next_x = h.x;
  int next_y = h.y;

  cout << "SafeMove invoked";
  cout << "\n\n";
  cout << data["you"];
  cout << "\n\n";

  if (index == 0) {
    ++next_y;
  } else  if (index == 1) {
    --next_y;
  } else if (index == 2){
    ++next_x;
  } else {
    --next_x;
  }

  if (next_x < 0 || next_x == data["board"]["width"] || next_y < 0 || next_y == data["board"]["height"]) {
    return false;
  }
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
    struct Head h;
    h.x = data["you"][0][0];
    h.y = data["you"][0][1];

    cout << data;
    cout << "\n\n";
    
    
    //You can get the "you" property like this:
    //data["you"];
    //Almost alike python dictionary parsing, but with a semicolon at the end of each line.
    //You might need to make some structs to store some data in a variable
    //Example:
    //you_struct you = data["you"];
  
    std::vector<string> moves {"up", "down", "left", "right"};

    int index = rand() % 4;
    while (SafeMove(data, h, index) == false) {
      index = rand() % 4;
    }
    res.set_content("{\"move\": \"" + moves[index] + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
