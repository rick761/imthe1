/*
 * ds18b20_library.h
 *
 *  Created on: 15 Oct 2018
 *      Author: Rick
 */

uint8_t therm_Reset();
void therm_WriteBit(uint8_t bit);
uint8_t therm_ReadBit();
void therm_WriteByte(uint8_t data);
uint8_t therm_ReadByte();
void therm_MatchRom(uint8_t rom[]);
void leesRauweTemperatuurData(uint8_t id[], uint8_t *byte0, uint8_t *byte1);
uint8_t RomReaderProgram();
uint8_t geefCelcius();
