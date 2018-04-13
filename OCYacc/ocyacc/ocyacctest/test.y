/*
 *	Test language, contains four operators and parenthesis, separated by
 *	semicolons
 */

%token <NSNumber> NUMBER
%token <NSString> TOKEN

%left '*' '/'
%left '+' '-'
%right '='

%start statements

%%

statements : statement					{ }
		   | statements statement		{ }
		   ;

statement  : assignment ';'				{ }
		   | error ';'					{ }
		   ;

assignment : TOKEN '=' expression		{ }
		   | TOKEN '=' assignment		{ }
		   ;

expression : '(' expression ')'			{ }
		   | expression '+' expression	{ }
		   | expression '-' expression	{ }
		   | expression '*' expression	{ }
		   | expression '/' expression	{ }
		   | NUMBER						{ }
		   ;
