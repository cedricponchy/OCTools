/*	Example.h
 *
 *		This file was automatically generated by OCYacc, part of the OCTools
 *	suite available at:
 *
 *		https://github.com/w3woody/OCTools
 */

#ifndef Example_h
#define Example_h

#include <stdint.h>
#include <map>
#include <vector>
#include <string>

    %header


    %%


/*
 *	Yacc constants
 */

#define NUMBER                                        0x00110002

#ifndef Example2_ValueDefined
#define Example2_ValueDefined

/*  Example2Value
 *
 *      Internally defined value.
 */

union Example2Value {
    
    int value;
};

#endif

/*
 *	Predefined error codes
 */

// Indicates a warning; set if msg should not halt parsing or return error
#define ERRORMASK_WARNING		0x8000

#define ERROR_SYNTAX			0x0001
#define ERROR_MISSINGTOKEN		0x0002	// { @"token": string of token missing }
#define ERROR_MISSINGTOKENS		0x0003	// { @"tokens": array of token strings }
#define ERROR_STARTERRORID		0x0100	// Your errors should start with this

class Example2;

/*
 *	Internal parser stack
 */

struct ExampleStack
{
	uint16_t state;
	int32_t line;
	int32_t column;
	std::string filename;
	union Example2Value value;
};

/*	Example
 *
 *		The generated parser
 */

class Example
{
	public:
		Example(Example2 *lexer);
		virtual ~Example(void);

		virtual void error(int32_t line, int32_t col, std::string fname, int32_t errCode, std::map<std::string,std::string> &map);

		bool parse(void);


    %global

	private:
		std::vector<ExampleStack> stack;
		Example2 *lex;
		bool success;
		int32_t errorCount;

		ExampleStack processReduction(int16_t rule);
		int32_t actionForState(int32_t state, int32_t token);
		int32_t gotoForState(int32_t state, int32_t token);
		void errorWithCode(int32_t code, std::map<std::string,std::string> &data);
		void errorWithCode(int32_t code);
		void errorOK();
		std::string tokenToString(uint32_t token);
		bool reduceByAction(int16_t action);

    %local
};

#endif
