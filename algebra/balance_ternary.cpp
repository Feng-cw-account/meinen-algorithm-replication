#include <vector>
#include <algorithm>
#include <iostream>
using ternary = std::vector<int>;
using bTernary = std::vector<char>;

bTernary dec2ter(int x) {
    bool is_possitive = x>=0;
    x = x > 0 ? x : -x;
    ternary reversed_temp_result;
    for (int reminder3 = x; reminder3 > 0; reminder3 /= 3){
        reversed_temp_result.push_back(reminder3 % 3);
    }

    bTernary rResult;
    
    for (int i = 0; i < reversed_temp_result.size(); ++i) {
        int dig = reversed_temp_result[i];

        if (dig==0) rResult.push_back('0');
        else if (dig == 1) rResult.push_back('1');
        else if (dig == 2) {
            rResult.push_back('z');
            if (i+1 < reversed_temp_result.size()) {
                reversed_temp_result[i+1]++;
            } else {
                reversed_temp_result.push_back(1);
            }
        }
        else if (dig == 3) {
            rResult.push_back('0');
            if (i+1 < reversed_temp_result.size()) {
                reversed_temp_result[i+1]++;
            } else {
                reversed_temp_result.push_back(1);
            }
        }
    }

    // apply sign
    if (!is_possitive){
        for (int idx = 0; idx < rResult.size(); ++idx) {
            if (rResult[idx] == '1') rResult[idx]='z';
            else if (rResult[idx] == 'z') rResult[idx]='1';
        }
    }

    std::reverse(rResult.begin(), rResult.end());
    return rResult;
}

int main() {
    // Test cases
    int test_cases[] = {0, 1, 2, 3, 4, 5, 9, 10, -1, -2, -5, 27};
    
    std::cout << "Decimal to Balanced Ternary Conversion:\n";
    std::cout << "========================================\n";
    
    for (int num : test_cases) {
        bTernary result = dec2ter(num);
        std::cout << "dec2ter(" << num << ") = ";
        for (char c : result) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    
    return 0;
}

