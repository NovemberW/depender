#ifndef TESTFRAME_1_H_
#define TESTFRAME_1_H_

/*
 * testrahmen_1.c
 *
 *  Created on: 04.12.2018
 *      Author: nicholas
 */

#include "../testframework/cubtest.h"
#include "../support/debugPrints.h"
#include "../codeToTest/cubesatConstants.h"
#include "../codeToTest/dataHandler.h"
#include "../support/crc32MPEG2.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "testframe_1.h"

void t_F_CreateType(handle* handle_) {
	uint8_t max = 128;
	uint8_t i = 10;
	appendNote("Starting Functional for xCreateType", handle_);
	debugPrintln("F-Davor\n\r");

	//Functional-Begin
	for (; i < max; i++) {
		uint8_t response = xCreateType(dataPacketType,  i);

		assert_IntEqual(response >> 8, 1, "MSB Incorrect for data", handle_);
		assert_IntEqual(i, i, "Content Incorrect for data",
				handle_);
	}
	for (i = 0; i < max; i++) {
		uint8_t response = xCreateType(commandPacketType, i);
		if(
				assert_IntEqual(response >> 8, 0, "MSB Incorrect for command", handle_)
		||
				assert_IntEqual(response & 0x7F, i, "Content Incorrect for command",
				handle_)){
			appendNoteAndInt("i was",i, handle_);
			appendNoteAndInt("response was",response, handle_);
		}
	}
	//Functional-End
	appendNote("Done Functional for xCreateType", handle_);
}

void t_A_CreateType(handle* handle_) {
	appendNote("Starting Abnormal for xCreateType", handle_);
	int i;
	int max = 128;
	//Abnormal-start
	//The range of 129-255
	for (i = max; i > 0; i++) {
		//	uint8_t response = xCreateType(dataPacketType, i);
		//	assert_IntEqual(response >> 8, 0, "MSB Incorrect for Data", handle_);
		//	assert_IntEqual(response && 0x7F, i && 0x7F,
//				"Content Incorrect for Data", handle_);
	}
	debugPrintln("zwei");
	for (i = max; i > 0; i++) {
		int response = xCreateType(commandPacketType, i);
		assert_IntEqual(response >> 8, 1, "MSB Incorrect for command", handle_);
		assert_IntEqual(response && 0x7F, i && 0x7F,
				"Content Incorrect for command", handle_);
	}
	//Abnormal-end
	appendNote("Done Abnormal for xCreateType", handle_);
}

void t_F_CubesatConstants(handle* handle_) {
	appendNote("Testing Functional for  cubesatConstants.h", handle_);
	assert_IntNotEqual(commandPacketType, dataPacketType,
			"packetTypes are the same.", handle_);
	//needed as the values are set by hand
	assert_IntEqual(sizeof(dataPacket), 16,
			"Size of dataPacket does not match CRC-Requirement", handle_);
	//needed as we want to utilize the build in hardware crc-calculator which requires size to be
	//a multiple of 64
	appendNote("Done Functional for cubesatConstants.h", handle_);
}

void t_F_DataHandlerPack(handle* handle_) {
	appendNote("Testing xDatahandler functional", handle_);

	subsystemID sender = obcID;
	subsystemID receiver = powerID;
	subsystemID type = 0x8F;
	uint32_t data = 0xA0A2A3A4;

	uint32_t timeBefore = HAL_GetTick();

	dataPacket *first = xDataHandlerPack(sender, receiver, type, data);

	uint32_t timeAfter = HAL_GetTick();

	assert_IntEqual(first->senderID, sender, "Wrong sender", handle_);
	assert_IntEqual(first->receiverID, receiver, "Wrong receiver", handle_);
	assert_IntEqual(first->type_sID, type, "Wrong type_sID", handle_);
	assert_IntEqual(first->data, data, "Wrong data", handle_);

	uint32_t time = first->timestamp;
	appendNoteAndInt("Before", timeBefore, handle_);
	appendNoteAndInt("CreateTime", time, handle_);
	appendNoteAndInt("After", timeAfter, handle_);

	assert_IntInRange(time, timeAfter, timeBefore,
			"Zeitstempel nicht in logischem Rahmen", handle_);

	appendNote(
			"Ein Zeittest ist hier nicht wirklich sinnig, da die Abfolge noch innerhalb eines Clock Ticks abgeschlossen wird",
			handle_);

	uint8_t checkSumHelper[12] = { 0x00, first->senderID, first->receiverID,
			first->type_sID };
	*(((uint32_t*) checkSumHelper) + 1) = first->timestamp;
	*(((uint32_t*) checkSumHelper) + 2) = first->data;
	uint32_t buffer[3];

	buffer[0] = first->senderID;
	buffer[0] = buffer[0] << 8;
	buffer[0] = buffer[0] | first->receiverID;
	buffer[0] = buffer[0] << 8;
	buffer[0] = buffer[0] | first->type_sID;
	buffer[0] = buffer[0] << 8;

	buffer[1] = first->timestamp;
	buffer[2] = first->data;
	int length = 12;
	/*int i = 0;	//checkSumHelper;
	uint8_t* buffer2 = (uint8_t*) buffer;*/
	/*for (; i < length; i++) {
	 debugPrintlnByte(buffer2[i]);
	 }

	 debugPrintln("-------------------------");
	 */
	length = 3;
	/*i = 0;	//checkSumHelper;
	 for (; i < length; i++) {
	 debugPrintlnIntHex(buffer[i]);
	 }
	 */
	/*debugPrintln( "-------------------------");
	 debugPrintlnInt(first->senderID);
	 debugPrintlnInt(first->receiverID);
	 debugPrintlnInt(first->type_sID);
	 debugPrintlnIntHex(first->timestamp);
	 debugPrintlnIntHex(first->data);*/
	debugPrintln("Test\n");
	if (assert_IntEqual(first->chksum, crc32((uint8_t*) buffer, length),
			"Checksums do not match", handle_)) {

	}

	appendNote("Additional check of the crc-chekcsum is required.", handle_);

	appendNote("Done testing xDatahandler functional", handle_);

}
#endif
