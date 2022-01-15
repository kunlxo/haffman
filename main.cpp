#include <bits/stdc++.h>

using namespace std;
using ll = long long;

FILE* f1, * f2, * f3;

struct Forest {
    ll freq;
    int root, symbol;
};

struct Tree {
    int left, right, parent, symbol;
};

struct Code {
    int l;
    int coding[256];
};

Forest forest[256];
Tree tree[520];
Code code[256];

void min_min(Forest forest[], int _size, int& pos1, int& pos2)
{
    if (forest[0].freq < forest[1].freq)
    {
        pos1 = 0;
        pos2 = 1;
    }
    else
    {
        pos1 = 1;
        pos2 = 0;
    }
    for (int i = 2; i < _size; i++)
    {
        if (forest[pos1].freq > forest[i].freq)
        {
            pos2 = pos1;
            pos1 = i;
        }
        else if (forest[pos2].freq > forest[i].freq)
        {
            pos2 = i;
        }
    }
}

int32_t main(int argc, char *argv[])
{
    if (!strcmp("encode", argv[1]))
    {
        f1 = fopen(argv[3], "rb");
        f2 = fopen(argv[2], "wb");
        ll freq[256];
        memset(freq, 0, sizeof(freq));
        unsigned char ch;
        while (fscanf(f1, "%c", &ch) != -1)
        {
            freq[ch]++;
        }
        int temp = 0;
        for (int i = 0; i < 256; i++)
        {
            if (freq[i] != 0)
            {
                forest[temp].freq = freq[i];
                forest[temp].root = temp;
                forest[temp].symbol = i;
                temp++;
            }
        }
        for (int i = 0; i < temp; i++)
        {
            tree[i].left = -1;
            tree[i].right = -1;
            tree[i].parent = -1;
            tree[i].symbol = forest[i].symbol;
        }
        int size_forest = temp;
        int size_tree = temp;
        int p1 = -1;
        int p2 = -1;
        int k_ls = size_tree;
        while (size_forest > 1)
        {
            min_min(forest, size_forest, p1, p2);
            tree[size_tree].left = forest[p1].root;
            tree[size_tree].right = forest[p2].root;
            tree[size_tree].parent = -1;
            tree[forest[p1].root].parent = size_tree;
            tree[forest[p2].root].parent = size_tree;
            forest[p1].freq += forest[p2].freq;
            forest[p1].root = size_tree;
            forest[p2] = forest[size_forest - 1];
            size_forest--;
            size_tree++;
        }
        int cur_par, cur_el, sym;
        for (int i = 0; i < k_ls; i++)
        {
            cur_el = i;
            cur_par = tree[i].parent;
            sym = tree[i].symbol;
            code[sym].l = 0;
            while (cur_par != -1)
            {
                if (tree[cur_par].left == cur_el)
                {
                    code[sym].coding[code[sym].l] = 0;
                }
                else
                {
                    code[sym].coding[code[sym].l] = 1;
                }
                cur_el = cur_par;
                cur_par = tree[cur_par].parent;
                code[sym].l++;
            }
        }
        fseek(f1, 0, SEEK_SET);
        fprintf(f2, " \n%d\n", size_tree);
        for(int i = 0; i < size_tree; i++)
        {
            fprintf(f2, "%d %d %d %d\n", tree[i].left, tree[i].right, tree[i].parent,  tree[i].symbol);
        }
        unsigned char byte = 0;
        int leng = 0;
        int lenoutput = 0;
        while (fscanf(f1, "%c", &ch) != -1)
        {
            for (int i = code[ch].l - 1; i >= 0; i--)
            {
                byte <<= 1;
                byte += code[ch].coding[i];
                leng += 1;
                if (leng == 8)
                {
                    fprintf(f2, "%c", byte);
                    lenoutput++;
                    byte = 0;
                    leng = 0;
                }
            }
        }
        if (byte > 0)
        {
            for (int i = 0; i < 8 - leng; i++)
            {
                byte <<= 1;
            }
            fprintf(f2, "%c", byte);
            lenoutput++;
        }
        fseek(f2, 0, SEEK_SET);
        fprintf(f2, "%d ", leng);
        fclose(f1);
        fclose(f2);
    }
    else
    {
        f1 = fopen(argv[2], "rb");
        f3 = fopen(argv[3], "wb");
        fseek(f1, 0, SEEK_END);
        int lenoutput = ftell(f1);
        fseek(f1, 0, SEEK_SET);
        int size_tree, cur_el, k_ls, leng;
        k_ls = 0;
        fscanf(f1, "%d",&leng);
        fscanf(f1, "%d",&size_tree);
        for (int i = 0; i < size_tree; i++)
        {
            fscanf(f1, "%d%d%d%d ", &tree[i].left, &tree[i].right, &tree[i].parent, &tree[i].symbol);
            if (tree[i].left == -1)
            {
                k_ls++;
            }
        }
        unsigned char ch;
        cur_el = size_tree - 1;
        while (fscanf(f1, "%c", &ch) != -1)
        {
            for (int i = 0; i < 8; i++)
            {
                if (ftell(f1) == lenoutput && i > leng)
                {
                    break;
                }
                if (cur_el < k_ls)
                {
                    fprintf(f3, "%c", tree[cur_el].symbol);
                    cur_el = size_tree - 1;
                }
                if ((int)ch / 128 == 0)
                {
                    cur_el = tree[cur_el].left;
                }
                else
                {
                    cur_el = tree[cur_el].right;
                }
                ch <<= 1;
            }
        }
        fclose(f1);
        fclose(f3);
    }
}
