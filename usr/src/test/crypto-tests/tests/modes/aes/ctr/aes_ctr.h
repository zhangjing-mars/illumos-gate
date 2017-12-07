/*
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 */

/*
 * Copyright 2015 Nexenta Systems, Inc.  All rights reserved.
 */

#ifndef _AES_CTR_H
#define	_AES_CTR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Test vectors
 * RFC3686 section 6
 */

uint8_t CTR_KEY0[] = {
	0xAE, 0x68, 0x52, 0xF8, 0x12, 0x10, 0x67, 0xCC,
	0x4B, 0xF7, 0xA5, 0x76, 0x55, 0x77, 0xF3, 0x9E,
};

uint8_t CTR_CB0[] = {
	0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

uint8_t CTR_DATA0[] = {
	0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
	0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67,
};

uint8_t CTR_RES0[] = {
	0xE4, 0x09, 0x5D, 0x4F, 0xB7, 0xA7, 0xB3, 0x79,
	0x2D, 0x61, 0x75, 0xA3, 0x26, 0x13, 0x11, 0xB8,
};

uint8_t CTR_KEY1[] = {
	0x7E, 0x24, 0x06, 0x78, 0x17, 0xFA, 0xE0, 0xD7,
	0x43, 0xD6, 0xCE, 0x1F, 0x32, 0x53, 0x91, 0x63,
};

uint8_t CTR_CB1[] = {
	0x00, 0x6C, 0xB6, 0xDB, 0xC0, 0x54, 0x3B, 0x59,
	0xDA, 0x48, 0xD9, 0x0B,
};

uint8_t CTR_DATA1[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};

uint8_t CTR_RES1[] = {
	0x51, 0x04, 0xA1, 0x06, 0x16, 0x8A, 0x72, 0xD9,
	0x79, 0x0D, 0x41, 0xEE, 0x8E, 0xDA, 0xD3, 0x88,
	0xEB, 0x2E, 0x1E, 0xFC, 0x46, 0xDA, 0x57, 0xC8,
	0xFC, 0xE6, 0x30, 0xDF, 0x91, 0x41, 0xBE, 0x28,
};

uint8_t CTR_KEY2[] = {
	0x76, 0x91, 0xBE, 0x03, 0x5E, 0x50, 0x20, 0xA8,
	0xAC, 0x6E, 0x61, 0x85, 0x29, 0xF9, 0xA0, 0xDC,
};

uint8_t CTR_CB2[] = {
	0x00, 0xE0, 0x01, 0x7B, 0x27, 0x77, 0x7F, 0x3F,
	0x4A, 0x17, 0x86, 0xF0,
};

uint8_t CTR_DATA2[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23,
};

uint8_t CTR_RES2[] = {
	0xC1, 0xCF, 0x48, 0xA8, 0x9F, 0x2F, 0xFD, 0xD9,
	0xCF, 0x46, 0x52, 0xE9, 0xEF, 0xDB, 0x72, 0xD7,
	0x45, 0x40, 0xA4, 0x2B, 0xDE, 0x6D, 0x78, 0x36,
	0xD5, 0x9A, 0x5C, 0xEA, 0xAE, 0xF3, 0x10, 0x53,
	0x25, 0xB2, 0x07, 0x2F,
};

uint8_t CTR_KEY3[] = {
	0x16, 0xAF, 0x5B, 0x14, 0x5F, 0xC9, 0xF5, 0x79,
	0xC1, 0x75, 0xF9, 0x3E, 0x3B, 0xFB, 0x0E, 0xED,
	0x86, 0x3D, 0x06, 0xCC, 0xFD, 0xB7, 0x85, 0x15,
};

uint8_t CTR_CB3[] = {
	0x00, 0x00, 0x00, 0x48, 0x36, 0x73, 0x3C, 0x14,
	0x7D, 0x6D, 0x93, 0xCB,
};

uint8_t CTR_DATA3[] = {
	0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
	0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67,
};

uint8_t CTR_RES3[] = {
	0x4B, 0x55, 0x38, 0x4F, 0xE2, 0x59, 0xC9, 0xC8,
	0x4E, 0x79, 0x35, 0xA0, 0x03, 0xCB, 0xE9, 0x28,
};

uint8_t CTR_KEY4[] = {
	0x7C, 0x5C, 0xB2, 0x40, 0x1B, 0x3D, 0xC3, 0x3C,
	0x19, 0xE7, 0x34, 0x08, 0x19, 0xE0, 0xF6, 0x9C,
	0x67, 0x8C, 0x3D, 0xB8, 0xE6, 0xF6, 0xA9, 0x1A,
};

uint8_t CTR_CB4[] = {
	0x00, 0x96, 0xB0, 0x3B, 0x02, 0x0C, 0x6E, 0xAD,
	0xC2, 0xCB, 0x50, 0x0D,
};

uint8_t CTR_DATA4[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};

uint8_t CTR_RES4[] = {
	0x45, 0x32, 0x43, 0xFC, 0x60, 0x9B, 0x23, 0x32,
	0x7E, 0xDF, 0xAA, 0xFA, 0x71, 0x31, 0xCD, 0x9F,
	0x84, 0x90, 0x70, 0x1C, 0x5A, 0xD4, 0xA7, 0x9C,
	0xFC, 0x1F, 0xE0, 0xFF, 0x42, 0xF4, 0xFB, 0x00,
};

uint8_t CTR_KEY5[] = {
	0x02, 0xBF, 0x39, 0x1E, 0xE8, 0xEC, 0xB1, 0x59,
	0xB9, 0x59, 0x61, 0x7B, 0x09, 0x65, 0x27, 0x9B,
	0xF5, 0x9B, 0x60, 0xA7, 0x86, 0xD3, 0xE0, 0xFE,
};

uint8_t CTR_CB5[] = {
	0x00, 0x07, 0xBD, 0xFD, 0x5C, 0xBD, 0x60, 0x27,
	0x8D, 0xCC, 0x09, 0x12,
};

uint8_t CTR_DATA5[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23,
};

uint8_t CTR_RES5[] = {
	0x96, 0x89, 0x3F, 0xC5, 0x5E, 0x5C, 0x72, 0x2F,
	0x54, 0x0B, 0x7D, 0xD1, 0xDD, 0xF7, 0xE7, 0x58,
	0xD2, 0x88, 0xBC, 0x95, 0xC6, 0x91, 0x65, 0x88,
	0x45, 0x36, 0xC8, 0x11, 0x66, 0x2F, 0x21, 0x88,
	0xAB, 0xEE, 0x09, 0x35,
};

uint8_t CTR_KEY6[] = {
	0x77, 0x6B, 0xEF, 0xF2, 0x85, 0x1D, 0xB0, 0x6F,
	0x4C, 0x8A, 0x05, 0x42, 0xC8, 0x69, 0x6F, 0x6C,
	0x6A, 0x81, 0xAF, 0x1E, 0xEC, 0x96, 0xB4, 0xD3,
	0x7F, 0xC1, 0xD6, 0x89, 0xE6, 0xC1, 0xC1, 0x04,
};

uint8_t CTR_CB6[] = {
	0x00, 0x00, 0x00, 0x60, 0xDB, 0x56, 0x72, 0xC9,
	0x7A, 0xA8, 0xF0, 0xB2,
};

uint8_t CTR_DATA6[] = {
	0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
	0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67,
};

uint8_t CTR_RES6[] = {
	0x14, 0x5A, 0xD0, 0x1D, 0xBF, 0x82, 0x4E, 0xC7,
	0x56, 0x08, 0x63, 0xDC, 0x71, 0xE3, 0xE0, 0xC0,
};

uint8_t CTR_KEY7[] = {
	0xF6, 0xD6, 0x6D, 0x6B, 0xD5, 0x2D, 0x59, 0xBB,
	0x07, 0x96, 0x36, 0x58, 0x79, 0xEF, 0xF8, 0x86,
	0xC6, 0x6D, 0xD5, 0x1A, 0x5B, 0x6A, 0x99, 0x74,
	0x4B, 0x50, 0x59, 0x0C, 0x87, 0xA2, 0x38, 0x84,
};

uint8_t CTR_CB7[] = {
	0x00, 0xFA, 0xAC, 0x24, 0xC1, 0x58, 0x5E, 0xF1,
	0x5A, 0x43, 0xD8, 0x75,
};

uint8_t CTR_DATA7[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};

uint8_t CTR_RES7[] = {
	0xF0, 0x5E, 0x23, 0x1B, 0x38, 0x94, 0x61, 0x2C,
	0x49, 0xEE, 0x00, 0x0B, 0x80, 0x4E, 0xB2, 0xA9,
	0xB8, 0x30, 0x6B, 0x50, 0x8F, 0x83, 0x9D, 0x6A,
	0x55, 0x30, 0x83, 0x1D, 0x93, 0x44, 0xAF, 0x1C,
};

uint8_t CTR_KEY8[] = {
	0xFF, 0x7A, 0x61, 0x7C, 0xE6, 0x91, 0x48, 0xE4,
	0xF1, 0x72, 0x6E, 0x2F, 0x43, 0x58, 0x1D, 0xE2,
	0xAA, 0x62, 0xD9, 0xF8, 0x05, 0x53, 0x2E, 0xDF,
	0xF1, 0xEE, 0xD6, 0x87, 0xFB, 0x54, 0x15, 0x3D,
};

uint8_t CTR_CB8[] = {
	0x00, 0x1C, 0xC5, 0xB7, 0x51, 0xA5, 0x1D, 0x70,
	0xA1, 0xC1, 0x11, 0x48,
};

uint8_t CTR_DATA8[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23,
};

uint8_t CTR_RES8[] = {
	0xEB, 0x6C, 0x52, 0x82, 0x1D, 0x0B, 0xBB, 0xF7,
	0xCE, 0x75, 0x94, 0x46, 0x2A, 0xCA, 0x4F, 0xAA,
	0xB4, 0x07, 0xDF, 0x86, 0x65, 0x69, 0xFD, 0x07,
	0xF4, 0x8C, 0xC0, 0xB5, 0x83, 0xD6, 0x07, 0x1F,
	0x1E, 0xC0, 0xE6, 0xB8,
};

uint8_t *DATA[] = {
	CTR_DATA0, CTR_DATA1, CTR_DATA2, CTR_DATA3, CTR_DATA4,
	CTR_DATA5, CTR_DATA6, CTR_DATA7, CTR_DATA8,
};

size_t DATALEN[] = {
	sizeof (CTR_DATA0), sizeof (CTR_DATA1), sizeof (CTR_DATA2),
	sizeof (CTR_DATA3), sizeof (CTR_DATA4), sizeof (CTR_DATA5),
	sizeof (CTR_DATA6), sizeof (CTR_DATA7), sizeof (CTR_DATA8),
};

uint8_t *RES[] = {
	CTR_RES0, CTR_RES1, CTR_RES2, CTR_RES3, CTR_RES4,
	CTR_RES5, CTR_RES6, CTR_RES7, CTR_RES8,
};

size_t RESLEN[] = {
	sizeof (CTR_RES0), sizeof (CTR_RES1), sizeof (CTR_RES2),
	sizeof (CTR_RES3), sizeof (CTR_RES4), sizeof (CTR_RES5),
	sizeof (CTR_RES6), sizeof (CTR_RES7), sizeof (CTR_RES8),
};

uint8_t *KEY[] = {
	CTR_KEY0, CTR_KEY1, CTR_KEY2, CTR_KEY3, CTR_KEY4,
	CTR_KEY5, CTR_KEY6, CTR_KEY7, CTR_KEY8,
};

size_t KEYLEN[] = {
	sizeof (CTR_KEY0), sizeof (CTR_KEY1), sizeof (CTR_KEY2),
	sizeof (CTR_KEY3), sizeof (CTR_KEY4), sizeof (CTR_KEY5),
	sizeof (CTR_KEY6), sizeof (CTR_KEY7), sizeof (CTR_KEY8),
};

uint8_t *CB[] = {
	CTR_CB0, CTR_CB1, CTR_CB2, CTR_CB3, CTR_CB4,
	CTR_CB5, CTR_CB6, CTR_CB7, CTR_CB8,
};

#ifdef __cplusplus
}
#endif

#endif /* _AES_CTR_H */
