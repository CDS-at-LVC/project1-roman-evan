#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <streambuf>

using namespace std;

int main() {
    ifstream input{"../vcpkg.json"};

    string fileText((istreambuf_iterator<char>(input)),
                    istreambuf_iterator<char>());

    using json = nlohmann::json;

    json j = json::parse(fileText);

    cout << j << endl;
    return 0;
}
