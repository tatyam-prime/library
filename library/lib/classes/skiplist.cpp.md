---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../assets/css/copy-button.css" />


# :heavy_check_mark: lib/classes/skiplist.cpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#1a2816715ae26fbd9c4a8d3f916105a3">lib/classes</a>
* <a href="{{ site.github.repository_url }}/blob/master/lib/classes/skiplist.cpp">View this file on GitHub</a>
    - Last commit date: 2020-04-09 14:56:42+09:00




## Verified with

* :heavy_check_mark: <a href="../../../verify/verify/skiplist_composite.test.cpp.html">verify/skiplist_composite.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
template <typename T>
struct SkipList{

    static uint32_t rnd(){
        static uint32_t x = 123456789, y = 362436069, z = 521288629, w = time(0);
        uint32_t t = x ^ (x << 11);
        x = y, y = z, z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w;
    }

    struct Node{
        int height;
        T val;
        vector<Node*> next, prev;
        vector<T> sum;
        vector<int> size;
        Node(T val, int height) : val(val), height(height), next(height, nullptr), prev(height, nullptr), sum(height, val), size(height, 1){}
    };

    using NodePtr = Node*;
    int max_height;
    NodePtr front, back;
    function<T(T, T)> f;
    T op;

    SkipList(function<T(T, T)> f = [](auto x, auto y){return x;}, T op = T()) : max_height(0), f(f), op(op){
        front = new Node(op, 32);
        back = new Node(op, 32);
        front->next[0] = back;
        back->prev[0] = front;
    }

    int size(){
        return front->size[max_height] - 1;
    }

    void insert_next(NodePtr pre, T key){
        uint32_t r = max(rnd(), uint32_t(1));
        int height = __builtin_ffs(r);
        while(max_height < height){
            ++max_height;
            front->size[max_height] = front->size[max_height - 1];
            front->next[max_height] = back;
            back->prev[max_height] = front;
            front->sum[max_height] = front->sum[max_height - 1];
        }
        NodePtr node = new Node(key, height);

        int pre_size = 1;
        T pre_sum = pre->val;
        T nex_sum = key;
        NodePtr nex = pre->next[0];
        for(int i = 0; i <= max_height; ++i){
            ++pre->size[i];
            if(i < height){
                pre->next[i] = node;
                node->next[i] = nex;
                nex->prev[i] = node;
                node->prev[i] = pre;
                int range_size = pre->size[i];
                pre->size[i] = pre_size;
                pre->sum[i] = pre_sum;
                node->size[i] = range_size - pre_size;
                node->sum[i] = nex_sum;
            }
            else{
                pre->sum[i] = f(pre_sum, nex_sum);
            }
            for(; pre->height == i + 1 && pre->prev[i] != nullptr; pre = pre->prev[i]){
                pre_sum = f(pre->prev[i]->sum[i], pre_sum);
                pre_size += pre->prev[i]->size[i];
            }
            for(; nex->height == i + 1  && nex->next[i] != nullptr; nex = nex->next[i]){
                nex_sum = f(nex_sum, nex->sum[i]);
            }
        }
    }

    // idx番目(idx=0なら先頭)に挿入する
    void insert_index(int idx, T key){
        NodePtr pre = access(idx - 1);
        insert_next(pre, key);
    }

    void insert_key(T key){
        NodePtr pre = lower_bound(key)->prev[0];
        insert_next(pre, key);
    }

    void erase(NodePtr target){
        // メモリ解放はしない(してもいいけど)
        int height = target->height;
        NodePtr pre = target->prev[0];
        NodePtr nex = target->next[0];
        T sum = pre->val;
        for(int i = 0; i <= max_height; ++i){
            pre->sum[i] = sum;
            --pre->size[i];
            if(i < height){
                pre->next[i] = nex;
                nex->prev[i] = pre;
                pre->size[i] += target->size[i];
            }
            for(; pre->height == i + 1 && pre->prev[i] != nullptr; pre = pre->prev[i])
                sum = f(pre->prev[i]->sum[i], sum);
            for(; nex->height == i + 1 && nex->next[i] != nullptr; nex = nex->next[i])
                sum = f(sum, nex->sum[i]);
        }
    }

