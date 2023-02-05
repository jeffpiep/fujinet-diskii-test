#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <apple2.h>

#ifdef __INTELLISENSE__
// 18, expect closing parenthses - needed to use cc65 inline asm command with agruments.
  #pragma diag_suppress 18
#endif

#define	SLOT6 0x60
#define PHI_0_OFF 0xC080
#define PHI_0_ON  0xC081
#define PHI_1_OFF 0xC082
#define PHI_1_ON  0xC083
#define PHI_2_OFF 0xC084
#define PHI_2_ON  0xC085
#define PHI_3_OFF 0xC086
#define PHI_3_ON  0xC087
#define MOTOR_OFF 0xC088
#define MOTOR_ON  0xC089
#define SELECT_1  0xC08A
#define SELECT_2  0xC08B

void delay(uint16_t c)
{
	static uint8_t xcnt = 0;
	static uint8_t ycnt = 0;
	
	xcnt = c & 0xff;
	xcnt = (c >> 8) & 0xff;

	__asm__ ("ldy %v", ycnt); // 2 cycles
	__asm__ ("ldx %v", xcnt); // 2 cycles
l_delay:
	__asm__ ("dex"); // 2 cycles
	__asm__ ("bne %g", l_delay); //(3 cycles in loop, 2 cycles at end)
    __asm__ ("dey"); //           ; (2 cycles)
    __asm__ ("bne %g", l_delay);  // (3 cycles in loop, 2 cycles at end)
}

void drive_enable()
{
	__asm__ ("ldx #%b", SLOT6);
	__asm__ ("lda %w, x", SELECT_1);
	__asm__ ("lda %w, x", MOTOR_ON);
}

void main(void)
{
	drive_enable();
	printf("hello ");
	delay(0xffff);
	while (1)
	{
	}
}
