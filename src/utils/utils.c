
#include "utils.h"

static void ft_bzero(void *s, size_t n)
{
    unsigned char *ptr;

    ptr = (unsigned char *)s;
    while (n--)
        *ptr++ = 0;
}

void *ft_calloc(size_t count, size_t size)
{
    void *ptr;

    if (!(ptr = malloc(count * size)))
        return (NULL);
    ft_bzero(ptr, count * size);
    return (ptr);
}

void    *ft_memcpy(void *_dest, const void *_src, size_t _n)
{
    char *dest;
    const char *src;

    dest = _dest;
    src = _src;
    while (_n--)
        *dest++ = *src++;
    return (_dest);
}

void   *ft_memset(void *b, int c, size_t len)
{
    unsigned char *ptr;

    ptr = (unsigned char *)b;
    while (len--)
        *ptr++ = (unsigned char)c;
    return (b);
}

int ft_strcmp(const char *_s1, const char *_s2){
    while (*_s1 && *_s1 == *_s2)
    {
        _s1++;
        _s2++;
    }
    return (*(unsigned char *)_s1 - *(unsigned char *)_s2);
}

static size_t ft_strlen(const char *s){
    size_t i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

size_t ft_strlcat(char *dst, const char *src, size_t siz){
    size_t dlen;
    size_t slen;
    size_t i;

    dlen = ft_strlen(dst);
    slen = ft_strlen(src);
    if (siz <= dlen)
        return (siz + slen);
    i = 0;
    while (src[i] && dlen + i < siz - 1)
    {
        dst[dlen + i] = src[i];
        i++;
    }
    dst[dlen + i] = 0;
    return (dlen + slen);
}

void	handle_error(char *msg)
{
    char	buff_str[255];

    if (ft_strlcat(buff_str, "ft_nm: ", 255) > 255
        || ft_strlcat(buff_str, msg, 255) > 255)
        exit(1);
    if (errno)
        perror(buff_str);
    else
        fprintf(stderr, "%s\n", buff_str);
    exit(EXIT_FAILURE);
}

void    address_to_hex(uint64_t addr, char *buffer, int buffer_len){
    const char *hex_digits = "0123456789abcdef";

    for (int i = buffer_len - 1; i != 0; i--){
        buffer[i] = hex_digits[addr % 16];
        addr /= 16;
    }
}









