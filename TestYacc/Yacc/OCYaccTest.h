/*	OCYaccTest.h
 *
 *		This file was automatically generated by OCYacc, part of the OCTools
 *	suite available at:
 *
 *		https://github.com/w3woody/OCTools
 */

#import <Foundation/Foundation.h>



/*
 *	Class forwards
 */

@class OCYaccTest;

/*
 *	Yacc constants
 */

#define TOKEN                                         0x00110002
#define NUMBER                                        0x00110003

/*
 *	Predefined error codes
 */

// Indicates a warning; set if msg should not halt parsing or return error
#define ERRORMASK_WARNING		0x8000

#define ERROR_SYNTAX			0x0001
#define ERROR_MISSINGTOKEN		0x0002	// { @"token": string of token missing }
#define ERROR_MISSINGTOKENS		0x0003	// { @"tokens": array of token strings }
#define ERROR_STARTERRORID		0x0100	// Your errors should start with this


/*	OCLexInput
 *
 *		The protocol for our lex reader file that the lex stream must
 *	provide. This is the same as the protocol generated as part of the lex
 *	output, and allows us to glue the Lexer and Parser together.
 */

#ifndef OCLexInputProtocol
#define OCLexInputProtocol

@protocol OCLexInput <NSObject>
- (NSInteger)line;
- (NSInteger)column;
- (NSString *)filename;
- (NSString *)text;
- (NSString *)abort;

- (NSInteger)lex;

- (id<NSObject>)value;
@end

#endif

/*	OCYaccTestError
 *
 *		The protocol for our parser for handling errors. As errors take place,
 *	we invoke the method so the error can be recorded and displayed to the
 *	user.
 */

@protocol OCYaccTestError <NSObject>
- (void)errorFrom:(OCYaccTest *)yacc line:(NSInteger)line
		column:(NSInteger)column filename:(NSString *)fname
		errorCode:(NSInteger)errorCode
		data:(NSDictionary<NSString *, id<NSObject>> *)data;
@end


/*	OCYaccTest
 *
 *		The generated parser
 */

@interface OCYaccTest : NSObject

/*
 *	Error processing
 */

@property (weak) id<OCYaccTestError> errorDelegate;

/*
 *	External interfaces
 */

- (instancetype)initWithLexer:(id<OCLexInput>)lexer;

/*
 *	Run parser. If any errors are recorded during parsing, this returns
 *	NO.
 */

- (BOOL)parse;



@end

