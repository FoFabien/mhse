

#ifndef SHA1_DEFINED
#define SHA1_DEFINED

namespace sha1
{
    void calc(const void* src, const int bytelength, unsigned char* hash);
    void toString(const unsigned char* hash, char* str);

} // namespace sha1

#endif // SHA1_DEFINED
