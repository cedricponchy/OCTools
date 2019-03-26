/*	Example.m
 *
 *		This file was automatically generated by OCYacc, part of the OCTools
 *	suite available at:
 *
 *		https://github.com/w3woody/OCTools
 */

#import "Example.h"


    %%

/*
 *	Note: this is a baseline implementation for an LR parser as described in
 *	Aho, Sethi, Ullman: "Compilers: Principles, Techniques and Tools", (the
 *	Dragon Book), pages 216-220. Unlike the example there we don't bother 
 *	storing the tokens on the token stack. We also take some liberties to
 *	compress our tables, and do a few optimizations similar to Bison.
 *
 *	Table compression scheme comes from
 *
 *	https://en.wikipedia.org/wiki/Sparse_matrix
 *
 *	A discussion of the Bison internals comes from
 *
 *	https://www.cs.uic.edu/~spopuri/cparser.html
 */

/************************************************************************/
/*                                                                      */
/*  State Tables and Constants                                          */
/*                                                                      */
/************************************************************************/

// Various constants
#define K_ACCEPTSTATE       3            // Final accept state
#define K_EOFTOKEN          0x110000     // EOF token ID
#define K_ERRORTOKEN        0x110001     // Error token ID
#define K_FIRSTTOKEN        0x110002     // Error token ID
#define K_MAXSYMBOL         0x110005     // Max ID for all symbols
#define K_STARTSTATE		0            // Start state is always 0

/*  TokenArray
 *
 *      Array of non-Unicode token values for error reporting
 */

static const NSString *TokenArray[] = {
    @"NUMBER"
};
/*  RuleLength
 *
 *      The number of tokens a reduce action removes from the stack
 */

static const uint8_t RuleLength[2] = {
     1,  3
};

/*  RuleProduction
 *
 *      The prodution ID we reduce to
 */

static const uint32_t RuleProduction[2] = {
    0x110003, 0x110004
};

/*
 *  The following tables are compressed using CSR format.
 *  See https://en.wikipedia.org/wiki/Sparse_matrix
 */

/*  ActionI, J, A
 *
 *      Compressed action index table.
 */

static const uint32_t ActionI[7] = {
         0,      1,      2,      3,      4,      5,      6
};

static const uint32_t ActionJ[6] = {
    0x110002,      '+', 0x110000, 0x110000, 0x110002, 0x110000
};

// Note: < 0 -> reduce (rule = -a-1), >= 0 -> shift (state).
static const int16_t ActionA[6] = {
         1,      4,      3,     -1,      5,     -2
};

/*  GotoI, J, A
 *
 *      Compressed goto table.
 */

static const uint32_t GotoI[7] = {
         0,      1,      1,      1,      1,      1,      1
};

static const uint32_t GotoJ[1] = {
    0x110004
};

static const int16_t GotoA[1] = {
         2
};

/************************************************************************/
/*                                                                      */
/*  Internal Structures													*/
/*                                                                      */
/************************************************************************/

/*
 *	Internal parser stack
 */

@interface ExampleStack: NSObject
@property (assign) uint16_t state;
@property (assign) NSInteger line;
@property (assign) NSInteger column;
@property (copy) NSString *filename;

/* Represent the intermediate values for reduction rule values */
@property (strong) id<NSObject> value;
@end

@implementation ExampleStack
@end

/************************************************************************/
/*                                                                      */
/*  Parser Code															*/
/*                                                                      */
/************************************************************************/

/*
 *	Class internals
 */

@interface Example ()
@property (strong) NSMutableArray<ExampleStack *> *stack;
@property (strong) id<OCLexInput> lex;

// Error support
@property (assign) BOOL success;

@property (assign) NSInteger errorCount;

// Yacc class declarations

    %local

@end

/*
 *	Generated class
 */

@implementation Example

/*
 *	Startup
 */

- (instancetype)initWithLexer:(id<OCLexInput>)lexer
{
	if (nil != (self = [super init])) {
		self.lex = lexer;

		/*
		 *	Alloc stack
		 */

		self.stack = [[NSMutableArray alloc] init];

    %init

	}
	return self;
}

- (void)dealloc
{

    %finish

}


CODE

