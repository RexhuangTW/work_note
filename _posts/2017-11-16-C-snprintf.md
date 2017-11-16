---
title: snprintf 
categories:
- C
tags: C
---
為什麼要使用`snprintf`？ 

最大的優點就是：可以限制長度，避免overflow。

`strcpy`及`strncpy`都可以用`snprintf`取代，可以降低overflow機會發生。

## Prototype
{% highlight C linenos %}
int snprintf ( char * s, size_t n, const char * format, ... );
{% endhighlight %}

## Example
{% highlight C linenos %}
#include "stdio.h"
#include "string.h"
int main()
{
    char dest[11]= {0}, *src= "1234567890A";
    
    snprintf(dest, sizeof(dest), "%s", src);
    
    printf("dest = %s\n", dest);
    
    return 0;
}
{% endhighlight %}

## 總結
執行結果：```dest = 1234567890```

可以發現目標的長度只有11，但來源實際的長度是12(包含字串結尾\0)，透過`snprintf`限制了存入目標的長度已防止overflow產生。
所以在使用上需要注意的地方是目標的長度必須大於或等於來源的長度才不會造成資料短缺。



