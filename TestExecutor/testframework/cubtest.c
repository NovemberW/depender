/**
 ******************************************************************************
 * @file           : cubtest.c
 * @brief          : Contains functions to write simple Unit-Tests.
 ******************************************************************************
 */

#ifndef CUBTEST_H_
#define CUBTEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boarddefBridge.h"
#include "cubtest.h"

/*
 *
 * Depending on the system the itoa (int to string) function is not provided (not officially in the c standard)
 * you may need this. Of course stolen from stackoverflow.
 *
 */
int itoaB(int value, char *sp, int radix) {
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

/**
 * @brief Appends the given message to the internal list of given handle
 * @param handle_ the handle to be used
 * @param message the message to be added
 * @return that pointer
 */
void appendToList(handle* handle_, char* message) {
	handle_->messages++;
	handle_->pF(message);
	handle_->pF("\n");
}

/**
 * @brief pointer to new handle (allocated) with values set to 0
 * @param none
 * @return that pointer
 */
handle* getEmptyHandle(printFunc pF) {
	handle *handle_ = (handle*) _malloc(sizeof(handle));
	handle_->fails = 0;
	handle_->total = 0;
	handle_->messages = 0;
	handle_->pF = pF;
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
	message[length_a + length_b + 1] = 0;
	return message;
}

/**
 * @brief Generates a string comprised of given integers and message in the format
 * Actual: a Expected: b message
 * @param buffer - the result will be stored here
 * @param a the first integer
 * @param b the second integer
 * @param message  the message to append to the end
 */
void generateActualExpectedString(char* buffer, int a, int b, char* message) {
	char* actual = "Actual: ";
	char* expected = "Expected: ";

	char intBuffer[12];

	int pos = 0;

	strcopyto(buffer, actual, 0, strlen(actual));

	pos += strlen(actual);

	strcopyto(buffer, " ", pos, strlen(" "));
	pos += strlen(" ");

	itoaB(a, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, strlen(intBuffer));
	pos += strlen(intBuffer);

	strcopyto(buffer, " ", pos, strlen(" "));
	pos += strlen(" ");

	strcopyto(buffer, expected, pos, strlen(expected));
	pos += strlen(expected);

	strcopyto(buffer, " ", pos, strlen(" "));
	pos += strlen(" ");

	itoaB(b, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, strlen(intBuffer));
	pos += strlen(intBuffer);

	strcopyto(buffer, " ", pos, strlen(" "));
	pos += strlen(" ");

	strcopyto(buffer, message, pos, strlen(message));
	pos += strlen(message);

	buffer[pos] = 0;

}

/**
 * @brief Generates a string comprised of given integers and message in the format
 * Actual: a Expected: b message
 * @param buffer - the result will be stored here
 * @param a the first integer
 * @param b the second integer
 * @param message  the message to append to the end
 */
void generateNoteAndIntString(char* buffer, char* message, int a) {
	char intBuffer[12];

	int pos = 0;

	strcopyto(buffer, message, 0, strlen(message));

	pos += strlen(message);

	strcopyto(buffer, ": ", pos, strlen(": "));
	pos += strlen(": ");

	int help = itoaB(a, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, help);
	pos += help;

	buffer[pos - 1] = 0;

}

void generateNoteString(char* buffer, char* message) {
	strcopyto(buffer, message, 0, strlen(message));
	buffer[strlen(message)] = 0;

}

void generateNoteAndMessageString(char* buffer,char* note, char* message) {
	strcopyto(buffer, note, 0, strlen(note));
	strcopyto(buffer, ": ", strlen(note), strlen(": "));
	strcopyto(buffer, message, strlen(note) + 2, strlen(message));
	buffer[strlen(note) + strlen(message) + 2] = 0;
}

void generateIntRangeString(char* buffer, int actual, int lowerBound,
		int upperBound, char* message) {
	char* start = "Range ";
	char* actualS = "Actual: ";

	char intBuffer[12];
	int help = 0;

	int pos = 0;

	strcopyto(buffer, start, 0, strlen(start));

	pos += strlen(start);

	strcopyto(buffer, " [", pos, strlen(" ["));
	pos += strlen(" [");

	help = itoaB(lowerBound, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, help);
	pos += help;

	strcopyto(buffer, ",", pos, strlen(","));
	pos += strlen(",");

	help = itoaB(upperBound, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, help);
	pos += help;

	strcopyto(buffer, "]", pos, strlen("]"));
	pos += strlen("]");

	strcopyto(buffer, " ", pos, strlen(" "));
	pos += strlen(" ");

	strcopyto(buffer, actualS, pos, strlen(actualS));
	pos += strlen(actualS);

	help = itoaB(actual, intBuffer, 10);
	strcopyto(buffer, intBuffer, pos, help);
	pos += help;

	buffer[pos] = 0;
}

void generateIntArrayEqualsString(char* buffer, int* a, int* b, int length,
		int index, char* message) {
	char* notEqual = "Arrays not equal at: ";
	char* values = "Values: ";

	char intBuffer[12];

	if (index != -1) {
		int pos = 0;

		strcopyto(buffer, notEqual, 0, strlen(notEqual));
		pos += strlen(notEqual);

		strcopyto(buffer, " ", pos, strlen(" "));
		pos += strlen(" ");

		int help = itoaB(index, intBuffer, 10);
		strcopyto(buffer, intBuffer, pos, help);
		pos += help;

		strcopyto(buffer, "=> ", pos, strlen("=> "));
		pos += strlen("=> ");

		help = itoaB(a[index], intBuffer, 10);
		strcopyto(buffer, intBuffer, pos, help);
		pos += help;

		strcopyto(buffer, "=|=", pos, strlen("=|="));
		pos += strlen("=|=");

		help = itoaB(b[index], intBuffer, 10);
		strcopyto(buffer, intBuffer, pos, help);
		pos += help;

		strcopyto(buffer, " :", pos, strlen(" :"));
		pos += strlen(" :");

		strcopyto(buffer, message, pos, strlen(message));
		pos += strlen(message);

		buffer[pos - 1] = 0;

	} else
		buffer[0] = 0;
}
/**
 * @brief Appends a note to the list of errormessages in given handle. The message is not counted as failed
 * of correct as it is not a test.
 * @param handle_ the handle where it should be added
 * @param note the note to be added
 * @return nothing
 */
void appendNote(char* note, handle* handle_) {
	char buffer[strlen(note) + 1];
	generateNoteString(buffer, note);
	appendToList(handle_, buffer);
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
void appendNoteAndInt(char* note, int i, handle* handle_) {
	char buffer[strlen(note) + 15 + 1];
	generateNoteAndIntString(buffer, note, i);
	appendToList(handle_, buffer);
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
void appendNoteAndMessage(char* note, char* message, handle* handle_) {
	char buffer[strlen(note) + strlen(message) + 1];
	generateNoteAndMessageString(buffer,note, message);
	appendToList(handle_, buffer);
}

/**
 * @brief Creates a simple header for the testreport. That header includes date and time ob the test.
 * The header is added to the given handle.
 * @param handle_ the handle where it should be added
 * @return nothing
 */
void generateHeader(handle* handle_) {
	char *startText = "Testlauf:";

	handle_->pF(startText);
	handle_->pF(" ");
	handle_->pF(__DATE__);
	handle_->pF(" ");
	handle_->pF(__TIME__);
	handle_->pF("\n");
}

/**
 * @brief Creates a testreport. Containing # of tests that have been passed, # of tests that have not been passed. total # of tests
 * @param handle_ the handle to which it will be added
 * @return nothing
 */
void generateReport(handle *handle_) {

	char* success = _malloc(10); //maybe rething and use log
	char* fails = _malloc(10);
	char* total = _malloc(10);

	itoaB(handle_->total, total, 10);
	itoaB(handle_->total - handle_->fails, success, 10);
	itoaB(handle_->fails, fails, 10);

	char* blank = " ";
	char *report = "Report:";
	char* successMessage = "Bestanden:";
	char* failMessage = "Fehlgeschlagen:";
	char* totalMessage = "Total:";
	handle_->pF(report);
	handle_->pF(blank);
	handle_->pF(successMessage);
	handle_->pF(blank);
	handle_->pF(success);
	handle_->pF(blank);
	handle_->pF(failMessage);
	handle_->pF(blank);
	handle_->pF(fails);
	handle_->pF(blank);
	handle_->pF(totalMessage);
	handle_->pF(blank);
	handle_->pF(total);
	handle_->pF("\n");
}

/**
 * @brief asserts that given integer actual is equal to given integer expected, result is logged to given handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual != expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntEqual(int actual, int expected, char *message, handle *handle_) {
//generateActualExpectedString(message, actual, expected, strlen(message))
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual == expected, buffer, handle_);
}
/**
 * @brief asserts that given integer actual is not equal to given integer expected, if that is not the case the given message is logged to given handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntNotEqual(int actual, int expected, char *message,
		handle* handle_) {
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual != expected, buffer, handle_);
}

/**
 * @brief asserts that given integer actual is greater than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntGreater(int actual, int expected, char *message,
		handle* handle_) {
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual > expected, buffer, handle_);
}

/**
 * @brief asserts that given integer actual is greater ot equal than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntGreaterOrEqual(int actual, int expected, char *message,
		handle* handle_) {
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual >= expected, buffer, handle_);
}

/**
 * @brief asserts that given integer actual is smaller than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntSmaller(int actual, int expected, char *message,
		handle* handle_) {
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual < expected, buffer, handle_);
}

/**
 * @brief asserts that given integer actual is smaller or equal than given integer expected, if that is not the case the given message is added to the handle
 * @param actual the value to analyst, expected the value to compare to, message that will be logged if actual == expected
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntSmallerOrEqual(int actual, int expected, char *message,
		handle* handle_) {
	char buffer[100];
	generateActualExpectedString(buffer, actual, expected, message);
	return noteMe(actual <= expected, buffer, handle_);
}

/**
 * @brief asserts that given integer actual is in between given bounds (inclusive).
 * The expected entry in the message will be set to -1.
 * @param actual the value to analyse,lowerBound the lower bound, upperBound the upper bound.
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntInRange(int actual, int lowerBound, int higherBound,
		char *message, handle* handle_) {
	char buffer[100];

	generateIntRangeString(buffer, actual, lowerBound, higherBound, message);
	return noteMe(lowerBound <= actual && actual <= higherBound, buffer,
			handle_);
}

/**
 * @brief asserts that given integer actual is not in between given bounds (inclusive).
 * The expected entry in the message will be set to -1.
 * @param actual the value to analyse,lowerBound the lower bound, upperBound the upper bound.
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntNotInRange(int actual, int lowerBound, int higherBound,
		char *message, handle* handle_) {
	char buffer[100];

	generateIntRangeString(buffer, actual, lowerBound, higherBound, message);
	return noteMe(lowerBound > actual || actual > higherBound, buffer, handle_);
}

/**
 * @brief asserts that the given integer[] actual & expected are equal in the range [0,length-], if that is not the case the given message is added to the handle
 * If a mismatch is found the values that mismatch are reported, followed by : and the index
 * @param actual the array to compare against.
 * @param expected the given array to compare with,
 * @param message that will be logged if actual != expected on [0,length-1]
 * handle_ the ure to generate the result of all tests
 * @return None
 */
_bool assert_IntArrayEqual(int* actual, int* expected, int length,
		char *message, handle* handle_) {
	int i = 0;
	int index = -1;
	int statusAkku = 1;

	for (; i < length && statusAkku; i++) {
		statusAkku &= (actual[i] == expected[i]);
	}

	if (i != length) {
		index = i;
	}

	char buffer[100];

	generateIntArrayEqualsString(buffer, actual, expected, length, index,
			message);

	return noteMe(statusAkku, buffer, handle_);

}

_bool assert_FloatEqual(float actual, float expected, float delta,
		char *message, handle* handle_) {
	return noteMe(
			(actual >= (expected - delta)) && (actual <= (expected + delta)),
			message, handle_);
}

_bool assert_FloatNotEqual(float actual, float expected, float delta,
		char *message, handle* handle_) {
	return noteMe(
			(actual < (expected - delta)) || (actual > (expected + delta)),
			message, handle_);
}

//add functions that are needed above this comment

/**
 * @brief Used for internal operations. Main purpose is to log result actuald on results of evaluation (_bool_) to given handle
 * @param _bool_ the result of the evaluation (comparison), message the message that will be logged if _bool_ if false
 * handle_ the handle to which results will be logged to
 * @return None
 */
_bool noteMe(int _bool_, char * message, handle *handle_) {
	handle_->total++;
	if (!_bool_) {
		handle_->fails++;
		handle_->pF(message);
		handle_->pF("\n");
	}
	return !_bool_;
}
#endif
