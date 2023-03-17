#define IMG_WIDTH 128
#define IMG_HEIGHT 64

static uint8_t lookupTable[] = {    
    
    // 1st line
    0x00,0x26,0x6f,0x49,0x49,0x7b,0x32,0x00, // 83=S
    0x00,0x7f,0x7f,0x30,0x18,0x30,0x7f,0x7f, // 87=W
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00, // 86=V 
    0x00,0x18,0x18,0x14,0x16,0x7f,0x7f,0x10, // 52=4
    0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00, // 46=.
    0x00,0x3e,0x7f,0x49,0x45,0x7f,0x3e,0x00, // 48=0
    0x00,0x27,0x67,0x45,0x45,0x7d,0x39,0x00, // 52=5
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    // 2nd line
    0x00,0x7f,0x7f,0x09,0x09,0x0f,0x06,0x00, // 80=P
    0x00,0x7f,0x7f,0x30,0x18,0x30,0x7f,0x7f, // 87=W
    0x00,0x7f,0x7f,0x06,0x0c,0x06,0x7f,0x7f, // 77=M
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00, // 49=1
    0x00,0x3e,0x7f,0x49,0x45,0x7f,0x3e,0x00, // 48=0
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x7f,0x7f,0x30,0x38,0x6c,0x44,0x00, // 107=k
    0x00,0x7f,0x7f,0x08,0x08,0x7f,0x7f,0x00, // 72=H
    0x00,0x44,0x64,0x74,0x5c,0x4c,0x44,0x00, // 122=z
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    // 3d line
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    // 4th (last) line
    0x00,0x3e,0x7f,0x41,0x41,0x63,0x22,0x00, // 67=C
    0x00,0x7f,0x7f,0x09,0x09,0x0f,0x06,0x00, // 80=P
    0x00,0x7f,0x7f,0x06,0x0c,0x06,0x7f,0x7f, // 77=M 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
          
};
