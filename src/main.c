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
	ycnt = (c >> 8) & 0xff;

	__asm__ volatile ("txa");
	__asm__ volatile ("pha");
	__asm__ volatile ("tya");
	__asm__ volatile ("pha");
	
	__asm__ volatile ("ldy %v", ycnt); // 2 cycles
l_outer:
	__asm__ volatile ("ldx %v", xcnt); // 2 cycles
l_inner:
	__asm__ volatile ("dex"); // 2 cycles
	__asm__ volatile ("bne %g", l_inner); //(3 cycles in loop, 2 cycles at end)
    __asm__ volatile ("dey"); //           ; (2 cycles)
    __asm__ volatile ("bne %g", l_outer);  // (3 cycles in loop, 2 cycles at end)

	__asm__ volatile ("pla");
	__asm__ volatile ("tay");
	__asm__ volatile ("pla");
	__asm__ volatile ("tax");

	// minimum overhead is 3.4 ms cause CC65 is slow here

	// inner loop takes 1 + X*5 cycles
	// outer loop adds 5 cycles to inner loop then * Y
	// approx: (6 + X*5)*Y
	// example 0xffff = (6 + 5*255)*255 = 326k cycles or 326 ms

}

void drive_enable()
{
	__asm__ volatile ("ldx #%b", SLOT6);
	__asm__ volatile ("lda %w, x", SELECT_1);
	__asm__ volatile ("lda %w, x", MOTOR_ON);
}

void drive_disable()
{
	__asm__ volatile ("ldx #%b", SLOT6);
	__asm__ volatile ("lda %w, x", SELECT_1);
	__asm__ volatile ("lda %w, x", MOTOR_OFF);
}

void sequence()
{
	int i;
	for (i = 0; i < 10; i++)
	{
		__asm__ volatile("lda %w", PHI_0_ON + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_3_OFF + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_1_ON + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_0_OFF + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_2_ON + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_1_OFF + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_3_ON + SLOT6);
		delay(1);
		__asm__ volatile("lda %w", PHI_2_OFF + SLOT6);
		delay(1);
	}
}

void step01()
{
	__asm__ volatile ("ldx #%b", SLOT6);
	__asm__ volatile ("lda %w, x", PHI_0_ON);
	delay(1);
	__asm__ volatile ("lda %w, x", PHI_1_ON);
	delay(1);
	__asm__ volatile ("lda %w, x", PHI_0_OFF);
	delay(1);
	__asm__ volatile ("lda %w, x", PHI_1_OFF);
	
}

void step12()
{
	__asm__ volatile ("ldx #%b", SLOT6);
	__asm__ volatile ("lda %w, x", PHI_1_ON);
	delay(0x3030);
	__asm__ volatile ("lda %w, x", PHI_2_ON);
	delay(100/5);
	__asm__ volatile ("lda %w, x", PHI_1_OFF);
}

void step23()
{


}

void step30()
{}



void main(void)
{
	printf("hi ");
	delay(0xffff);
	printf("on ");
	drive_enable();
	delay(0xffff);
	sequence();
	delay(0xffff);
	printf("off ");
	drive_disable();

	while (1)
	{
	}
}
