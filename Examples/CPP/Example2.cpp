/*	Example2.cpp
 *
 *		This file was automatically generated by OCLex, part of the OCTools
 *	suite available at:
 *
 *		https://github.com/w3woody/OCTools
 */

#include "Example2.h"
#include <stdlib.h>
#include <string.h>
#include <new>
/************************************************************************/
/*                                                                      */
/*  Lex Transition State Tables                                         */
/*                                                                      */
/************************************************************************/

/*
 *  Constants
 */

#define MAXSTATES       3
#define MAXCHARCLASS    2
#define MAXACTIONS      2


//  Character classes used during parsing:
//
//  0: N
//  1: [\000-MO-\377]

/*  CharClass
 *
 *      Maps 8-bit character to character class
 */

static uint16_t CharClass[256] = {
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   0,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1, 
      1,   1,   1,   1,   1,   1,   1,   1
};

/*  StateActions
 *
 *      Maps states to actions. MAXACTION if this is not a terminal
 */

static uint16_t StateActions[3] = {
      2,   0,   1
};

/*  StateMachineIA, StateMachineJA, StateMachineA
 *
 *      Lex state machine in compressed sparce row storage format. We do this
 *  in order to compact the resulting sparse matrix state machine so we don't
 *  consume as much space. Decoding the new state becomes an O(log(N)) process
 *  on the input character class as we use a binary search on the JA array.
 *
 *      See the article below for more information:
 *
 *      https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_.28CSR.2C_CRS_or_Yale_format.29
 */

static uint16_t StateMachineIA[4] = {
      0,   2,   2,   2
};

static uint16_t StateMachineJA[2] = {
      0,   1
};

static uint16_t StateMachineA[2] = {
      1,   2
};


/************************************************************************/
/*                                                                      */
/*  Construction/Destruction                                            */
/*                                                                      */
/************************************************************************/

/*	Example2::Example2
 *
 *		Constructor
 */

Example2::Example2(OCFileInput *f)
{
	file = f;

	isMarked = false;

	markSize = 0;
	markAlloc = 256;
	markBuffer = (unsigned char *)malloc(markAlloc);

	readPos = 0;
	readSize = 0;
	readAlloc = 256;
	readBuffer = (unsigned char *)malloc(readAlloc);

	textMarkSize = 0;
	textSize = 0;
	textAlloc = 256;
	textBuffer = (unsigned char *)malloc(textAlloc);

	states = 0;
%init

}

/*	Example2::~Example2
 *
 *		Destructor
 */

Example2::~Example2(void)
{
	if (markBuffer) free(markBuffer);
	if (readBuffer) free(readBuffer);
	if (textBuffer) free(textBuffer);
%finish

}

/*
 *	Internal read backtrack support. We implement the internal methods
 *	for mark, unmark and reset to note the current file position. This is
 *	used by the lexer to unwind the read queue if we overread the number
 *	of characters during parsing
 *
 *	In essence, as we execute the DFA while we read characters, when we
 *	find an end state, we mark the stream. Then we continue to read until
 *	we reach an error--at which point we rewind back to the mark location.
 */

/*	Example2::mark
 *
 *		Mark: note that we should cache characters being read so we can rewind
 *	to this location in the future
 */

void Example2::mark(void)
{
	/*
	 *	Store the current file location and enable marking. This has the side
	 *	effect of flushing any previous mark buffer
	 */

	markLine = curLine;
	markColumn = curColumn;
	textMarkSize = textSize;

	isMarked = true;
	markSize = 0;
}

/*	Example2::reset
 *
 *		Reset: reset the buffer positions
 */

void Example2::reset(void)
{
	if (!isMarked) return;	// not marked, nothing to do.

	/*
	 *	Reset to the marked position. We maintain two mutable data objects,
	 *	one which stores the data we're reading from (if any), and one which
	 *	stores the marked data.
	 *
	 *	Reset does two things: it prepends the read buffer with the data we
	 *	stored during the mark phase, and we clear the mark flag.
	 */

	if (markSize + readSize - readPos > readAlloc) {
		/*
		 *	Resize to fit
		 */

		int32_t toFit = markSize + readSize - readPos;
		toFit = (toFit + 255) & ~255;	// align to 256 byte boundary
		if (toFit < 0) toFit = 256;

		unsigned char *ptr = (unsigned char *)realloc(readBuffer, toFit);
		if (ptr == NULL) {
			throw std::bad_alloc();
		}

		readBuffer = ptr;
		readAlloc = toFit;
	}

	/*
	 *	Move the contents of the read buffer to a location above where the
	 *	mark queue will go
	 */

	if (readPos < readSize) {
		memmove(markSize + readBuffer, readPos + readBuffer, readSize - readPos);
	}

	/*
	 *	Move the marked data
	 */

	if (markSize > 0) {
		memmove(readBuffer, markBuffer, markSize);
	}

	/*
	 *	Update the read buffer settings
	 */

	readSize = markSize + readSize - readPos;
	readPos = 0;

	/*
	 *	Clear the mark
	 */

	isMarked = false;
	markSize = 0;

	/*
	 *	Update the file position
	 */

	curLine = markLine;
	curColumn = markColumn;
	textSize = textMarkSize;
}

/*
 *	Read input stream.
 */