/*
 *	Process production rule. This processes the production rule and creates
 *	a new stack state with the rule reduction.
 */

- (ExampleStack *)processReduction:(NSInteger)rule
{
	// Get production len, for translating $1...$N into something useful
	NSInteger pos = self.stack.count - RuleLength[rule];

	// Set up initial state.
	ExampleStack *s = [[ExampleStack alloc] init];

	// File position of reduced rule is first token of the symbols reduced
	ExampleStack *fs = self.stack[pos];
	s.filename = fs.filename;
	s.line = fs.line;
	s.column = fs.column;

	// Now process production.
	//
	// Note that $$ translated into (s.value), and
	// $n translates into ((<type> *)(self.stack[pos+(n-1)])), where <type>
	// is the declared type of the token or production rule.

	@try {
		switch (rule) {

        // Production rules
            // (110003) $accept : add $end
            // (110004) add : NUMBER '+' NUMBER 
            case 1:
                {
                s.value = ((value *)(self.stack[pos].value)) + ((value *)(self.stack[pos + 2].value)); 
        
                }
                break;


			default:
				break;
		}
	}
	@catch (NSException *exception) {
		/* This can happen in the event we start seeing errors */
	}

	return s;
}

/*
 *	Look up the action value for the state and token values provided. This
 *	parses through the ActionI, ActionJ and ActionA lists to find the correct
 *	entry. This returns NSIntegerMax if a value was not found, and this is an
 *	error entry.
 */

- (NSInteger)actionForState:(NSInteger)state token:(NSInteger)token
{
	size_t min,max,mid;

	/* Deal with EOF case */
	if (token == -1) token = K_EOFTOKEN;

	/* Find range */
	min = ActionI[state];
	max = ActionI[state+1];

	/* Binary search for value in ja */
	while (min < max) {
		mid = (min + max)/2;
		NSInteger j = ActionJ[mid];
		if (token == j) {
			return ActionA[mid];
		} else if (token < j) {
			max = mid;
		} else {
			min = mid+1;
		}
	}
	return NSIntegerMax;
}

/*
 *	Look up the goto value for the state and token (production rule) provided.
 *	Operates the same as actionForState, except we look in the goto table.
 *	In teory I guess we could combine these two tables (as productions and
 *	tokens do not overlap).
 */

- (NSInteger)gotoForState:(NSInteger)state production:(NSInteger)token
{
	size_t min,max,mid;

	/* Find range */
	min = GotoI[state];
	max = GotoI[state+1];

	/* Binary search for value in ja */
	while (min < max) {
		mid = (min + max)/2;
		NSInteger j = GotoJ[mid];
		if (token == j) {
			return GotoA[mid];
		} else if (token < j) {
			max = mid;
		} else {
			min = mid+1;
		}
	}
	return NSIntegerMax;
}

/*
 *	Errors. This formats and prints the specified error
 */

- (void)errorWithCode:(NSInteger)code data:(NSDictionary<NSString *, id<NSObject>> *)data
{
	if (self.errorDelegate == nil) return;	// No error handling, ignore.

	if (self.errorCount > 0) return;		// skip until synced on 3 shifts

	// Call delegate with current token position
	// Token position is the topmost symbol
	ExampleStack *top = self.stack.lastObject;
	[self.errorDelegate errorFrom:self line:top.line column:top.column filename:top.filename errorCode:code data:data];

	// And now skip the next 3 token shifts so we don't spew garbage.
	if (0 == (code & ERRORMASK_WARNING)) {
		self.success = NO;
		self.errorCount = 3;
	}
}

- (void)errorWithCode:(NSInteger)code
{
	[self errorWithCode:code data:nil];
}

- (void)errorOK
{
	self.errorCount = 0;
}

- (const NSString *)tokenToString:(uint32_t)token
{
	if (token >= K_FIRSTTOKEN) {
		return TokenArray[token - K_FIRSTTOKEN];
	} else if (token <= 0xFFFF) {
		return [NSString stringWithFormat:@"%C",(unichar)token];
	} else {
		// Extended UTF-32 character.
		unichar c[2];
		token -= 0x10000;	// Convert to two UTF-16 words
		c[0] = 0xD800 | (0x3FF & (token >> 10));
		c[1] = 0xDC00 | (0x3FF & token);
		return [NSString stringWithCharacters:c length:2];
	}
}