    void erase_index(int idx){
        NodePtr target = access(idx);
        erase(target);
    }
    void erase_key(T key){
        NodePtr target = lower_bound(key);
        if(target == back || target->val != key)
            return;
        erase(target);
    }

    NodePtr lower_bound(T key){
        NodePtr pre = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < pre->next.size() && pre->next[i] != back && pre->next[i]->val < key; pre = pre->next[i]);
        return pre->next[0];
    }

    NodePtr upper_bound(T key){
        NodePtr pre = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < pre->next.size() && pre->next[i] != back && pre->next[i]->val <= key; pre = pre->next[i]);
        return pre->next[0];
    }

    bool contains(T key){
        NodePtr ptr = lower_bound(key);
        return ptr != back && ptr->key == key;
    }

    // 0-indexedでアクセスする
    NodePtr access(int idx){
        ++idx;
        assert(0 <= idx && idx <= size());
        NodePtr ptr = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < ptr->next.size() && ptr->next[i] != back && ptr->size[i] <= idx; idx -= ptr->size[i], ptr = ptr->next[i]);
        return ptr;
    }

    T get(int l, int r){
        NodePtr ptr = access(l);
        T sum = op;
        int diff = r - l;
        int height_bound = 32;
        for(; ptr->size[ptr->height - 1] <= diff; ptr = ptr->next[ptr->height - 1]){
            diff -= ptr->size[ptr->height - 1];
            sum = f(sum, ptr->sum[ptr->height - 1]);
        }
        for(int i = ptr->height - 2; diff; --i)
            for(; ptr->size[i] <= diff; ptr = ptr->next[i]){
                diff -= ptr->size[i];
                sum = f(sum, ptr->sum[i]);
            }
        return sum;
    }

    void print(){
        for(NodePtr node = front; node != nullptr; node = node->next[0]){
            if(node == front || node == back)
                printf("  null: ");
            else
                printf("%6lld: ", node->val);
            for(int i = 0; i < node->height; ++i)
                printf("%2d ", node->sum[i]);
                // cout << node->size[i] << " ";
            cout << endl;
        }
        cout << endl;
    }
};

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "lib/classes/skiplist.cpp"
template <typename T>
struct SkipList{

    static uint32_t rnd(){
        static uint32_t x = 123456789, y = 362436069, z = 521288629, w = time(0);
        uint32_t t = x ^ (x << 11);
        x = y, y = z, z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w;
    }

    struct Node{
        int height;
        T val;
        vector<Node*> next, prev;
        vector<T> sum;
        vector<int> size;
        Node(T val, int height) : val(val), height(height), next(height, nullptr), prev(height, nullptr), sum(height, val), size(height, 1){}
    };

    using NodePtr = Node*;
    int max_height;
    NodePtr front, back;
    function<T(T, T)> f;
    T op;

    SkipList(function<T(T, T)> f = [](auto x, auto y){return x;}, T op = T()) : max_height(0), f(f), op(op){
        front = new Node(op, 32);
        back = new Node(op, 32);
        front->next[0] = back;
        back->prev[0] = front;
    }

    int size(){
        return front->size[max_height] - 1;
    }

    void insert_next(NodePtr pre, T key){
        uint32_t r = max(rnd(), uint32_t(1));
        int height = __builtin_ffs(r);
        while(max_height < height){
            ++max_height;
            front->size[max_height] = front->size[max_height - 1];
            front->next[max_height] = back;
            back->prev[max_height] = front;
            front->sum[max_height] = front->sum[max_height - 1];
        }
        NodePtr node = new Node(key, height);

        int pre_size = 1;
        T pre_sum = pre->val;
        T nex_sum = key;
        NodePtr nex = pre->next[0];
        for(int i = 0; i <= max_height; ++i){
            ++pre->size[i];
            if(i < height){
                pre->next[i] = node;
                node->next[i] = nex;
                nex->prev[i] = node;
                node->prev[i] = pre;
                int range_size = pre->size[i];
                pre->size[i] = pre_size;
                pre->sum[i] = pre_sum;
                node->size[i] = range_size - pre_size;
                node->sum[i] = nex_sum;
            }
            else{
                pre->sum[i] = f(pre_sum, nex_sum);
            }
            for(; pre->height == i + 1 && pre->prev[i] != nullptr; pre = pre->prev[i]){
                pre_sum = f(pre->prev[i]->sum[i], pre_sum);
                pre_size += pre->prev[i]->size[i];
            }
            for(; nex->height == i + 1  && nex->next[i] != nullptr; nex = nex->next[i]){
                nex_sum = f(nex_sum, nex->sum[i]);
            }
        }
    }