int Example2::input(void)
{
	int ch;

	/*
	 *	Low level: read, if from marked buffer
	 */

	if (readPos < readSize) {
		ch = readBuffer[readPos++];
	} else {
		ch = file->readByte();

		if (ch == -1) return -1;		// At EOF; immediate return.
	}

	/*
	 *	Update file position
	 */

	if (ch == '\n') {
		curColumn = 0;
		++curLine;
	} else {
		++curColumn;
	}

	/*
	 *	Handle mark
	 */

	if (isMarked) {
		if (markSize >= markAlloc) {
			int32_t toFit = (markAlloc + 256) & ~255;
			if (toFit < 256) toFit = 256;
			unsigned char *ptr = (unsigned char *)realloc(markBuffer, toFit);
			if (ptr == NULL) {
				throw std::bad_alloc();
			}

			markBuffer = ptr;
			markAlloc = toFit;
		}
		markBuffer[markSize++] = (unsigned char)ch;
	}

	/*
	 *	Return read character.
	 */

	return ch;
}

/*
 *	Read ahead to determine if we're at the EOL
 */

bool Example2::atEOL()
{
	int ch;

	if (readPos < readSize) {
		ch = readBuffer[readPos];
	} else {
		ch = file->peekByte();
	}

	if ((ch == -1) || (ch == '\n')) return true;
	return false;
}

bool Example2::atSOL()
{
	return column == 0;
}

/*
 *	Read the state for the class/state combination. Decodes the sparce
 *	matrix that is compressed in StateMachineIA/JA/A above. If the
 *	entry is not found, returns MAXSTATES. This is the same as the lookup
 *	StateMachine[class][state] if the StateMachine sparse array was
 *	unrolled
 */

uint16_t Example2::stateForClass(uint16_t charClass, uint16_t state)
{
	size_t min,max,mid;

	/* Find range */
	min = StateMachineIA[state];
	max = StateMachineIA[state+1];

	/* Binary search for value in ja */
	while (min < max) {
		mid = (min + max)/2;
		uint16_t j = StateMachineJA[mid];
		if (charClass == j) {
			return StateMachineA[mid];
		} else if (charClass < j) {
			max = mid;
		} else {
			min = mid+1;
		}
	}
	return MAXSTATES;
}

void Example2::setFile(std::string &file, int32_t line)
{
	filename = file;
	curLine = line;
}

void Example2::setLine(int32_t line)
{
	curLine = line;
}

/*
 *	Internal methods declared within the Lex file
 */


CODE


/*
 *  For conditional states this takes an end DFA state and
 *  determines the proper end rule given the current start
 *  conditionals.
 */

uint16_t Example2::conditionalAction(uint16_t state)
{
    switch (state) {
        default:
            return MAXACTIONS;
    }
}

/*	Example2::lex
 *
 *		Lex interpreter. THis runs the state machine until we find something
 */

int32_t Example2::lex(void)
{
	uint16_t state;
	uint16_t action = MAXACTIONS;

#ifdef Example2_ValueDefined
	memset(&value,0,sizeof(value));
#endif

	/*
	 *	Run until we hit EOF or a production rule triggers a return
	 */

	for (;;) {
		/*
		 *	Start running the DFA
		 */

		state = 0;
		textSize = 0;

		line = curLine;
		column = curColumn;

		for (;;) {
			int ch = input();
			if (ch == -1) {
				/*
				 *	We've hit EOF. If there is no stored text, we assume
				 *	we're at the file EOF, so return EOF. Otherwise treat as
				 *	illegal state transition.
				 */

				if (textSize == 0) return -1;
				break;
			}

			/*
			 *	Attempt to transition to the next state
			 */

			uint16_t charClass = CharClass[ch];
			uint16_t newState = stateForClass(charClass, state);
			if (newState >= MAXSTATES) {
				/* Illegal state transition */
				break;
			}

			/*
			 *	Store the character we read, and transition to the next
			 *	state
			 */

			state = newState;

			if (textSize >= textAlloc) {
				int32_t toFit = (textAlloc + 256) & ~255;
				if (toFit < 256) toFit = 256;
				unsigned char *ptr = (unsigned char *)realloc(textBuffer, toFit);
				if (ptr == NULL) {
					throw std::bad_alloc();
				}

				textBuffer = ptr;
				textAlloc = toFit;
			}
			textBuffer[textSize++] = (char)ch;

			/*
			 *	Note the current action if we have one
			 */

			uint16_t newAction = StateActions[state];
			if (newAction > MAXACTIONS) {
				newAction = conditionalAction(newAction);
			}
			if (newAction != MAXACTIONS) {
				action = newAction;
				mark();
			}
		}

		/*
		 *	If no action is set, we simply abort after setting an internal
		 *	error state. This should never happen in a well designed lexer
		 */

		if (action == MAXACTIONS) {
			abort = "Illegal character sequence";
			return -1;
		}

		/*
		 *	Action is set, so we rewind.
		 */

		reset();
		if (textSize == 0) {
			abort = "No characters read in sequence";
			return -1;
		}

		/*
		 *	Convert text sequence into string
		 */

		text = std::string((char *)textBuffer,textSize);

		/*
		 *	Execute action
		 */

		switch (action) {
            case 0:
                {
                return 1; 
                }
                break;

			default:
				break;
		}
	}
}