- (BOOL)reduceByAction:(int16_t)action
{
	// Determine the new state we're transitioning to.
	uint32_t production = RuleProduction[action];
	NSInteger length = RuleLength[action];

	// We pull the state we'd have after popping the stack.
	NSInteger statep = self.stack[[self.stack count] - length - 1].state;

	// Now calculate the state we should transition to
	NSInteger newState = [self gotoForState:statep production:production];
	if (newState == NSIntegerMax) return NO;

	// Process production rule, which generates the new state
	ExampleStack *state = [self processReduction:action];

	// Update state
	state.state = newState;

	// Pop the stack
	[self.stack removeObjectsInRange:NSMakeRange(self.stack.count - length, length)];

	// Push new state
	[self.stack addObject:state];

	// Done.
	return YES;
}

/*
 *	Parser engine. Returns NO if there was an error during processing. Note
 *	that as we uncover errors we call our delegate for error handling. This
 *	implements the algorithm described in the Dragon Book, Algorithm 4.7.
 */

- (BOOL)parse
{
	ExampleStack *s;				// state
	NSInteger a;				// lex symbol

	/*
	 *	Step 1: reset and push the empty state.
	 */

	self.success = YES;
	[self.stack removeAllObjects];

	s = [[ExampleStack alloc] init];
	s.state = K_STARTSTATE;

	s.filename = self.lex.filename;
	s.line = self.lex.line;
	s.column = self.lex.column;

	[self.stack addObject:s];

	/*
	 *	Now repeat forever:
	 */

	a = [self.lex lex];

	for (;;) {
		s = [self.stack lastObject];

		/*
		 *	Determine if this is the end state. If so, then we immediately
		 *	quit. We assume the user has set the production rule at the 
		 *	top, so we can simply drop the stack
		 */

		if (s.state == K_ACCEPTSTATE) {
			[self.stack removeAllObjects];
			return self.success;
		}

		/*
		 *	Now determine the action and shift, reduce or handle error as
		 *	appropriate
		 */

		NSInteger action = [self actionForState:s.state token:a];

		if (action == NSIntegerMax) {
			/*
			 *	Handle error. First, note we have an error, and note the
			 *	symbol on which our error took place.
			 */

			self.success = NO;		// regardless, we will always fail.

			/*
			 *	First, scan backwards from the current state, looking for one
			 *	which has an 'error' symbol.
			 */

			NSInteger ix = self.stack.count;
			while (ix > 0) {
				ExampleStack *si = self.stack[--ix];
				action = [self actionForState:si.state token:K_ERRORTOKEN];
				if ((action >= 0) && (action != NSIntegerMax)) {
					/*
					 *	Encountered error state. If the user has defined an
					 *	error token, we ultimately will want to (a) unwind
					 *	the stack until we find a state which handles the
					 *	error transition. We then .
					 */

					NSRange range = NSMakeRange(ix+1, self.stack.count - ix - 1);
					if (range.length) {
						[self.stack removeObjectsInRange:range];
					}

					/*
					 *	At this point we perform a shift to our new error
					 *	state.
					 */

					s = [[ExampleStack alloc] init];
					s.state = action;
					s.value = self.lex.value;

					s.filename = self.lex.filename;
					s.line = self.lex.line;
					s.column = self.lex.column;

					[self.stack addObject:s];

					/*
					 *	Second, we start pulling symbols until we find a symbol
					 *	that shifts, or until we hit the end of file symbol. 
					 *	This becomes our current token for parsing
					 */

					for (;;) {
						a = [self.lex lex];
						action = [self actionForState:s.state token:a];
						if ((action >= 0) && (action != NSIntegerMax)) {
							/*
							 *	Valid shift. This becomes our current token,
							 *	and we resume processing.
							 */

							continue;

						} else if (action == K_EOFTOKEN) {
							/*
							 *	We ran out of tokens. At this point all
							 *	we can do is print an error and force quit.
							 */

							[self errorWithCode:ERROR_SYNTAX];
							[self.stack removeAllObjects];

							return false;
						}
					}
				}
			}

			/*
			 *	If we reach this point, there is no error we can recover to.
			 *	So figure this out on our own.
			 *
			 *	First, we see if the state we're in has a limited number of
			 *	choices. For example, in C, the 'for' keyword will always be
			 *	followed by a '(' token, so we can offer to automatically 
			 *	insert that token.
			 */

			NSInteger actionMin = ActionI[s.state];
			NSInteger actionMax = ActionI[s.state + 1];
			NSInteger actionVal = actionMin;
			int16_t actionState = -1;
			for (NSInteger ix = actionMin; ix < actionMax; ++ix) {
				int16_t act = ActionA[ix];
				if (actionState == -1) {
					if (act >= 0) {
						actionState = act;
						actionVal = ix;
					}
				} else {
					actionState = -1;
					break;
				}
			}

			if (actionState != -1) {
				/*
				 *	We can accomplish this transition with one token. Print
				 *	an error, and do a shift on the state with an empty value.
				 */

				const NSString *tokenStr = [self tokenToString:ActionJ[actionVal]];
				[self errorWithCode:ERROR_MISSINGTOKEN data:@{ @"token": tokenStr} ];

				/*
				 *	Perform a shift but do not pull a new token
				 */

				ExampleStack *top = self.stack.lastObject;

				ExampleStack *stack = [[ExampleStack alloc] init];
				stack.state = actionState;
				stack.value = self.lex.value;

				stack.filename = top.filename;
				stack.line = top.line;
				stack.column = top.column;

				[self.stack addObject:stack];
				continue;
			}

			/*
			 *	See if we have a limited choice in reductions. If this can
			 *	only reduce to a single state, try that reduction.
			 */

			actionState = 0;
			for (NSInteger ix = actionMin; ix < actionMax; ++ix) {
				int16_t act = ActionA[ix];
				if (actionState == 0) {
					if ((act < 0) && (actionState != act)) {
						actionState = act;
					}
				} else {
					actionState = 0;
					break;
				}
			}

			if (actionState != 0) {
				/*
				 *	We have one possible reduction. Try that. Note that this
				 *	will trigger a syntax error since we're reducing down 
				 *	without the follow token. My hope is that the state we
				 *	transition to has a limited set of next tokens to follow.
				 */

				[self reduceByAction:action];
				continue;
			}

			/*
			 *	If we have a limited number of tokens which can follow, 
			 *	print a list of them. Then shift by the first one we
			 *	find. We don't do this if the number of shifts is greater
			 *	than five.
			 */

			if (actionMax - actionMin <= 5) {
				NSMutableArray<const NSString *> *list = [[NSMutableArray alloc] init];
				for (NSInteger ix = actionMin; ix < actionMax; ++ix) {
					[list addObject:[self tokenToString:ActionJ[ix]]];
				}

				[self errorWithCode:ERROR_MISSINGTOKENS data:@{ @"tokens": list }];

				/*
				 *	Now we artificially insert the first of the list of
				 *	tokens as our action and continue.
				 */

				a = ActionJ[actionMin];
				continue;
			}

			/*
			 *	If we get here, things just went too far south. So we
			 *	skip a token, print syntax error and move on
			 */

			[self errorWithCode:ERROR_SYNTAX];
			a = [self.lex lex];
			if (a == -1) return false;

		} else if (action >= 0) {
			/*
			 *	Shift operation.
			 */

			// Shift
			ExampleStack *stack = [[ExampleStack alloc] init];
			stack.state = action;
			stack.value = self.lex.value;

			stack.filename = self.lex.filename;
			stack.line = self.lex.line;
			stack.column = self.lex.column;

			[self.stack addObject:stack];

			// Advance to next token.
			a = [self.lex lex];

			// Decrement our error count. If this is non-zero we're in an
			// error state, and we don't pass spurrous errors upwards
			if (self.errorCount) {
				self.errorCount = self.errorCount-1;
			}

		} else {
			/*
			 *	Reduce action. (Reduce is < 0, and the production to reduce
			 *	by is given below
			 */

			action = -action-1;

			if (![self reduceByAction:action]) {
				// If there is an error, this handles the error.
				// (This should not happen in practice).
				[self errorWithCode:ERROR_SYNTAX];

				// Advance to next token.
				a = [self.lex lex];
			}
		}
	}
}

@end
