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
//�������ģ���ڴ�Ĵ���
int times = 500;
const int seq_len = 32;
int block[4]={0,0,0,0};
int visit_seq [seq_len];

page *first,*node, *head, *end;;
struct page all[32];

//����һ������ģ���ڴ�
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

    //����ҳ��
    first = generate(10);
    //���ɷ�������

    printf("���̷���ҳ��˳��Ϊ: ");
    for(int & i : visit_seq)
    {
        i = rand()%10;
        printf("%d ", i);
    }
}
void OPT()
{
    //δ���д���
    int miss = 0;
    //������һ��ҳ������Զ�Ľ���������.
    int check[seq_len];
    int num;
    for(int i = 0 ; i < seq_len; i++)
    {
        //���ҳ����û�з���������Ҫ���ʵ�ҳ
        if(block[0]!= visit_seq[i] && block[1]!= visit_seq[i] && block[2]!= visit_seq[i] && block[3]!= visit_seq[i] )
        {
            //ȱҳ������һ
            miss++;
            //Ѱ����Զ�����ʵ�ҳ��.���ȱ���Ŀǰδ���ʵ�����.
            for(int j = i ; j < seq_len; j++)
            {
                //֮ǰ����һ������,�����洢��ǰ�������������ҳ�ķ��ʴ���.
                check[visit_seq[j]]++;
                //�����ʱһ��ҳ��Ϊ�ı����ʴ���Ϊ0,�������ı����ʴ�������Ϊ0˵����������Զ�����ʵ�ҳ��.
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
            //�ҵ�����block��ʱ�����ҳ���index
            int max = Max(all[block[0]], Max(all[block[1]], Max(all[block[2]], all[block[3]]))).index;
            //�ҵ���index֮���block�е�ÿһ��Ԫ�ؽ��м��,����������滻���·��ʵ�ҳ,����block������Ԫ�ص�ʱ������.
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
                //����all�����ж�Ӧҳ��block�е�ʱ��.
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