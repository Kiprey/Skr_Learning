#include "csapp.h"
#include <semaphore.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

#define PREFIX_PRINT "proxy > "
#define MAX_CACHE_LINE 100
// 最大http header数量
#define MAXHEADERS 30
typedef struct {
    char name[MAXLINE];
    char value[MAXLINE];
}http_headers;

typedef struct{
    char requestType[MAXLINE];
    char Protocol[MAXLINE];
    char address[MAXLINE];
    char resource[MAXLINE];
    char port[10];
}http_target;

// 缓存(这部分需要设置线程互斥)
typedef struct{
    char address[MAXLINE];
    char content_type[MAXLINE];
    char* data;
    size_t size;
    size_t lru;
}cache_line;
size_t current_lru = 1, cache_nums = 0, cache_size = 0;
cache_line cache[MAX_CACHE_LINE];
// 互斥锁
pthread_mutex_t thread_mutex;

/*
    写入缓存。此函数需要设置为互斥，不允许同时写
*/
void writeCache(const char* address, const char* data, const size_t dataSize, const char* content_type)
{
    size_t index = cache_nums;
    // 如果cache已满(条目已满或大小已满)
    // 之所以用while，是因为可能存在驱逐一个cache_line后，新的数据还是放不下的情况
    while(cache_nums >= MAX_CACHE_LINE || cache_size + dataSize > MAX_CACHE_SIZE)
    {
        // 遍历所有cache，查找最近最少用的cache
        size_t minLRU = (size_t)-1;
        for(size_t i = 0; i < cache_nums; i++){
            if(cache[i].size > 0 && cache[i].lru < minLRU)
            {
                minLRU = cache[i].lru;
                index = i;
            }
        }
        // 将该cache驱逐
        free(cache[index].data);
        cache[index].size = 0;
        cache[index].address[0] = '\0';
        cache_nums--;
    }
    
    strncpy(cache[index].content_type, content_type, MAXLINE);
    strncpy(cache[index].address, address, MAXLINE);
    cache[index].size = dataSize;
    cache[index].data = malloc(dataSize);
    memcpy(cache[index].data, data, dataSize);
    cache[index].lru = current_lru;
    // lru递增
    current_lru++;
    cache_nums++;
    cache_size += cache[index].size;
}

/* 
    读取缓存。此函数需要设置互斥,因为要修改缓存的LRU
    如果在缓存中找到了数据，则返回该缓存行
*/
const cache_line* readCache(const char* address)
{
    for(size_t i = 0; i < cache_nums; i++){
        if(!strcmp(cache[i].address, address))
        {
            cache[i].lru = current_lru++;
            return &cache[i];
        }
    }
        
    return NULL;
}
// 初始化缓存
void initCache(void)
{
    for(size_t i = 0; i < MAX_CACHE_LINE; i++)
    {
        cache[i].data = NULL;
        cache[i].size = 0;
        cache[i].lru = 0;
    }
}

/*
    read_requesthdrs - 返回读取的http header条目个数
*/
int read_requesthdrs(rio_t *rp, http_headers headers[MAXHEADERS]) 
{
    int idx = 0;
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n"))
    {
        sscanf(buf, "%[^:]:%s\r\n", headers[idx].name, headers[idx].value);
        printf("%s: %s\r\n", headers[idx].name, headers[idx].value);
        idx++;
        Rio_readlineb(rp, buf, MAXLINE);
    }
    printf("\r\n");
    return idx;
}
/*
 * parse_uri 从给出的访问地址中，将地址，端口与资源地址分开
 */
void parse_uri(char *uri, http_target* targetServer) 
{
    // 以 http://www.baidu.com:80/index.html 为例
    char* addressPtr = NULL;
    // 获取协议名称
    if((addressPtr = strstr(uri, "//")))
        addressPtr += 2;
    else
        addressPtr = uri;
    // 此时的adressPtr -> www.baidu.com:80/index.html

    // 获取端口
    char* portPtr = strstr(addressPtr, ":");
    // 此时的portPtr -> NULL | :80/index.html

    // 获取资源名
    char* resourcePtr = strstr(addressPtr, "/");
    // 此时的resourcePtr -> NULL | /index.html

    if(resourcePtr != NULL)
        strncpy(targetServer->resource, resourcePtr, MAXLINE);
    // 如果没有指定资源，则默认 /
    else
    {
        strncpy(targetServer->resource, "/", 2);
        resourcePtr = addressPtr + strlen(addressPtr);
    }
    if(portPtr != NULL)
        strncpy(targetServer->port, portPtr + 1, resourcePtr-(portPtr+1));
    // 如果没有指定端口，则默认80
    else
    {
        strncpy(targetServer->port, "80", 3);
        portPtr = resourcePtr;
    }
    strncpy(targetServer->address, addressPtr, portPtr - addressPtr);
    printf(PREFIX_PRINT"targetResources: %s:%s%s\n", 
        targetServer->address, targetServer->port, targetServer->resource);
}
/*
    add_header - 添加新http header至结构体中
        注意，如果结构体中已经存在某条name，新value将不会被设置
*/
void add_header(http_headers headers[MAXHEADERS], int* header_num, const char* name, const char* value)
{
    // 查找是否存在相同的name
    for(int i = 0; i < *header_num; i++)
        if(!strcmp(name, headers[i].name))
            return;
    strncpy(headers[*header_num].name, name, MAXLINE);
    strncpy(headers[*header_num].value, value, MAXLINE);
    printf(PREFIX_PRINT"add_header: %s:%s\n", headers[*header_num].name, headers[*header_num].value);
    (*header_num)++;
}

