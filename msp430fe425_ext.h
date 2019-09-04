__no_init volatile union
{
  unsigned short SD16CCTL2;   /* SD16 Channel 2 Control Register */

  struct
  {
    unsigned short SD16GRP         : 1; /* SD16 Grouping of Channels: 0:Off/1:On */
    unsigned short SD16SC          : 1; /* SD16 Start Conversion */
    unsigned short SD16IFG         : 1; /* SD16 Channel x Interrupt Flag */
    unsigned short SD16IE          : 1; /* SD16 Channel x Interrupt Enable */
    unsigned short SD16DF          : 1; /* SD16 Channel x Data Format: 0:Unipolar/1:Bipolar */
    unsigned short SD16OVIFG       : 1; /* SD16 Channel x Overflow Interrupt Flag */
    unsigned short SD16LSBACC      : 1; /* SD16 Channel x Access LSB of ADC */
    unsigned short SD16LSBTOG      : 1; /* SD16 Channel x Toggle LSB Output of ADC */
    unsigned short SD16OSR0        : 1; /* SD16 Channel x OverSampling Ratio 0 */
    unsigned short SD16OSR1        : 1; /* SD16 Channel x OverSampling Ratio 1 */
    unsigned short SD16SNGL        : 1; /* SD16 Channel x Single Conversion On/Off */
  }SD16CCTL2_bit;
} @0x0106;

__no_init volatile union
{
  unsigned char SD16INCTL2;   /* SD16 Input Control Register Channel 1 */

  struct
  {
    unsigned char SD16INCH0       : 1; /* SD16 Input Channel select 0 */
    unsigned char SD16INCH1       : 1; /* SD16 Input Channel select 1 */
    unsigned char SD16INCH2       : 1; /* SD16 Input Channel select 2 */
    unsigned char SD16GAIN0       : 1; /* SD16 Input Pre-Amplifier Gain Select 0 */
    unsigned char SD16GAIN1       : 1; /* SD16 Input Pre-Amplifier Gain Select 1 */
    unsigned char SD16GAIN2       : 1; /* SD16 Input Pre-Amplifier Gain Select 2 */
    unsigned char SD16INTDLY0     : 1; /* SD16 Interrupt Delay after 1.Conversion 0 */
    unsigned char SD16INTDLY1     : 1; /* SD16 Interrupt Delay after 1.Conversion 1 */
  }SD16INCTL2_bit;
} @0x00B2;

  /* SD16 Channel 1 Conversion Memory */
__no_init volatile unsigned short SD16MEM2 @ 0x0116;

 /* SD16 Preload Register Channel 1 */
__no_init volatile unsigned char SD16PRE2 @ 0x00BA;