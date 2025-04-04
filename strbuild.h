
#ifndef DSPD_2_ASSIGNMENT_STRBUILD_H
#define DSPD_2_ASSIGNMENT_STRBUILD_H
#define DEFAULT_STRING_SIZE 16

#define STR_STDCALL __stdcall

#ifdef _WIN64
#define C_STR_PUBLIC(type)   __declspec(dllexport) type STR_STDCALL __attribute__((unused))
#elif __GNUC__
#define C_STR_PUBLIC(type)   __attribute__((visibility("default"))) type STR_STDCALL __attribute__((unused))
#endif

typedef struct _builder {
    char* _Dest;
    int _Size;
    int _Capacity;
}string_builder;

C_STR_PUBLIC(void) str_reallocate(string_builder *pBuilder);
C_STR_PUBLIC(string_builder*) builder();
C_STR_PUBLIC(void) str_append(string_builder* __restrict__ pBuilder, const char* __restrict__ source);
C_STR_PUBLIC(char*) str_get(string_builder* pBuilder);
C_STR_PUBLIC(void) str_serialize(string_builder *pBuilder, const char* fBuf);
C_STR_PUBLIC(void) str_deserialize(string_builder *pBuilder, const char* fBuf);
C_STR_PUBLIC(void) str_clear(string_builder *pBuilder);



#endif //DSPD_2_ASSIGNMENT_STRBUILD_H