/*
    transmitDataS2C - 将服务端所有数据转发给客户端
*/
void transmitDataS2C(int clientFd, int serverFd, const char* resAddr)
{
    char buf[2 * MAXLINE + 5];
    int size = 0, header_num = 0, contentSize = -1;
    rio_t serverRio;
    http_headers headers[MAXHEADERS], *content_type = NULL;

    Rio_readinitb(&serverRio, serverFd);
    // 读取并发送第一行的状态码
    rio_readlineb(&serverRio, buf, MAXLINE);
    printf("%s", buf);
    Rio_writen(clientFd, buf, strlen(buf));
    // 读取服务器返回的http头
    header_num = read_requesthdrs(&serverRio, headers);
    // 获取当前返回数据中的content-length与Content-type。并将http header全部发送给客户端
    for(int i = 0; i < header_num; i++){
        if(!strcmp("Content-length", headers[i].name)){
            sscanf(headers[i].value, "%d", &contentSize);
            printf(PREFIX_PRINT"current Content-size: %d\n", contentSize);
        }
        else if(!strcmp("Content-type", headers[i].name)){
            content_type = &headers[i];
            printf(PREFIX_PRINT"current Content_type: %s\n", content_type->value);
        }
        snprintf(buf, 2 * MAXLINE + 5, "%s: %s\r\n", headers[i].name, headers[i].value);
        printf("%s", buf);
        Rio_writen(clientFd, buf, strlen(buf));
    }
    Rio_writen(clientFd, "\r\n", 2);

    // 如果当前请求不可被缓存(缺失内容大小/缺失内容类型/内容过大)
    if(contentSize == -1 || contentSize > MAX_OBJECT_SIZE || content_type == NULL)
    {
        // 直接发送
        while ((size = Rio_readnb(&serverRio, buf, MAXLINE)))
        {
            printf("%s", buf);
            Rio_writen(clientFd, buf, size);
        }
    }
    else
    {
        /* 获取客户端数据的body */
        // 多加一字节验证正确性
        char* data = malloc(contentSize + 1);
        size = Rio_readnb(&serverRio, data, contentSize + 1);
        printf("%s", data);
        if(size != contentSize)
            printf(PREFIX_PRINT"WARNING: Content-length is different from actual data lenth.\n");
        int actualStoreSize = ((size > contentSize) ? contentSize : size);

        /* 将数据添加进Cache中 */
        // 设置线程锁
        pthread_mutex_lock(&thread_mutex); 
        writeCache(resAddr, data, actualStoreSize, content_type->value);
        pthread_mutex_unlock(&thread_mutex); 
        // 将数据发送给客户端
        Rio_writen(clientFd, data, actualStoreSize);
        // 注意释放内存
        free(data);
    }
}

/*
    writeToServer - 向服务器发送数据
*/
void writeToServer(int clientFd, int serverFd, 
            http_target* targetServer, http_headers* headers, int header_num)
{
    printf(PREFIX_PRINT"write to fd %d:\n", serverFd);
    char buf[3 * MAXLINE + 5];
    // 写入第一行
    snprintf(buf, MAXLINE, "%s %s %s\r\n", 
        targetServer->requestType, targetServer->resource, targetServer->Protocol);
    printf("%s", buf);
    Rio_writen(serverFd, buf, strlen(buf));
    // 写入http header
    for(int i = 0; i < header_num; i++)
    {
        snprintf(buf, 3 * MAXLINE + 5, "%s: %s\r\n", headers[i].name, headers[i].value);
        printf("%s", buf);
        Rio_writen(serverFd, buf, strlen(buf));
    }
    Rio_writen(serverFd, "\r\n", 2);
    printf("\r\n");
    /*
    @note 一个奇怪的地方：无法检测客户端的EOF，再次读取会直接阻塞; 但却可以检测到服务端的EOF
        初步判断原因是服务端数据一经发送，服务端断开与proxy的连接，所以就有了EOF
    // transmitDataS2C(serverFd, clientFd);
    */
}

