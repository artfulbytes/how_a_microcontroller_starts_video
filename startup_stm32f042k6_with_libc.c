extern int main(void);
extern void __libc_init_array(void);

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

// Normally provided by Newlib's runtime file crti.o, but we compile without
// (-startfiles) Inserted by __libc_init_array because newlib was compiled with
// _HAVE_INIT_FINI. We don't need it, so add an empty stub.
void _init(void) {}

void isr_reset(void) {
    // You may implement system_init to perform additional early hardware
    // initialization depending on system requirements. This may include
    // configuring clocks, setting up memory or other peripherals, working
    // around hardware errata, and so on.
    // system_init();

    copy_data();
    clear_bss();
    __libc_init_array();
    main();

    // For a more feature-rich C runtime, you may link with the C runtime files
    // files (crt0.o, crti.o...) from Newlib. To do so, remove "-nostartfiles"
    // and replace __libc_init_array() and main() with a call to _start().
    // The runtime files from Newlib includes semi-hosting, profiling,
    // transactional memory, function hooks, and so on. Note, this will likely
    // require som additions in the linker script as well.
    // _start();

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
