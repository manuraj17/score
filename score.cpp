#include <iostream>
#include <curl/curl.h>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

string data;

unordered_map<string, int> scores { { "IssueEvent", 7},
                                    { "IssueCommentEvent", 6},
                                    { "PushEvent", 5}, 
                                    { "PullRequestReviewCommentEvent", 4},
                                    { "WatchEvent", 3},
                                    { "CreateEvent", 2},
                                    { "other", 1}};

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

  for (unsigned int c = 0; c<size*nmemb; c++)
  {
      data.push_back(buf[c]);
  }
  return size*nmemb; //tell curl how many bytes we handled
}

string createURL(string username) {
  return "https://api.github.com/users/" + username + "/events/public";
}

void fetch(string target){

  string target_url = createURL(target);
  const char *url = target_url.c_str();

  CURL* curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  curl_global_cleanup();

}

int getScore(string event) {

  if (scores.find(event) == scores.end()) {
    return 1;
  } else {
    return scores[event];
  }
}

int calculateScore() {
  auto result = json::parse(data);
  int total = 0;
  for (json::iterator it  = result.begin(); it != result.end(); ++it) {
    total += getScore((*it)["type"]); 
  }
  return total;
}

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    cout<< "Usage: "<< argv[0] << " <username>" << endl;
    exit(0);
  }
  
  string target = argv[1];
  fetch(target);
  int total = calculateScore();
  transform(target.begin(), target.end(), target.begin(), ::toupper);
  cout << target << "'s score is " << total << endl;
  return 0;
}

