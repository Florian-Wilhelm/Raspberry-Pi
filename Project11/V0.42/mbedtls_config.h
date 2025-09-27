//Hardware config
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_HAVE_TIME

//error reporting
#define MBEDTLS_ERROR_C
//used by LwIP
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CTR_DRBG_C

//RSA KEY EXCHANGE
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_RSA_C

//general key exchange
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C

//encryption
#define MBEDTLS_AES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_AES_FEWER_TABLES
#define MBEDTLS_GCM_C

//certs
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_OID_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C

//hash methods
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA512_C

//TLS
#define MBEDTLS_CIPHER_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_MD_C

//enable client and server modes and TLS
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SERVER_NAME_INDICATION

#define MBEDTLS_SSL_SRV_C

//enable TLS 1.2
#define MBEDTLS_SSL_PROTO_TLS1_2

#include "/home/pi/pico/pico-sdk/lib/mbedtls/include/mbedtls/check_config.h"