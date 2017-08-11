//
//  OCYaccParser.h
//  ocyacc
//
//  Created by William Woody on 7/25/17.
//  Copyright © 2017 Glenview Software. All rights reserved.
//

#ifndef OCYaccParser_h
#define OCYaccParser_h

#include <OCLexer.h>

#include <stdio.h>
#include <map>
#include <list>
#include <vector>
#include <utility>
#include <string>

/************************************************************************/
/*																		*/
/*	Parser Definitions													*/
/*																		*/
/************************************************************************/

/*	OCYaccParser
 *
 *		Input stream parser; converts language definition into an abstract
 *	syntax tree for processing
 */

class OCYaccParser
{
	public:
		OCYaccParser();
		~OCYaccParser();

		bool ParseFile(OCLexer &lex);

		/*
		 *	Definitions
		 */

		std::string declCode;			// include section of .m
		std::string classHeader;		// class declaration of .h
		std::string classLocal;			// class declaration of .m
		std::string classGlobal;		// class declaration of .h
		std::string endCode;			// code section of .m

		/*
		 *	Associativity, if declared for terminal symbols
		 */

		enum Assoc {
			None, Left, Right, NonAssoc
		};

		struct Precedence {
			uint16_t prec;
			Assoc assoc;
			std::string type;
		};

		/*
		 *	SymbolInstance
		 *
		 *		Represents a single instance of a token declaration in
		 *	the CC section.
		 */

		struct SymbolInstance {
			std::vector<std::string> tokenlist;
			std::string code;
		};

		/*
		 *	SymbolDecl
		 *
		 *		Contains a list of all the declarations for a given CC
		 *	symbol
		 */

		struct SymbolDecl {
			std::vector<SymbolInstance> declarations;
		};

		/*
		 *	Parser results
		 */

		// associativity list. Shows %left, %right, %nonassoc mapping if
		// declared
		std::map<std::string,Precedence> terminalSymbol;

		// Type name for production
		std::map<std::string,std::string> symbolType;

		// Code declaration section
		std::map<std::string,SymbolDecl> symbols;

		// Start symbol
		std::string startSymbol;

	private:
		void SkipToNextDeclaration(OCLexer &lex);
		bool ParseDeclarations(OCLexer &lex);

		void SkipToNextRule(OCLexer &lex);
		bool ParseRules(OCLexer &lex);
};


#endif /* OCYaccParser_h */
