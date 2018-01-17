---
title: Linked Lists
categories:
- C
tags: C programing
---

什麼是Linked List？

* 有次序排列之資料稱為串列(List)，如一年四季，數字0~9。
* 鏈結串列各元素在記憶體之位置是不連續、隨機的。它是由動態記憶體分配節點(Node)串接而成。(相形之下，陣列為一個循序的記憶體結構)。

在介紹Linked List先簡單介紹一下結構陣列。
{% highlight C linenos %}
#include <stdio.h>
struct team_member{
    int age;
    char name[10];
};

int main(){
    int size;
    struct team_member team1[3] ={
        26, {'A', 'n', 'd', 'y', '\0'},
        27, {'M', 'i', 'k', 'e', '\0'},
        28, {'P', 'a', 'u', 'l', '\0'},
    };
    
    size = sizeof(team1)/sizeof(struct team_member);
    
    for(int i = 0; i < size; i++){
        printf("Age :%d \t", team1[i].age);
        printf("Name :%s \n", team1[i].name);
    };
    return 0;
}
{% endhighlight %}
執行結果：
![](https://i.imgur.com/IIBba5N.png)

從上面的code可以得知我們宣告了一個struct且裡面有三個element。
程式在執行時會將資料儲存至記憶體且會依序排放在記憶體空間內。
如果需要在第一組element及第二組element之間新增一組element，要怎麼做呢？
![](https://i.imgur.com/namXCW5.png)
使用陣列或是結構陣列的情況下，我們必須要從第二組element開始往後移動，騰出一組空間儲存新的資料，所以不管是新增或是刪除都會導致資料移動頻繁，減少系統的效率，且程式也不易維護。

為了解決這些問題，就產生了Linked List的資料型態。

接下來會介紹該如何使用Linked List（單向）

## Single-Linked List 簡介
* Single-Linked List的結構如下圖所示
![](https://i.imgur.com/nDliOZh.png)
其中 Head : 指向串列的指標；Tail : 指向串列尾端的指標

* Linked List 透過儲存記憶體的位址取得下一個節點。
```
Node = Data + address
```
上面結構陣列改用Linked List之後資料型態如下圖所示：
![](https://i.imgur.com/wQXMQHN.png)
這樣就可以方便的新增、刪除及移動各個元素，如需要再新增一組資料就如下圖所示：
![](https://i.imgur.com/k0PVQCJ.png)
這樣我們就只需要去改變指標所指向的address就可以將資料排列成我們所需要的方式，且比一般的陣列或是結構陣列還要彈性及可以減少記憶體空間的不必要的浪費。

接下來介紹如何在程式中使用Linked List

## Prototype
{% highlight C linenos %}
struct ListNode {
    int val;
    struct ListNode *next;
};
{% endhighlight %}

## How to use ?

首先建立一個你需要的Struct型態，以上面結構陣列來作範例：
{% highlight C linenos %}
struct team_member {
    int age;
    char name[10];
    struct team_member *next;
};
{% endhighlight %}

### Step 1 
宣告一個指向 team_member的head，且需要分配空間給head
{% highlight C linenos %}
struct team_member *head = (struct team_member*)malloc(sizeof(struct team_member)); 
{% endhighlight %}

### Step 2
宣告一個現在被指到的 node 也就是 head :
{% highlight C linenos %}
struct team_member *current = head;
{% endhighlight %}
如需要新增節點就必須將 current 指向下一個 node:
{% highlight C linenos %}
struct team_member *tmp_t = (struct team_member*)malloc(sizeof(struct team_member)); 
current->next = tmp_t;
{% endhighlight %}
同時，為了能夠建立後續的 node，必須把 current 往前移動一個節點，使 current 能夠指向最新建立的 node:
{% highlight C linenos %}
current = current->next;
{% endhighlight %}
如不需要再新增 node 也就是把最後一個node指向 NULL ：
{% highlight C linenos %}
current->next = NULL;
{% endhighlight %}

### Example
{% highlight C linenos %}
#include <stdio.h>
#include <string.h>

struct team_member {
    int age;
    char *name;
    struct team_member *next;
};

int main(){
    
    char *name[]={"Andy","Mike","Paul",NULL};
    int age[3]={26,27,278};
    
    // Create head
    struct team_member *head = (struct team_member*)malloc(sizeof(struct team_member));
    
    printf("=== Linked List demo === \n");
    
    //Add data at head
    head->name=name[0];
    head->age=age[0];
    head->next = NULL;
    
    //Now, let current point to Head
    struct team_member *current = head;
    
    for(int i=1;i<3;i++)
    {
        //let current->next point to new node
        struct team_member *tmp_t = (struct team_member*)malloc(sizeof(struct team_member));
        current->next = tmp_t;
        
        //let current point to new node and add data
        current = current->next;
        current->age=age[i];
        current->name=name[i];
        current->next = NULL;
    }
    
    // let current point to head again
    current = head;
    
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
 
    return 0;
}
{% endhighlight %}
執行結果：
![](https://i.imgur.com/mEbVc8O.png)

## 刪除 node

刪除 node 基本上會有兩種狀況：

### 刪除第一個 node 

刪除第一個 node 比較簡單，直接讓 head 指向下一個 node 就行了，但記得要釋放被刪除 node 的記憶體空間。
![](https://i.imgur.com/fIptHKJ.png)

### 刪除第一個 node 以外的任何一個 node

利用 temp 來找這個被刪除的 node，再利用 current 跟在 temp 後面。
找到目標後將 current->next 指向 temp->next 之後就可以釋放temp的記憶體空間。
![](https://i.imgur.com/xxUIrtl.png)

### Example
{% highlight C linenos %}
    // start delete Mike
    printf("=== Linked List delete demo === \n");
    char * dname="Mike";
    printf("Delete Mike ...\n");
    struct team_member *temp_t;

    /* 刪除第一個 node */
    temp_t = head;
    if(!strcmp(temp_t->name,dname))
    {
        head = head->next;
        free(temp_t);
    }
    else
    {
          /* 刪除第一個 node 以外的任何一個 node */
        temp_t = head;
        while(temp_t && strcmp(temp_t->name,dname))
        {
            current = temp_t;
            temp_t = temp_t->next;
        }
        if(!temp_t)
        {
            printf("Not found!\n");
        }
        else
        {
            current->next = temp_t->next;
            free(temp_t);
        }
    }
    // let current point to head again
    current = head;
    
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
{% endhighlight %}
執行結果：
![](https://i.imgur.com/tCx4Xuw.png)

## 新增 node
新增 node 跟刪除 node 一樣會有兩種狀況：
### 新增在最前面
新增在最前面就是將新增的 node 指向原本第一個 node 就可以了

### 新增在最前面以外的指定位置
* Step 1 
必須要先取得指定位置的前一個 node : current

* Step 2 
將新增的 node : insert 指向 current 所指向的 node 

* Step 3 
將current 指向 insert 

![](https://i.imgur.com/K3yvI7Y.png)

### Example
{% highlight C linenos %}
    // start insert Rex
    printf("=== Linked List insert demo === \n");
    char * i_name = "Rex";
    int i_age = 30;
    int locate = 0;
    int target = 1;
    
    struct team_member *insert_t = (struct team_member*)malloc(sizeof(struct team_member));
    insert_t->name = i_name;
    insert_t->age = i_age;
    
    current = head;

    /* 新增在最前面 */
    if(target == 0)
    {
        insert_t->next = current;
        head = insert_t;
    }
    else
    {
        /* 新增在最前面以外的指定位置 */
        while(current && locate < target - 1)
        {
            locate++;
            current = current->next;
        }
        insert_t->next = current->next;
        current->next = insert_t;
    }
    
    // let current point to head again
    current = head;
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
{% endhighlight %}
執行結果：
![](https://i.imgur.com/b6aVmyo.png)

以上就是 single-linked list的基本操作方式。

本文完整Sample code : 