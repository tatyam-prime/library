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


# :heavy_check_mark: lib/classes/hashmap.cpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#1a2816715ae26fbd9c4a8d3f916105a3">lib/classes</a>
* <a href="{{ site.github.repository_url }}/blob/master/lib/classes/hashmap.cpp">View this file on GitHub</a>
    - Last commit date: 2020-04-11 13:18:14+09:00




## Verified with

* :heavy_check_mark: <a href="../../../verify/verify/hashmap_dict.test.cpp.html">verify/hashmap_dict.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/verify/hashmap_itp.test.cpp.html">verify/hashmap_itp.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/verify/xfastttrie_itp.test.cpp.html">verify/xfastttrie_itp.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/verify/yfastttrie2_itp.test.cpp.html">verify/yfastttrie2_itp.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/verify/yfastttrie_itp.test.cpp.html">verify/yfastttrie_itp.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
template <typename T, typename U, T del = numeric_limits<T>::max(), T null = numeric_limits<T>::max() - 1>
struct HashMap{
    static constexpr __int128_t z = 0xf332ac987401cba5;
    uint64_t n, q, d;

    vector<pair<T, U>> v;

    HashMap() : n(0), q(0), d(1),  v(2, make_pair(null, U())){
    }

    inline uint64_t hash(T key){return uint64_t((z * __int128_t(key)) >> (64 - d)) & ((1LL << d) - 1);}

    pair<U, bool> find(T x){
        for(uint64_t i = hash(x); v[i].first != null; i = (i + 1) & ((1 << d) - 1))
            if(v[i].first == x)
                return make_pair(v[i].second, true);
        return make_pair(U(), false);
    }

    bool add(T x, U val){
        if(find(x).second)
            return false;
        if(((q + 1) << 1) > (1 << d) || (1 << d) < 3 * n)
            resize();
        uint64_t i = hash(x);
        for(; v[i].first != null && v[i].first != del; i = (i + 1) & ((1 << d) - 1));
        q += (v[i].first == null);
        ++n;
        v[i] = make_pair(x, val);
        return true;
    }

    bool erase(T x){
        uint64_t i = hash(x);
        for(; v[i].first != null && v[i].first != x; i = (i + 1) & ((1 << d) - 1));
        if(v[i].first == null)
            return false;
        --n;
        v[i] = make_pair(del, U());
        return true;
    }

    void resize(){
        ++d;
        vector<pair<T, U>> old_table;
        q = n;
        swap(old_table, v);
        v.assign(1 << d, make_pair(null, U()));
        n = 0;
        for(int i = 0; i < old_table.size(); ++i)
            if(old_table[i].first != null && old_table[i].first != del)
                add(old_table[i].first, old_table[i].second);
    }
};


```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "lib/classes/hashmap.cpp"
template <typename T, typename U, T del = numeric_limits<T>::max(), T null = numeric_limits<T>::max() - 1>
struct HashMap{
    static constexpr __int128_t z = 0xf332ac987401cba5;
    uint64_t n, q, d;

    vector<pair<T, U>> v;

    HashMap() : n(0), q(0), d(1),  v(2, make_pair(null, U())){
    }

    inline uint64_t hash(T key){return uint64_t((z * __int128_t(key)) >> (64 - d)) & ((1LL << d) - 1);}

    pair<U, bool> find(T x){
        for(uint64_t i = hash(x); v[i].first != null; i = (i + 1) & ((1 << d) - 1))
            if(v[i].first == x)
                return make_pair(v[i].second, true);
        return make_pair(U(), false);
    }

    bool add(T x, U val){
        if(find(x).second)
            return false;
        if(((q + 1) << 1) > (1 << d) || (1 << d) < 3 * n)
            resize();
        uint64_t i = hash(x);
        for(; v[i].first != null && v[i].first != del; i = (i + 1) & ((1 << d) - 1));
        q += (v[i].first == null);
        ++n;
        v[i] = make_pair(x, val);
        return true;
    }

    bool erase(T x){
        uint64_t i = hash(x);
        for(; v[i].first != null && v[i].first != x; i = (i + 1) & ((1 << d) - 1));
        if(v[i].first == null)
            return false;
        --n;
        v[i] = make_pair(del, U());
        return true;
    }

    void resize(){
        ++d;
        vector<pair<T, U>> old_table;
        q = n;
        swap(old_table, v);
        v.assign(1 << d, make_pair(null, U()));
        n = 0;
        for(int i = 0; i < old_table.size(); ++i)
            if(old_table[i].first != null && old_table[i].first != del)
                add(old_table[i].first, old_table[i].second);
    }
};


```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