/*
 * doit - handle one HTTP request/response transaction
 * pthread多线程使用函数，始终传入clientFd与返回NULL
 */
void* doit(void* thread_arg) 
{
    // 设置客户端的文件描述符
    int clientFd = (int)(long)thread_arg;
    char buf[MAXLINE], uri[MAXLINE], cache_content_type[MAXLINE];
    http_headers headers[MAXHEADERS];  
    http_target targetServer;
    rio_t clientRio;
    int serverFd;
    char* cacheData = NULL;
    int cacheSize;

    /* 开始对http header做一些处理 */
    Rio_readinitb(&clientRio, clientFd);
    // 读取第一行的get请求，例如    “GET /index.html HTTP/1.0\r\n”
    if (!Rio_readlineb(&clientRio, buf, MAXLINE))  
        return NULL;
    printf(PREFIX_PRINT"%s", buf);
    sscanf(buf, "%s %s %s", targetServer.requestType, uri, targetServer.Protocol);  
    
    // 设置线程锁
    pthread_mutex_lock(&thread_mutex); 
    // 判断该资源是否存在cache中
    const cache_line* tmpCache = readCache(uri);
    // 如果资源存在
    if(tmpCache)
    {
        // 趁着有锁，将数据复制到另一块内存中
        cacheSize = tmpCache->size;
        cacheData = malloc(cacheSize);
        memcpy(cacheData, tmpCache->data, cacheSize);
        strncpy(cache_content_type, tmpCache->content_type, MAXLINE);
    }
    // 解锁
    pthread_mutex_unlock(&thread_mutex);
    // 如果数据存在于cache
    if(cacheData)
    {
        char tmp[MAXLINE];
        // 发送http头部
        snprintf(buf, MAXLINE, "HTTP/1.0 200 OK\r\n");
        strncat(buf, "Server: Tiny Proxy Server\r\n", MAXLINE);
        snprintf(tmp, MAXLINE, "Content-type: %s\r\n", cache_content_type);
        strncat(buf, tmp, MAXLINE);
        snprintf(tmp, MAXLINE, "Content-length: %d\r\n", cacheSize);
        strncat(buf, tmp, MAXLINE);
        strncat(buf, "\r\n", MAXLINE);
        Rio_writen(clientFd, buf, strlen(buf));
        // 发送主体内容给客户端
        Rio_writen(clientFd, cacheData, cacheSize);
        printf(PREFIX_PRINT"write to fd %d\n%s%s:\n", clientFd, buf, cacheData);
        free(cacheData);
    }
    else
    {
        // 获取其余的header
        int headers_num = read_requesthdrs(&clientRio, headers);   
        // 获取目标ip，端口、以及所申请的资源
        parse_uri(uri, &targetServer);
        // 添加新的header
        add_header(headers, &headers_num, "User-Agent", user_agent_hdr);
        add_header(headers, &headers_num, "Host", targetServer.address);
        add_header(headers, &headers_num, "Connection", "close");
        add_header(headers, &headers_num, "Proxy-Connection", "close");
        // 连接远程服务器
        serverFd = Open_clientfd(targetServer.address, targetServer.port);
        printf(PREFIX_PRINT"Connect to %s:%s.(serverFd: %d)\n",
            targetServer.address, targetServer.port, serverFd);
        // 发送数据至远程服务器
        writeToServer(clientFd, serverFd, &targetServer, headers, headers_num);
        // 将远程服务器传来的数据转发给客户端
        transmitDataS2C(clientFd, serverFd, uri);
    }
    // 关闭服务端
    close(serverFd);
    // 关闭客户端
    Close(clientFd);
    printf(PREFIX_PRINT"client closed\n\n");
    return NULL;
}

int main(int argc, char* argv[])
{
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_storage clientaddr;
    pthread_attr_t thread_attr;
    socklen_t clientlen;
    pthread_t thread;
    
    int connfd;
    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    // 设置线程状态为分离状态，防止大量线程结束后没有被回收，所导致的内存泄露
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    // 初始化线程锁
    pthread_mutex_init(&thread_mutex, NULL);
    // 初始化缓存
    initCache();
    int listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf(PREFIX_PRINT"Accepted connection from (%s, %s), clientFd: %d\n", 
                hostname, port, connfd);
        pthread_create(&thread, &thread_attr, doit, (void*)(long)connfd);
    }
    return 0;
}