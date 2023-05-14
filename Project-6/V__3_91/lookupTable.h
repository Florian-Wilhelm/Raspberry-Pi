//#define IMG_WIDTH 128
//#define IMG_HEIGHT 64

// this thing is more or less a workaround so far

/* 
Start Screen
*/
static uint8_t startScreen[] = {    
    
    // 1st line
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,       
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,       
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00, // V 
    0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00, // 3
    0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00, // .
    0x00,0x26,0x6f,0x49,0x49,0x7f,0x3e,0x00, // 9 
    0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00, // 1 
    // 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,
    // 2nd line
    0x00,0x3e,0x7f,0x41,0x41,0x63,0x22,0x00, // C
    0x00,0x7f,0x7f,0x09,0x09,0x0f,0x06,0x00, // P
    0x00,0x7f,0x7f,0x06,0x0c,0x06,0x7f,0x7f, // M 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // -
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // - 
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // -     
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    // 3d line
    0x00,0x38,0x7c,0x44,0x44,0x6c,0x28,0x00, // c
    0x00,0xfc,0xfc,0x24,0x24,0x3c,0x18,0x00, // p
    0x00,0x7c,0x7c,0x18,0x30,0x18,0x7c,0x7c, // m 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // -
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // - 
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // -     
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    // 4th (last) line
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,       
    0x00,0x7f,0x7f,0x49,0x49,0x7f,0x36,0x00, // B 
    0x00,0x7c,0x7e,0x0b,0x0b,0x7e,0x7c,0x00, // A 
    0x00,0x01,0x01,0x7f,0x7f,0x01,0x01,0x00, // T 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,         
};

/* 
characters that will be rendered on the display
*/
static uint8_t null[]  = {0x00,0x3e,0x7f,0x49,0x45,0x7f,0x3e,0x00}; 
static uint8_t one[]   = {0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00}; 
static uint8_t two[]   = {0x00,0x62,0x73,0x51,0x49,0x4f,0x46,0x00}; 
static uint8_t three[] = {0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00}; 
static uint8_t four[]  = {0x00,0x18,0x18,0x14,0x16,0x7f,0x7f,0x10}; 
static uint8_t five[]  = {0x00,0x27,0x67,0x45,0x45,0x7d,0x39,0x00}; 
static uint8_t six[]   = {0x00,0x3e,0x7f,0x49,0x49,0x7b,0x32,0x00}; 
static uint8_t seven[] = {0x00,0x03,0x03,0x79,0x7d,0x07,0x03,0x00}; 
static uint8_t eight[] = {0x00,0x36,0x7f,0x49,0x49,0x7f,0x36,0x00}; 
static uint8_t nine[]  = {0x00,0x26,0x6f,0x49,0x49,0x7f,0x3e,0x00};

//battery indication characters
static uint8_t full[] = {0x00,0x7f,0x7f,0x08,0x08,0x7f,0x7f,0x00}; // H
static uint8_t half_empty[] = {0x00,0x7f,0x7f,0x06,0x0c,0x06,0x7f,0x7f}; // M
static uint8_t empty[]  = {0x00,0x7f,0x7f,0x40,0x40,0x40,0x40,0x00}; // L

static uint8_t hot[]  = {0x00,0x00,0x00,0x4f,0x4f,0x00,0x00,0x00}; // ! 

static uint8_t implausible[] = {0x00,0x02,0x03,0x51,0x59,0x0f,0x06,0x00}; // ?
static uint8_t clr_digit[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static uint8_t arrow[] = {0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
                            0x00,0x41,0x41,0x63,0x36,0x1c,0x08,0x00}; // little arrow ->

