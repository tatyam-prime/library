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


# :heavy_check_mark: lib/classes/unionfind.cpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#1a2816715ae26fbd9c4a8d3f916105a3">lib/classes</a>
* <a href="{{ site.github.repository_url }}/blob/master/lib/classes/unionfind.cpp">View this file on GitHub</a>
    - Last commit date: 2019-11-30 20:08:52+09:00




## Verified with

* :heavy_check_mark: <a href="../../../verify/verify/unionfind.test.cpp.html">verify/unionfind.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
struct UnionFind{
    vector<int> par;
    int count;
    UnionFind(int n) : par(n, -1), count(0){}
    int Find(int x){return par[x] < 0 ? x : Find(par[x]);}
    int Size(int x){return par[x] < 0 ? -par[x] : Size(par[x]);}
    bool Unite(int x, int y){
        x = Find(x);
        y = Find(y);
        if(x == y)
            return false;
        if(par[x] > par[y])
            swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return ++count;
    }
};


```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "lib/classes/unionfind.cpp"
struct UnionFind{
    vector<int> par;
    int count;
    UnionFind(int n) : par(n, -1), count(0){}
    int Find(int x){return par[x] < 0 ? x : Find(par[x]);}
    int Size(int x){return par[x] < 0 ? -par[x] : Size(par[x]);}
    bool Unite(int x, int y){
        x = Find(x);
        y = Find(y);
        if(x == y)
            return false;
        if(par[x] > par[y])
            swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return ++count;
    }
};


```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
