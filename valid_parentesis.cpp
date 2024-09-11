#include <string>
#include "stack.cpp"

using namespace std;

class Solution {
public:
    bool isValid(string s) {
        Stack<int> mystack;

        for (char c : s) {
            if (c == '[' || c == '{' || c == '(' ){
                mystack.push(c);
            }
            if (c == '}' || c == ']' || c == ')' ){
                if (mystack.isempty()) {
                    return false;
                }
                else{
                    if (c == '}' && mystack.tops() == '{'){
                        mystack.pop();
                    }
                    else if (c == ']' && mystack.tops() == '['){
                        mystack.pop();
                    }
                    else if (c == ')' && mystack.tops() == '('){
                        mystack.pop();
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        if (mystack.isempty()){
            return true;
        }
    }
};

int main() {
    Solution solution;
    std::string testString = "({[]}))";
    bool result = solution.isValid(testString);
    std::cout << "La cadena " << testString << " es " << (result ? "valida" : "invalida") << std::endl;

    return 0;
}