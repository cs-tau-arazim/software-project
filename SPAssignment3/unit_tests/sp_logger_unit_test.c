#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"

// The following macro receives a printing function and checks that it returns "SP_LOGGER_INVAlID_ARGUMENT"
// whenever it should according to the documentation-
// "If any of msg or file or function are null or line is negative".
#define checkArguments(func) { \
	ASSERT_TRUE(\
			spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT); \
	ASSERT_TRUE( \
			spLoggerPrintError("a",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT); \
	ASSERT_TRUE( \
			spLoggerPrintError("a","b",NULL,5) == SP_LOGGER_INVAlID_ARGUMENT); \
	ASSERT_TRUE( \
			spLoggerPrintError("a", "b", "c", -1) == SP_LOGGER_INVAlID_ARGUMENT); }

// This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

// Check if all functions behave correctly when Logger is not defined
static bool loggerDefinedTest() {
	ASSERT_TRUE(
			spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(
			spLoggerPrintWarning("B","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintInfo("C") == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(
			spLoggerPrintDebug("D","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();

	// All functions know when to return "SP_LOGGER_UNDIFINED"
	return true;
}

static bool loggerInvalidArgumentTest() {
	// set logger
	ASSERT_TRUE(
			spLoggerCreate(NULL, SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);

	// PrintError:
	checkArguments(spLoggerPrintError);
	// PrintWarning:
	checkArguments(spLoggerPrintWarning);
	// PrintDebug:
	checkArguments(spLoggerPrintDebug);
	// PrintMsg:
	ASSERT_TRUE(spLoggerPrintMsg(NULL) == SP_LOGGER_INVAlID_ARGUMENT);

	spLoggerDestroy();
	return true;
}

// Only errors should be printed
static bool loggerErrorTest() {
	const char* expectedFile = "loggerErrorTestExp.log";
	const char* testFile = "loggerErrorTest.log";

	ASSERT_TRUE(
			spLoggerCreate(testFile, SP_LOGGER_ERROR_LEVEL)
					== SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile, expectedFile));
	return true;
}

// should print only warnings and errors
static bool loggerWarningTest() {
	const char* expectedFile = "loggerWarningTestExp.log";
	const char* testFile = "loggerWarningTest.log";

	ASSERT_TRUE(
			spLoggerCreate(testFile, SP_LOGGER_WARNING_ERROR_LEVEL)
					== SP_LOGGER_SUCCESS); // set logger

// should be printed
	ASSERT_TRUE(
			spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

// should be printed
	ASSERT_TRUE(
			spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

// should not be printed
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);

// should not be printed
	ASSERT_TRUE(
			spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile, expectedFile));
	return true;
}

// All messages except debug should be printed in info level
static bool loggerInfoTest() {
	const char* expectedFile = "loggerInfoTestExp.log";
	const char* testFile = "loggerInfoTest.log";

	ASSERT_TRUE(
			spLoggerCreate(testFile, SP_LOGGER_INFO_WARNING_ERROR_LEVEL)
					== SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);

// This one should not be printed
	ASSERT_TRUE(
			spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile, expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool loggerDebugTest() {
	const char* expectedFile = "loggerDebugTestExp.log";
	const char* testFile = "loggerDebugTest.log";

	ASSERT_TRUE(
			spLoggerCreate(testFile, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
					== SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(
			spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile, expectedFile));
	return true;
}

static bool loggerDestroyedTest() {
	// set logger
	ASSERT_TRUE(
			spLoggerCreate(NULL, SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);

	// delete the logger
	spLoggerDestroy();
	// make sure logger isn't defined
	ASSERT_TRUE(
			spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	return true;
}

int main() {
	RUN_TEST(loggerDefinedTest);
	RUN_TEST(loggerInvalidArgumentTest);
	RUN_TEST(loggerErrorTest);
	RUN_TEST(loggerWarningTest);
	RUN_TEST(loggerInfoTest);
	RUN_TEST(loggerDebugTest);
	RUN_TEST(loggerDestroyedTest);
	return 0;
}

