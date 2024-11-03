extern int main(void);

// Symbols defined by the linker script
extern unsigned int _stack;
extern unsigned int _idata;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;

// Global (and static local) variables initialized to non-zero value
static void copy_data(void)
{
    unsigned int *src_data_ptr = &_idata;
    unsigned int *dst_data_ptr = &_data;
    while (dst_data_ptr < &_edata) {
        *dst_data_ptr++ = *src_data_ptr++;
    }
}

static void clear_bss(void)
{
    unsigned int *bss_ptr = &_bss;
    while (bss_ptr < &_ebss) {
        *bss_ptr++ = 0;
    }
}

void isr_reset(void) {
    copy_data();
    clear_bss();
    main();

    // We should never return from main, but just in case we do
    while(1);
}

void isr_hardfault(void) {
    while(1);
}

#define IVT_ARRAY_SIZE (48U)
typedef void (*isr_t)(void);
__attribute((used, section(".ivt")))
static const isr_t ivt[IVT_ARRAY_SIZE] =
{
    (isr_t)&_stack,
    isr_reset,
    0, // isr_nmi
    isr_hardfault,

    // Incomplete table (only for demo)! The rest of the ISRs defaults to
    // value 0. Common practice otherwise is to give these a default handler
    // and attribute them weak.
};