    // idx番目(idx=0なら先頭)に挿入する
    void insert_index(int idx, T key){
        NodePtr pre = access(idx - 1);
        insert_next(pre, key);
    }

    void insert_key(T key){
        NodePtr pre = lower_bound(key)->prev[0];
        insert_next(pre, key);
    }

    void erase(NodePtr target){
        // メモリ解放はしない(してもいいけど)
        int height = target->height;
        NodePtr pre = target->prev[0];
        NodePtr nex = target->next[0];
        T sum = pre->val;
        for(int i = 0; i <= max_height; ++i){
            pre->sum[i] = sum;
            --pre->size[i];
            if(i < height){
                pre->next[i] = nex;
                nex->prev[i] = pre;
                pre->size[i] += target->size[i];
            }
            for(; pre->height == i + 1 && pre->prev[i] != nullptr; pre = pre->prev[i])
                sum = f(pre->prev[i]->sum[i], sum);
            for(; nex->height == i + 1 && nex->next[i] != nullptr; nex = nex->next[i])
                sum = f(sum, nex->sum[i]);
        }
    }

    void erase_index(int idx){
        NodePtr target = access(idx);
        erase(target);
    }
    void erase_key(T key){
        NodePtr target = lower_bound(key);
        if(target == back || target->val != key)
            return;
        erase(target);
    }

    NodePtr lower_bound(T key){
        NodePtr pre = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < pre->next.size() && pre->next[i] != back && pre->next[i]->val < key; pre = pre->next[i]);
        return pre->next[0];
    }

    NodePtr upper_bound(T key){
        NodePtr pre = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < pre->next.size() && pre->next[i] != back && pre->next[i]->val <= key; pre = pre->next[i]);
        return pre->next[0];
    }

    bool contains(T key){
        NodePtr ptr = lower_bound(key);
        return ptr != back && ptr->key == key;
    }

    // 0-indexedでアクセスする
    NodePtr access(int idx){
        ++idx;
        assert(0 <= idx && idx <= size());
        NodePtr ptr = front;
        for(int i = max_height; i >= 0; --i)
            for(; i < ptr->next.size() && ptr->next[i] != back && ptr->size[i] <= idx; idx -= ptr->size[i], ptr = ptr->next[i]);
        return ptr;
    }

    T get(int l, int r){
        NodePtr ptr = access(l);
        T sum = op;
        int diff = r - l;
        int height_bound = 32;
        for(; ptr->size[ptr->height - 1] <= diff; ptr = ptr->next[ptr->height - 1]){
            diff -= ptr->size[ptr->height - 1];
            sum = f(sum, ptr->sum[ptr->height - 1]);
        }
        for(int i = ptr->height - 2; diff; --i)
            for(; ptr->size[i] <= diff; ptr = ptr->next[i]){
                diff -= ptr->size[i];
                sum = f(sum, ptr->sum[i]);
            }
        return sum;
    }

    void print(){
        for(NodePtr node = front; node != nullptr; node = node->next[0]){
            if(node == front || node == back)
                printf("  null: ");
            else
                printf("%6lld: ", node->val);
            for(int i = 0; i < node->height; ++i)
                printf("%2d ", node->sum[i]);
                // cout << node->size[i] << " ";
            cout << endl;
        }
        cout << endl;
    }
};

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
