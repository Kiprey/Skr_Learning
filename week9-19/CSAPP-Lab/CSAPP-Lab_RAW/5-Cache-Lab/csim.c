#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <getopt.h>

int b = 0, s = 0, B = 0, S = 0;
int E = 0,  verbosity = 0;
// 注意， LRU从1开始（0为空行）
long long int lru_counter = 1;
char* trace_file = NULL;
int hit_count = 0, miss_count = 0, eviction_count = 0;

typedef long long unsigned mem_addr_t;
struct cache_line_t{
    mem_addr_t tag;
    int valid;
    unsigned int lru; 
};
typedef struct cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

cache_t cache;
mem_addr_t set_index_mask = 0;

void printUsage(char* name)
{
  printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", name);
  puts("Options:");
  puts("  -h         Print this help message.");
  puts("  -v         Optional verbose flag.");
  puts("  -s <num>   Number of set index bits.");
  puts("  -E <num>   Number of lines per set.");
  puts("  -b <num>   Number of block offset bits.");
  puts("  -t <file>  Trace file.");
  puts("\nExamples:");
  printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", name);
  printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", name);
  exit(0);
}

void initCache()
{
  // 给cache分配内存空间初始化
  cache = (cache_t)malloc(sizeof(cache_set_t) * S);
  for (int i = 0; i < S; ++i )
  {
    cache[i] = (cache_set_t)malloc(sizeof(struct cache_line_t) * E);
    for (int j = 0; j < E; ++j )
    {
      cache[i][j].valid = 0;
      cache[i][j].tag = 0;
      cache[i][j].lru = 0;
    }
  }

  set_index_mask  = pow(2, s) - 1;
}

void accessData(mem_addr_t addr)
{
  int eviction_line;
  // 注意是无符号整数
  unsigned int eviction_lru = -1;
  eviction_line = 0;
  mem_addr_t tag = addr >> (s + b);
  cache_set_t cache_set = cache[(addr >> b) & set_index_mask];

  // 所需数据的cache_line编号
  int i;
  for (i = 0; ; ++i )
  {
    // 如果把所有的cache_line全遍历完了还找不到所需的数据
    if ( i >= E )
    {
      // 数据未命中
      ++miss_count;
      if ( verbosity )
        printf("miss ");
      // 在一组cache_line中查找将被删除的cache_line
      for (int ia = 0; ia < E; ++ia )
      {
        if ( cache_set[ia].lru < eviction_lru )
        {
          eviction_line = ia;
          eviction_lru = cache_set[ia].lru;
        }
     }
      // 如果当前这个要被删除的cache_line是valid
      // 即，这个要被替换数据的cache_line是一条之前读入的数据而不是空行
      if ( cache_set[eviction_line].valid )
      {
        // 删除数+1
        ++eviction_count;
        if ( verbosity )
          printf("eviction ");
      }
      // 模拟读入并覆盖数据到这个刚刚被删除（或本来是空行）的cache_line里
      cache_set[eviction_line].valid = 1;
      cache_set[eviction_line].tag = tag;
      cache_set[eviction_line].lru = lru_counter++;
      return;
    }
    // 查找cache中的数据
    if ( cache_set[i].tag == tag && cache_set[i].valid )
      break;
  }
  // 如果找到数据了，自然就hit_count++
  ++hit_count;
  if ( verbosity )
    printf("hit ");
  cache_set[i].lru = lru_counter++;
}

void replayTrace(char *trace_fn)
{
  FILE * trace_fp = fopen(trace_fn, "r");
  if ( !trace_fp )
  {
    int* err_num = __errno_location();
    char* err_str = strerror(*err_num);
    fprintf(stderr, "%s: %s\n", trace_fn, err_str);
    exit(1);
  }

  char buf[1000];
  while ( fgets(buf, 1000, trace_fp) )
  {
    unsigned int len = 0;
    mem_addr_t addr = 0;
    if ( buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M' )
    {
      sscanf(&buf[3], "%llx,%u", &addr, &len);
      if ( verbosity )
        printf("%c %llx,%u ", buf[1], addr, len);
      // 读取/写入数据
      // 写入数据同样需要判断数据是否存在与cache。如果数据不在，同样要将其读回cache
      accessData(addr);
      // 如果当前指令是修改指令，则上一条accessData读取数据，下一条的accessData写入数据
      if ( buf[1] == 'M' )
        accessData(addr);
      if ( verbosity )
        putchar('\n');
    }
  }
  fclose(trace_fp);
}

void freeCache()
{
    for (int i = 0; i < S; ++i )
        free(cache[i]);
    free(cache);
}

int main(int argc, char* argv[])
{
    char c;
    while((c = getopt(argc, argv, "s:E:b:t:vh")) != -1)
    {
        switch ( c )
        {
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 'h':
            printUsage(argv[0]);
            return 0;
        case 's':
            s = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        default:
            printUsage(argv[0]);
            return 0;
        }
    }
    if ( !s || !E || !b || !trace_file )
    {
        printf("%s: Missing required command line argument\n", *argv);
        printUsage(argv[0]);
    }
    S = pow(2, s);
    B = pow(2, b);
    initCache();
    replayTrace(trace_file);
    freeCache();
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}

