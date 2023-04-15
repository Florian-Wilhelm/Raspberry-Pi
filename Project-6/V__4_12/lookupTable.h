#define IMG_WIDTH 128
#define IMG_HEIGHT 64

static uint8_t lookupTable[] = {    
    
    // 1st line
    0x00,0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00, // 86=V 
    0x00,0x18,0x18,0x14,0x16,0x7f,0x7f,0x10, // 52=4
    0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00, // 46=.
    0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00, // 49=1 
    0x00,0x62,0x73,0x51,0x49,0x4f,0x46,0x00, // 50=2    
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x1c,0x3e,0x63,0x41,0x00,0x00, // 40=( 
    0x00,0x7f,0x7f,0x49,0x49,0x7f,0x36,0x00, // 66=B
    0x00,0x7f,0x7f,0x40,0x40,0x40,0x40,0x00, // 76=L 
    0x00,0x7f,0x7f,0x49,0x49,0x41,0x41,0x00, // 69=E     
    0x00,0x00,0x41,0x63,0x3e,0x1c,0x00,0x00, // 41=) 
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
    0x00,0x38,0x7c,0x44,0x44,0x6c,0x28,0x00, // 99=c
    0x00,0xfc,0xfc,0x24,0x24,0x3c,0x18,0x00, // 112=p
    0x00,0x7c,0x7c,0x18,0x30,0x18,0x7c,0x7c, // 109=m 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // 45=-
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // 45=- 
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // 45=-    
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
    0x00,0x7f,0x7f,0x49,0x49,0x7f,0x36,0x00, // 66=B 
    0x00,0x7c,0x7e,0x0b,0x0b,0x7e,0x7c,0x00, // 65=A 
    0x00,0x01,0x01,0x7f,0x7f,0x01,0x01,0x00, // 84=T 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32=        
};