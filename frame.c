#include "frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define onBoard 1 // uncomment when used on board

/**
 ******************************************************************************
 * @file           : frame.c
 * @brief          : Contains functions to write simple Unit-Tests.
 ******************************************************************************
 */

#ifdef onBoard
#include <FreeRTOS.h>
#define _malloc pvPortMalloc
#define _free pvPortFree
#else
#define _malloc malloc
#define _free free

/*
 *
 * Depending on the system the itoa (int to string) function is not provided (not officially in the c standard)
 * you may need this. Of course stolen from stackoverflow.
 *
 */
// Yet, another good itoa implementation
// returns: the length of the number string
int itoa(int value, char *sp, int radix) {
	char tmp[16]; // be careful with the length of the buffer
	char *tp = tmp;
	int i;
	unsigned v;

	int sign = (radix == 10 && value < 0);
	if (sign)
		v = -value;
	else
		v = (unsigned) value;

	while (v || tp == tmp) {
		i = v % radix;
		v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	int len = tp - tmp;

	if (sign) {
		*sp++ = '-';
		len++;
	}

	while (tp > tmp)
		*sp++ = *--tp;

	return len;
}

#endif

/**
 * @brief creates a new reportNode to to be enqued
 * @param none
 * @return that pointer
 */
struct reportNode* getNewNode(char* message) {
	struct reportNode* node = (struct reportNode*) _malloc(
			sizeof(struct reportNode));
	node->next = 0;
	node->message = message;
	node->filename = "";//not in use yet
	node->linenumber = -1;// not in use yet
	return node;
}

/**
 * @brief Appends the given message to the internal list of given handle
 * @param handle_ the handle to be used
 * @param message the message to be added
 * @return that pointer
 */
void appendToList(struct handle* handle_, char* message) {
	handle_->messages++;
	if (handle_->root == 0) {//root case
		handle_->root = getNewNode(message);
	} else {
		struct reportNode* pivot = handle_->root;
		while (pivot->next != 0) {//iterating to the last element
			pivot = pivot->next;
		}
		pivot->next = getNewNode(message); //appending to that ende
	}
}

/**
 * @brief pointer to new handle (allocated) with values set to 0
 * @param none
 * @return that pointer
 */
struct handle* getEmptyHandle() {
	struct handle *handle_ = (struct handle*) _malloc(sizeof(struct handle));
	handle_->fails = 0;
	handle_->total = 0;
	handle_->messages = 0;
	handle_->root = 0;

	return handle_;
}

/**
 * @brief an extended string copy funktion to complement strcpy (from string.h).
 * @param dest, the actual to append to. src, the part that will be appended. begin the index at wich src will be appended to
 * src. length the length of the section of src which will be appended.
 * @return a pointer to dest
 */
char* strcopyto(char* dest, char* src, int begin, int length) {
	int i = begin;

	while (i < begin + length) {
		dest[i] = src[i - begin];
		i++;
	}

	return dest;
}

char* strncombine(char* a, char* b, int length_a, int length_b) {
	size_t message_len = length_a + length_b + 2 + 1 + 10; /*+ 2 for ": ", + 1 for terminating NULL */
	char *message = (char*) malloc(message_len);

	strcopyto(message, a, 0, length_a);
	strcopyto(message, ": ", length_a, 2);
	strcopyto(message, b, length_a + 2, length_b);
	return message;
}

/**
 * @brief appends a string representation of the two given integers to the string and return a new string
 * containing all three.
 * @param src
 * @param a the first integer
 * @param b the second integer
 * @param begin
 * @return that pointer
 */
char* appendNumbers(char* src, int a, int b, int begin) {
	char* actual = "Actual: ";
	char* expected = "Expected: ";
	char* akku = _malloc(
			strlen(src) + strlen(actual) + strlen(expected) + 12 * 2);

	char* buffer = _malloc(15);

	strcpy(akku, src); // =^= strcopyto(F,A,0,sizeof(A));

	int pos = strlen(src);

	strcopyto(akku, " ", pos, strlen(" "));
	pos += strlen(" ");
	strcopyto(akku, actual, pos, strlen(actual));
	pos += strlen(actual);
	strcopyto(akku, " ", pos, strlen(" "));
	pos += strlen(" ");
	itoa(b, buffer, 10);
	strcopyto(akku, buffer, pos, strlen(buffer));
	pos += strlen(buffer);
	strcopyto(akku, " ", pos, strlen(" "));
	pos += strlen(" ");
	strcopyto(akku, expected, pos, strlen(expected));
	pos += strlen(expected);
	strcopyto(akku, " ", pos, strlen(" "));
	pos += strlen(" ");
	itoa(a, buffer, 10);
	strcopyto(akku, buffer, pos, strlen(buffer));


	return akku;
}

/**
 * @brief Appends a note to the list of errormessages in given handle. The message is not counted as failed
 * of correct as it is not a test.
 * @param handle_ the handle where it should be added
 * @param note the note to be added
 * @return nothing
 */
void appendNote(char* note, struct handle* handle_) {
	appendToList(handle_, strncombine(note, "", strlen(note), 0));
}

/**
 * @brief Appends a note to the list of errormessages in given handle. The message is not counted as failed
 * of correct as it is not a test. The note consists of the note and the given int.They are seperated
 * by <: >.
 * @param handle_ the handle where it should be added
 * @param note the note to be added
 * @param i the int to be added
 * @return nothing
 */
void appendNoteAndInt(char* note, int i, struct handle* handle_) {
	char* converted = (char*) _malloc(12);
	itoa(i, converted, 10);
	appendToList(handle_,
			strncombine(note, converted, strlen(note), strlen(converted)));
}

/**
 * @brief Appends a note to the list of errormessages in given handle. The message is not counted as failed
 * of correct as it is not a test. The note consists of the note and the message.They are seperated
 * by <: >.
 * @param handle_ the handle where it should be added
 * @param note the note to be added
 * @param message the message to be added
 * @return nothing
 */
void appendNoteAndMessage(char* note, char* message, struct handle* handle_) {
	appendToList(handle_,
			strncombine(note, message, strlen(note), strlen(message)));
}

/**
 * @brief Creates a simple header for the testreport. That header includes date and time ob the test.
 * The header is added to the given handle.
 * @param handle_ the handle where it should be added
 * @return nothing
 */
void generateHeader(struct handle* handle_) {
	char A[] = "Testlauf:";
	char *akkumulator = _malloc(sizeof(char) * 35);

	int start_length = (int) strlen(A);

	strcpy(akkumulator, A); // =^= strcopyto(F,A,0,sizeof(A));

	//__TIME__ & __DATE__ are evanluated by the compiler and therefor the time and date
	//when the code is compiled

	strcopyto(akkumulator, " ", start_length, sizeof(" "));
	strcopyto(akkumulator, __DATE__, start_length + 1, sizeof(__DATE__));
	strcopyto(akkumulator, " ", start_length + sizeof(__DATE__), sizeof(" "));
	strcopyto(akkumulator, __TIME__, start_length + 1 + sizeof(__DATE__),
			sizeof(__TIME__));
	handle_->header = akkumulator;
}

/**
 * @brief Creates a testreport. Containing # of tests that have been passed, # of tests that have not been passed. total # of tests
 * @param handle_ the handle to which it will be added
 * @return nothing
 */
void generateReport(struct handle *handle_) {
	char *akkumulator = _malloc(sizeof(char) * 50); //should be changed to a dynamic value based on length( report, etc.)

	char* success = _malloc(10);//maybe rething and use log
	char* fails = _malloc(10);
	char* total = _malloc(10);

	itoa(handle_->total, total, 10);
	itoa(handle_->total - handle_->fails, success, 10);
	itoa(handle_->fails, fails, 10);

	int writeAkku = 0;

	char* blank = " \n";
	char A[] = "Report:";
	char* successMessage = "Bestanden: ";
	char* failMessage = "Fehlgeschlagen: ";
	char* totalMessage = "Total: ";

	int start_length = (int) strlen(A);

	strcpy(akkumulator, A); // =^= strcopyto(F,A,0,sizeof(A));
	writeAkku += start_length;
	strcopyto(akkumulator, blank, writeAkku, strlen(blank));
	writeAkku += strlen(blank);
	strcopyto(akkumulator, successMessage, writeAkku, strlen(successMessage));
	writeAkku += strlen(successMessage);
	strcopyto(akkumulator, success, writeAkku, strlen(success));
	writeAkku += strlen(success);
	strcopyto(akkumulator, blank, writeAkku, strlen(blank));
	writeAkku += strlen(blank);
	strcopyto(akkumulator, failMessage, writeAkku, strlen(failMessage));
	writeAkku += strlen(failMessage);
	strcopyto(akkumulator, fails, writeAkku, strlen(fails));
	writeAkku += strlen(fails);
	strcopyto(akkumulator, blank, writeAkku, strlen(blank));
	writeAkku += strlen(blank);
	strcopyto(akkumulator, totalMessage, writeAkku, strlen(totalMessage));
	writeAkku += strlen(totalMessage);
	strcopyto(akkumulator, total, writeAkku, strlen(total));
	handle_->report = akkumulator;
}

/**
 * @brief returns an chararray containing the messages that have been reportet as failed
 * @param the handle where to extract
 * @return that list
 */
char** getMessages(struct handle* handle_) {
	struct reportNode* pi = handle_->root;

	char** noteList = (char**) _malloc(sizeof(char*) * (handle_->messages));
	int i = 0;
	while (pi != 0) {
		noteList[i] = pi->message;
		pi = pi->next;
		i++;
	}

	return noteList;
}

/**
 * @brief asserts that given integer actual is equal to given integer expected, result is logged to given handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual != expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntEqual(int actual, int expected, char *message,
		struct handle *handle_) {

return	noteMe(actual == expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}
/**
 * @brief asserts that given integer actual is not equal to given integer expected, if that is not the case the given message is logged to given handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntNotEqual(int actual, int expected, char *message,
		struct handle* handle_) {
	return noteMe(actual != expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is greater than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntGreater(int actual, int expected, char *message,
		struct handle* handle_) {
	return noteMe(actual > expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is greater ot equal than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntGreaterOrEqual(int actual, int expected, char *message,
		struct handle* handle_) {
	return noteMe(actual >= expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is smaller than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntSmaller(int actual, int expected, char *message,
		struct handle* handle_) {
	return noteMe(actual < expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is smaller or equal than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntSmallerOrEqual(int actual, int expected, char *message,
		struct handle* handle_) {
	noteMe(actual <= expected,
			appendNumbers(message, actual, expected, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is in between given bounds (inclusive).
 * The expected entry in the message will be set to -1.
 * @param actual the value to analyse,lowerBound the lower bound, upperBound the upper bound.
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntInRange(int actual, int lowerBound, int higherBound, char *message, struct handle* handle_) {
	noteMe(lowerBound <= actual && actual <= higherBound, appendNumbers(message, -1, actual, strlen(message)), handle_);
}

/**
 * @brief asserts that given integer actual is not in between given bounds (inclusive).
 * The expected entry in the message will be set to -1.
 * @param actual the value to analyse,lowerBound the lower bound, upperBound the upper bound.
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool  assert_IntNotInRange(int actual, int lowerBound, int higherBound, char *message, struct handle* handle_) {
	noteMe(lowerBound > actual || actual > higherBound, appendNumbers(message, -1, actual, strlen(message)), handle_);
}

/**
 * @brief asserts that the given integer[] actual & expected are equal in the range [0,length-], if that is not the case the given message is added to the handle
 * If a mismatch is found the values that mismatch are reported, followed by : and the index
 * @param actual the array to compare against.
 * @param expected the given array to compare with,
 * @param message that will be logged if actual != expected on [0,length-1]
 * handle_ the structure to generate the result of all tests
 * @return None
 */
_bool assert_IntArrayEqual(int* actual, int* expected, int length, char *message,
		struct handle* handle_) {
	int i = 0;

	int statusAkku = 1;

	for (; i < length && statusAkku; i++) {
		statusAkku &= (actual[i] == expected[i]);
	}

	char* missmatch = appendNumbers(message, actual[i - 1], expected[i - 1],
			strlen(message));

	char* converted = (char*) _malloc(12);
	itoa(i - 1, converted, 10);

	return noteMe(statusAkku,
			strncombine(missmatch, converted, strlen(missmatch),
					strlen(converted)), handle_);

}

_bool assert_FloatEqual(float actual, float expected, float delta, char *message,
		struct handle* handle_) {
	return noteMe((actual >= (expected - delta)) && (actual <= (expected + delta)),
			message, handle_);
}

_bool  assert_FloatNotEqual(float actual, float expected, float delta,
		char *message, struct handle* handle_) {
	return noteMe((actual < (expected - delta)) || (actual > (expected + delta)),
			message, handle_);
}

//add functions that are needed above this comment

/**
 * @brief Used for internal operations. Main purpose is to log result actuald on results of evaluation (_bool_) to given handle
 * @param _bool_ the result of the evaluation (comparison), message the message that will be logged if _bool_ if false
 * handle_ the handle to which results will be logged to
 * @return None
 */
_bool noteMe(int _bool_, char * message, struct handle *handle_) {
	handle_->total++;

	if (!_bool_) {
		handle_->fails++;
		appendToList(handle_, message);
	}
	
	return !_bool_;
}

/**
 * @brief Frees all the memory used for the handle, its list and the associated strings 
 * @param handle_ the handle that will be deleted 
 * @return None
 */
void deleteHandle(struct handle* handle_) {
	struct reportNode* pivot = handle_->root;
	struct reportNode* help;

	while(pivot != 0){
		free(pivot->message);
		help = pivot;
		pivot = pivot->next;
		_free(help);
	}

	_free(handle_->header);
	_free(handle_->report);
	_free(handle_);
}

