char* mystrcpy(char* dest, const char* src){
    if(src == nullptr || dest == nullptr)
        return nullptr;
    char* res = dest;
    while(*src != '\0')
        *dest++ = *src++;
    if(*dest != '\0')
        *dest = '\0';
    return res;
}

char* mystrncpy(char* dest, const char* src, int len){
    if(src == nullptr || dest == nullptr || len <= 0)
        return nullptr;
    char* res = dest;
    while(len > 0 && *src != '\0'){
        *dest++ = *src++;
        --len;
    }
    if(*dest != '\0')
        *dest = '\0';
    return res;
}

// memcpy可以用于非字符串，且可能会出现内存重叠的场景；
void* mymemcpy(void* dest, void* src, int len){
    if(src == nullptr || dest == nullptr || len <= 0)
        return nullptr;
    char* tmpsrc = static_cast<char*>(src);
    char* tmpdest = static_cast<char*>(dest);
    if((tmpdest+len<tmpsrc) || (tmpdest<tmpsrc)){
        while(len){
            *tmpdest++ = *tmpsrc++;
            --len;
        }
    }
    else{
        tmpsrc += len;
        tmpdest += len;
        while(len){
            *tmpdest-- = *tmpsrc--;
            --len;
        }
    }
    return dest;
}
