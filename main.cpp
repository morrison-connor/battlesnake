//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
using namespace std; // the namespace which includes 'standard' c++ functions such as cout
using namespace nlohmann;

bool SafeMove(json data, int index) {
  cout << "SafeMove invoked";
  cout << "\n\n";
  cout << data["you"];
  cout << "\n\n";

  bool safety;

  if (index == 1) {
    safety = true;
  } else if (index == 3){
    safety = true;
  } else {
    safety = false;
  }

  return safety;
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
    while (SafeMove(data, index) == false) {
      index = rand() % 4;
    }
    res.set_content("{\"move\": \"" + moves[index] + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
