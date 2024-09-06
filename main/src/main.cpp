#include <iostream>
#include "api/UnderlyingAsset.h"
#include "util/Token.h"
#include "arbitrage/putcall.h"
#include <vector>

using namespace std;

int main(){


    token::Token& token = token::Token::getInstance();
    token.initialize();
    
    put_call::PutCall& putcall = put_call::PutCall::getInstance();

    /* 만기일이 같은 선물옵션 종목코드 생성하기 */

    // 1. 시중에 존재하는 옵션월물리스트를 가져온 후 
    std::vector<std::string> option_list = putcall.getOptionList();


    // 2. 각 리스트에 대해 선물, 콜 풋 종목코드 생성
    std::map<std::string, std::string> all_code = putcall.getAllCode(option_list);

    for (const auto& pair : all_code) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    
    return 0;
}