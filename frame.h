#ifndef FRAME_H
#define FRAME_H

#define _bool int

struct reportNode {
	struct reportNode* next;
	char* message;
	char* filename;//not in use yet
	int linenumber;//not in use yet

};
//reportNode

struct handle {
	int total;
	int fails;
	int messages;
	struct reportNode* root;

	char* header;
	char* report;

};
//handle;

struct reportNode* getNewNode(char* message);

struct handle* getEmptyHandle();

char* strcopyto(char* dest, char* src, int begin, int length);

char* strncombine(char* a, char* b, int lengt_a, int length_b);

char* appendNumbers(char* dest, int a, int b, int begin);

void generateHeader(struct handle* handle_);

void generateReport(struct handle* handle_);

char** getMessages(struct handle* handle_);

void  appendNote(char* note, struct handle* handle_);

void  appendNoteAndInt(char* note, int i, struct handle* handle_);

void  appendNoteAndMessage(char* note, char* message, struct handle* handle_);

_bool  assert_IntEqual(int actual, int expected, char *message,
		struct handle* handle_);

_bool  assert_IntNotEqual(int actual, int expected, char *message,
		struct handle* handle_);

_bool  assert_IntGreater(int actual, int expected, char *message,
		struct handle* handle_);

_bool  assert_IntGreaterOrEqual(int actual, int expected, char *message,
		struct handle* handle_);

_bool assert_IntSmaller(int actual, int expected, char *message,
		struct handle* handle_);

_bool assert_IntSmallerOrEqual(int actual, int expected, char *message,
		struct handle* handle_);

_bool assert_IntArrayEqual(int* actual, int* expected, int length, char *message,
		struct handle* handle_);

_bool  assert_IntInRange(int actual, int lowerBound, int higherBound, char *message, struct handle* handle_);

_bool  assert_IntNotInRange(int actual, int lowerBound, int higherBound, char *message, struct handle* handle_);

_bool  assert_FloatEqual(float actual, float expected, float delte, char *message,
		struct handle* handle_);

_bool  assert_FloatNotEqual(float actual, float expected, float delta,
		char *message, struct handle* handle_);

//add needed functions above this comment 

_bool noteMe(int _bool_, char *message, struct handle *handle_);

void deleteHandle(struct handle* handle_);

#endif
