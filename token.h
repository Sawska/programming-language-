#ifndef TOKEN_H

#define TOKEN_H 
#include  <string>


struct TOKEN
{

    enum OPERATORS {
        PLUS_OPERATOR,
        MINUS_OPERATOR,
        MULTIPLY_OPERATOR,
        DIVIDE_OPERATOR,
        AND_OPERATOR,
        OR_OPERATOR,
        NOT_OPERATOR,
        BIT_AND_OPERATOR,
        BIT_OR_OPERATOR,
        BIT_NOT_OPERATOR,
        GREATER_THAN_OPERATOR,
        LESS_THAN_OPERATOR,
        LEFT_SHIFT_OPERATOR,
        RIGHT_SHIFT_OPERATOR,
        EQUALS_OPERATOR,
        QUESTION_OPERATOR,
        INCREMENT_OPERATOR,
        COMPARE_OPERATOR,
        DECREMENT_OPERATOR,
        UNKNOWN,
        EQUAL_PLUS_OPERATOR,
        EQUAL_MINUS_OPERATOR, 
        EQUAL_DIVIDE_OPERATOR,
        EQUAL_MULTIPLY_OPERATOR,
        LEFT_SHIFT_EQUAL_OPERATOR,
        RIGHT_SHIFT_EQUAL_OPERATOR,
        GREATER_THAN_EQUAL_OPERATOR,
        LESS_THAN_EQUAL_OPERATOR,
        NOT_EQUALS_OPERATOR,
        NEWLINE_OPERATOR,
        SEQUENCE_OPERATOR
    };

    
    enum TOKEN_CONCEPTS {
        NUMBER,
        STRING,
        VARIABLE,
        VARIABLE_NAME,
        OPEN_SQUARE_BRACKETS,
        CLOSE_CIRCLE_BRACKETS,
        OPEN_CIRCLE_BRACKETS,
        CLOSE_SQUARE_BRACKETS,
        OPEN_BRACKETS,
        CLOSE_BRACKETS,
        ERROR_TOKEN,
        EXPRESSION,
        BLOCK,
        BREAK,
        CONTINUE,
        RETURN,
        IF,
        WHILE
    };


    TOKEN_CONCEPTS concept = ERROR_TOKEN;  
    OPERATORS op = UNKNOWN;                
    double number = 0.0;                   
    std::string string;                    
    std::string variableName;              
    std::string expression;


};





#endif //TOKEN_H;