//
// Created by Godlowd on 2020/4/29.
//
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
typedef struct page{
    int index;
    bool in_block;
    int times_be_call;
    int times_stay;
    page* next;
} page;
page Max(page a, page b);
//定义访问模拟内存的次数
int times = 500;
const int seq_len = 32;
int block[4]={0,0,0,0};
int visit_seq [seq_len];

page *first,*node, *head, *end;;
struct page all[32];

//生成一个链表模拟内存
page* generate(int n)
{
    head = (page*)malloc(sizeof(page));
    head->index=1;
    head->in_block = false;
    head->times_stay = 0;
    end = head;
    all[0] = *head;
  for(int i = 0 ; i < n ; i ++)
  {
      node = (page*)malloc(sizeof(page));
      node->times_stay = 0;
      node->in_block = false;
      node->index = i+2;
      end->next = node;
      end = node;
      all[i+1] = *node;
  }
  end->next = NULL;
  return head;
}
int main(void)
{

    //生成页框
    first = generate(10);
    //生成访问序列

    printf("进程访问页框顺序为: ");
    for(int & i : visit_seq)
    {
        i = rand()%10;
        printf("%d ", i);
    }
}
void OPT()
{
    //未命中次数
    int miss = 0;
    //计算哪一个页框在最远的将来被访问.
    int check[seq_len];
    int num;
    for(int i = 0 ; i < seq_len; i++)
    {
        //如果页框中没有访问序列中要访问的页
        if(block[0]!= visit_seq[i] && block[1]!= visit_seq[i] && block[2]!= visit_seq[i] && block[3]!= visit_seq[i] )
        {
            //缺页次数加一
            miss++;
            //寻找最远被访问的页框.首先遍历目前未访问的序列.
            for(int j = i ; j < seq_len; j++)
            {
                //之前定义一个数组,用来存储从前往后遍历遇到的页的访问次数.
                check[visit_seq[j]]++;
                //如果此时一个页框为的被访问次数为0,而其他的被访问次数都不为0说明他就是最远被访问的页框.
                if(check[block[0]]==0 && check[block[1]]!=0 && check[block[2]]!=0 && check[block[3]]!=0)
                {
                    block[0] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]==0 && check[block[2]]!=0 && check[block[3]]!=0)
                {
                    block[1] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]!=0 && check[block[2]]==0 && check[block[3]]!=0)
                {
                    block[2] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]!=0 && check[block[2]]!=0 && check[block[3]]==0)
                {
                    block[3] = visit_seq[i];
                    break;
                }
            }


        }
    }
}

void FIFO()
{
    int miss;
    for(int i = 0; i < seq_len ; i++)
    {
        if(block[0] != visit_seq[i] && block[1] != visit_seq[i] && block[2] != visit_seq[i] && block[3] != visit_seq[i])
        {
            miss++;
            block[0] = block[1];
            block[1] = block[2];
            block[2] = block[3];
            block[3] = visit_seq[i];
        }
        else continue;
    }
}

void LRU()
{
    int miss;
    page* node = head;
    for(int i = 0; i < seq_len ; i++)
    {
        if(block[0] != visit_seq[i] && block[1] != visit_seq[i] && block[2] != visit_seq[i] && block[3] != visit_seq[i])
        {
            miss++;
            //找到呆在block中时间最长的页框的index
            int max = Max(all[block[0]], Max(all[block[1]], Max(all[block[2]], all[block[3]]))).index;
            //找到了index之后对block中的每一个元素进行检查,如果是则将其替换成新访问的页,并且block中其他元素的时间自增.
            if(block[0] == max)
            {
                block[0] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[1]].times_stay++;
                all[block[2]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[1] == max)
            {
                block[1] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[2]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[2] == max)
            {
                block[1] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[1]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[3] == max)
            {
                block[1] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[1]].times_stay++;
                all[block[2]].times_stay++;
            }

        }
        else
        {
                //更新all数组中对应页在block中的时间.
                if(block[0] == visit_seq[i])
                {
                    all[block[0]].times_stay=1;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay++;
                }
                else if  (block[1] == visit_seq[i])
                {
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay=1;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay++;
                }
                else if  (block[2] == visit_seq[i])
                {
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay=1;
                    all[block[3]].times_stay++;
                }
                else if  (block[3] == visit_seq[i])
                {
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay=1;
                }
                }
            }
}
page Max(page a, page b)
{
    if (a.times_stay> b.times_stay)
        return a;
    else return b;
}