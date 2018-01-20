#include <lib/xshash.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
//#include <xspublicfun.h>
#include <zlib.h>


unsigned int APHash(char* str, unsigned int len)
{
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash;
}


u_int FNVHash(char* str, u_int len)
{
   static const unsigned int fnv_prime = 0x811C9DC5;
   unsigned int hash      = 0;
   unsigned int i         = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash *= fnv_prime;
      hash ^= (*str);
   }
   if(hash==0) {
     //  WriteLog("hasherror",str);
//      printf("hasherror %s\n",str);
   }
   return hash;
}
/* End Of FNV Hash Function */

unsigned int BKDRHash(char* str, unsigned int len)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}

u_int CRC32(u_char*pbyte,u_int len)
{
    return crc32(0,pbyte,len);
}


static const uint64_t crc64_tab[256] = {
    u_int64_t(0x0000000000000000), u_int64_t(0x7ad870c830358979),
    u_int64_t(0xf5b0e190606b12f2), u_int64_t(0x8f689158505e9b8b),
    u_int64_t(0xc038e5739841b68f), u_int64_t(0xbae095bba8743ff6),
    u_int64_t(0x358804e3f82aa47d), u_int64_t(0x4f50742bc81f2d04),
    u_int64_t(0xab28ecb46814fe75), u_int64_t(0xd1f09c7c5821770c),
    u_int64_t(0x5e980d24087fec87), u_int64_t(0x24407dec384a65fe),
    u_int64_t(0x6b1009c7f05548fa), u_int64_t(0x11c8790fc060c183),
    u_int64_t(0x9ea0e857903e5a08), u_int64_t(0xe478989fa00bd371),
    u_int64_t(0x7d08ff3b88be6f81), u_int64_t(0x07d08ff3b88be6f8),
    u_int64_t(0x88b81eabe8d57d73), u_int64_t(0xf2606e63d8e0f40a),
    u_int64_t(0xbd301a4810ffd90e), u_int64_t(0xc7e86a8020ca5077),
    u_int64_t(0x4880fbd87094cbfc), u_int64_t(0x32588b1040a14285),
    u_int64_t(0xd620138fe0aa91f4), u_int64_t(0xacf86347d09f188d),
    u_int64_t(0x2390f21f80c18306), u_int64_t(0x594882d7b0f40a7f),
    u_int64_t(0x1618f6fc78eb277b), u_int64_t(0x6cc0863448deae02),
    u_int64_t(0xe3a8176c18803589), u_int64_t(0x997067a428b5bcf0),
    u_int64_t(0xfa11fe77117cdf02), u_int64_t(0x80c98ebf2149567b),
    u_int64_t(0x0fa11fe77117cdf0), u_int64_t(0x75796f2f41224489),
    u_int64_t(0x3a291b04893d698d), u_int64_t(0x40f16bccb908e0f4),
    u_int64_t(0xcf99fa94e9567b7f), u_int64_t(0xb5418a5cd963f206),
    u_int64_t(0x513912c379682177), u_int64_t(0x2be1620b495da80e),
    u_int64_t(0xa489f35319033385), u_int64_t(0xde51839b2936bafc),
    u_int64_t(0x9101f7b0e12997f8), u_int64_t(0xebd98778d11c1e81),
    u_int64_t(0x64b116208142850a), u_int64_t(0x1e6966e8b1770c73),
    u_int64_t(0x8719014c99c2b083), u_int64_t(0xfdc17184a9f739fa),
    u_int64_t(0x72a9e0dcf9a9a271), u_int64_t(0x08719014c99c2b08),
    u_int64_t(0x4721e43f0183060c), u_int64_t(0x3df994f731b68f75),
    u_int64_t(0xb29105af61e814fe), u_int64_t(0xc849756751dd9d87),
    u_int64_t(0x2c31edf8f1d64ef6), u_int64_t(0x56e99d30c1e3c78f),
    u_int64_t(0xd9810c6891bd5c04), u_int64_t(0xa3597ca0a188d57d),
    u_int64_t(0xec09088b6997f879), u_int64_t(0x96d1784359a27100),
    u_int64_t(0x19b9e91b09fcea8b), u_int64_t(0x636199d339c963f2),
    u_int64_t(0xdf7adabd7a6e2d6f), u_int64_t(0xa5a2aa754a5ba416),
    u_int64_t(0x2aca3b2d1a053f9d), u_int64_t(0x50124be52a30b6e4),
    u_int64_t(0x1f423fcee22f9be0), u_int64_t(0x659a4f06d21a1299),
    u_int64_t(0xeaf2de5e82448912), u_int64_t(0x902aae96b271006b),
    u_int64_t(0x74523609127ad31a), u_int64_t(0x0e8a46c1224f5a63),
    u_int64_t(0x81e2d7997211c1e8), u_int64_t(0xfb3aa75142244891),
    u_int64_t(0xb46ad37a8a3b6595), u_int64_t(0xceb2a3b2ba0eecec),
    u_int64_t(0x41da32eaea507767), u_int64_t(0x3b024222da65fe1e),
    u_int64_t(0xa2722586f2d042ee), u_int64_t(0xd8aa554ec2e5cb97),
    u_int64_t(0x57c2c41692bb501c), u_int64_t(0x2d1ab4dea28ed965),
    u_int64_t(0x624ac0f56a91f461), u_int64_t(0x1892b03d5aa47d18),
    u_int64_t(0x97fa21650afae693), u_int64_t(0xed2251ad3acf6fea),
    u_int64_t(0x095ac9329ac4bc9b), u_int64_t(0x7382b9faaaf135e2),
    u_int64_t(0xfcea28a2faafae69), u_int64_t(0x8632586aca9a2710),
    u_int64_t(0xc9622c4102850a14), u_int64_t(0xb3ba5c8932b0836d),
    u_int64_t(0x3cd2cdd162ee18e6), u_int64_t(0x460abd1952db919f),
    u_int64_t(0x256b24ca6b12f26d), u_int64_t(0x5fb354025b277b14),
    u_int64_t(0xd0dbc55a0b79e09f), u_int64_t(0xaa03b5923b4c69e6),
    u_int64_t(0xe553c1b9f35344e2), u_int64_t(0x9f8bb171c366cd9b),
    u_int64_t(0x10e3202993385610), u_int64_t(0x6a3b50e1a30ddf69),
    u_int64_t(0x8e43c87e03060c18), u_int64_t(0xf49bb8b633338561),
    u_int64_t(0x7bf329ee636d1eea), u_int64_t(0x012b592653589793),
    u_int64_t(0x4e7b2d0d9b47ba97), u_int64_t(0x34a35dc5ab7233ee),
    u_int64_t(0xbbcbcc9dfb2ca865), u_int64_t(0xc113bc55cb19211c),
    u_int64_t(0x5863dbf1e3ac9dec), u_int64_t(0x22bbab39d3991495),
    u_int64_t(0xadd33a6183c78f1e), u_int64_t(0xd70b4aa9b3f20667),
    u_int64_t(0x985b3e827bed2b63), u_int64_t(0xe2834e4a4bd8a21a),
    u_int64_t(0x6debdf121b863991), u_int64_t(0x1733afda2bb3b0e8),
    u_int64_t(0xf34b37458bb86399), u_int64_t(0x8993478dbb8deae0),
    u_int64_t(0x06fbd6d5ebd3716b), u_int64_t(0x7c23a61ddbe6f812),
    u_int64_t(0x3373d23613f9d516), u_int64_t(0x49aba2fe23cc5c6f),
    u_int64_t(0xc6c333a67392c7e4), u_int64_t(0xbc1b436e43a74e9d),
    u_int64_t(0x95ac9329ac4bc9b5), u_int64_t(0xef74e3e19c7e40cc),
    u_int64_t(0x601c72b9cc20db47), u_int64_t(0x1ac40271fc15523e),
    u_int64_t(0x5594765a340a7f3a), u_int64_t(0x2f4c0692043ff643),
    u_int64_t(0xa02497ca54616dc8), u_int64_t(0xdafce7026454e4b1),
    u_int64_t(0x3e847f9dc45f37c0), u_int64_t(0x445c0f55f46abeb9),
    u_int64_t(0xcb349e0da4342532), u_int64_t(0xb1eceec59401ac4b),
    u_int64_t(0xfebc9aee5c1e814f), u_int64_t(0x8464ea266c2b0836),
    u_int64_t(0x0b0c7b7e3c7593bd), u_int64_t(0x71d40bb60c401ac4),
    u_int64_t(0xe8a46c1224f5a634), u_int64_t(0x927c1cda14c02f4d),
    u_int64_t(0x1d148d82449eb4c6), u_int64_t(0x67ccfd4a74ab3dbf),
    u_int64_t(0x289c8961bcb410bb), u_int64_t(0x5244f9a98c8199c2),
    u_int64_t(0xdd2c68f1dcdf0249), u_int64_t(0xa7f41839ecea8b30),
    u_int64_t(0x438c80a64ce15841), u_int64_t(0x3954f06e7cd4d138),
    u_int64_t(0xb63c61362c8a4ab3), u_int64_t(0xcce411fe1cbfc3ca),
    u_int64_t(0x83b465d5d4a0eece), u_int64_t(0xf96c151de49567b7),
    u_int64_t(0x76048445b4cbfc3c), u_int64_t(0x0cdcf48d84fe7545),
    u_int64_t(0x6fbd6d5ebd3716b7), u_int64_t(0x15651d968d029fce),
    u_int64_t(0x9a0d8ccedd5c0445), u_int64_t(0xe0d5fc06ed698d3c),
    u_int64_t(0xaf85882d2576a038), u_int64_t(0xd55df8e515432941),
    u_int64_t(0x5a3569bd451db2ca), u_int64_t(0x20ed197575283bb3),
    u_int64_t(0xc49581ead523e8c2), u_int64_t(0xbe4df122e51661bb),
    u_int64_t(0x3125607ab548fa30), u_int64_t(0x4bfd10b2857d7349),
    u_int64_t(0x04ad64994d625e4d), u_int64_t(0x7e7514517d57d734),
    u_int64_t(0xf11d85092d094cbf), u_int64_t(0x8bc5f5c11d3cc5c6),
    u_int64_t(0x12b5926535897936), u_int64_t(0x686de2ad05bcf04f),
    u_int64_t(0xe70573f555e26bc4), u_int64_t(0x9ddd033d65d7e2bd),
    u_int64_t(0xd28d7716adc8cfb9), u_int64_t(0xa85507de9dfd46c0),
    u_int64_t(0x273d9686cda3dd4b), u_int64_t(0x5de5e64efd965432),
    u_int64_t(0xb99d7ed15d9d8743), u_int64_t(0xc3450e196da80e3a),
    u_int64_t(0x4c2d9f413df695b1), u_int64_t(0x36f5ef890dc31cc8),
    u_int64_t(0x79a59ba2c5dc31cc), u_int64_t(0x037deb6af5e9b8b5),
    u_int64_t(0x8c157a32a5b7233e), u_int64_t(0xf6cd0afa9582aa47),
    u_int64_t(0x4ad64994d625e4da), u_int64_t(0x300e395ce6106da3),
    u_int64_t(0xbf66a804b64ef628), u_int64_t(0xc5bed8cc867b7f51),
    u_int64_t(0x8aeeace74e645255), u_int64_t(0xf036dc2f7e51db2c),
    u_int64_t(0x7f5e4d772e0f40a7), u_int64_t(0x05863dbf1e3ac9de),
    u_int64_t(0xe1fea520be311aaf), u_int64_t(0x9b26d5e88e0493d6),
    u_int64_t(0x144e44b0de5a085d), u_int64_t(0x6e963478ee6f8124),
    u_int64_t(0x21c640532670ac20), u_int64_t(0x5b1e309b16452559),
    u_int64_t(0xd476a1c3461bbed2), u_int64_t(0xaeaed10b762e37ab),
    u_int64_t(0x37deb6af5e9b8b5b), u_int64_t(0x4d06c6676eae0222),
    u_int64_t(0xc26e573f3ef099a9), u_int64_t(0xb8b627f70ec510d0),
    u_int64_t(0xf7e653dcc6da3dd4), u_int64_t(0x8d3e2314f6efb4ad),
    u_int64_t(0x0256b24ca6b12f26), u_int64_t(0x788ec2849684a65f),
    u_int64_t(0x9cf65a1b368f752e), u_int64_t(0xe62e2ad306bafc57),
    u_int64_t(0x6946bb8b56e467dc), u_int64_t(0x139ecb4366d1eea5),
    u_int64_t(0x5ccebf68aecec3a1), u_int64_t(0x2616cfa09efb4ad8),
    u_int64_t(0xa97e5ef8cea5d153), u_int64_t(0xd3a62e30fe90582a),
    u_int64_t(0xb0c7b7e3c7593bd8), u_int64_t(0xca1fc72bf76cb2a1),
    u_int64_t(0x45775673a732292a), u_int64_t(0x3faf26bb9707a053),
    u_int64_t(0x70ff52905f188d57), u_int64_t(0x0a2722586f2d042e),
    u_int64_t(0x854fb3003f739fa5), u_int64_t(0xff97c3c80f4616dc),
    u_int64_t(0x1bef5b57af4dc5ad), u_int64_t(0x61372b9f9f784cd4),
    u_int64_t(0xee5fbac7cf26d75f), u_int64_t(0x9487ca0fff135e26),
    u_int64_t(0xdbd7be24370c7322), u_int64_t(0xa10fceec0739fa5b),
    u_int64_t(0x2e675fb4576761d0), u_int64_t(0x54bf2f7c6752e8a9),
    u_int64_t(0xcdcf48d84fe75459), u_int64_t(0xb71738107fd2dd20),
    u_int64_t(0x387fa9482f8c46ab), u_int64_t(0x42a7d9801fb9cfd2),
    u_int64_t(0x0df7adabd7a6e2d6), u_int64_t(0x772fdd63e7936baf),
    u_int64_t(0xf8474c3bb7cdf024), u_int64_t(0x829f3cf387f8795d),
    u_int64_t(0x66e7a46c27f3aa2c), u_int64_t(0x1c3fd4a417c62355),
    u_int64_t(0x935745fc4798b8de), u_int64_t(0xe98f353477ad31a7),
    u_int64_t(0xa6df411fbfb21ca3), u_int64_t(0xdc0731d78f8795da),
    u_int64_t(0x536fa08fdfd90e51), u_int64_t(0x29b7d047efec8728),
};

u_int64_t crc64(uint64_t crc, const u_char *s, u_int64_t l) {
    u_int64_t j;

    for (j = 0; j < l; j++) {
        uint8_t byte = s[j];
        crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
    }
    return crc;
}


u_int64_t CRC64(const u_char*pbyte,u_int64_t len)
{
     return crc64(0,pbyte,len);
}

int64_t crcint64(uint64_t crc, const u_char *s, u_int64_t l) {
    u_int64_t j;

    for (j = 0; j < l; j++) {
        uint8_t byte = s[j];
        crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
    }
    return crc;
}

int64_t CRCInt64(const u_char*pbyte,u_int64_t len)
{
     return crcint64(0,pbyte,len);
}

