#define IMG_WIDTH 128
#define IMG_HEIGHT 64

static uint8_t lookupTable[] = {    
    
    // 1st line
    0x00,0x26,0x6f,0x49,0x49,0x7b,0x32,0x00, // 83=S
    0x00,0x7f,0x7f,0x30,0x18,0x30,0x7f,0x7f, // 87=W
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=     
    0x00,0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00, // 86=V 
    0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00, // 51=3
    0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00, // 46=.
    0x00,0x62,0x73,0x51,0x49,0x4f,0x46,0x00, // 50=2
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    // 2nd line
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
    // 3d line
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
    // 4th (last) line
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
};
